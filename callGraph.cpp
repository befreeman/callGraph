#include <iostream>
#include <stdio.h>
#include <sstream>
//libxml libs
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>


//get the line number and convert it to a string
std::string getLineNumber(xmlNodePtr node) {
	std::string result;
	std::stringstream ss;
	ss << xmlGetLineNo(node);
	return ss.str();
}

//set applicable SRCML namespaces here
void setSrcmlNS(xmlXPathContextPtr ctx) {
	xmlXPathRegisterNs(ctx, BAD_CAST "src", BAD_CAST "http://www.sdml.info/srcML/src");
 	xmlXPathRegisterNs(ctx, BAD_CAST "cpp", BAD_CAST "http://www.sdml.info/srcML/cpp");
}

//get the node name and return a pointer to a new node containing the node name
xmlNodePtr getFullName(xmlNodePtr node) {

	xmlNodePtr result = xmlNewNode(NULL, BAD_CAST "name");;
	//if there is content in the node passed in
	//(xmlNodeIsText returns 1 if text node, 0 if not
	if (xmlNodeIsText(node -> children) ) {
		//std::cout << "no-namespace: " << node -> children -> content << "\n";
		xmlNodeAddContent(result, node -> children -> content);
	
	//else it should be <name><operator><name> as children nodes
	} else if (xmlNodeIsText(node -> children -> children)) {
		//std::cout << "namespaced: " << node -> children -> children -> content << "\n";
		xmlChar * namePlaceholder = xmlStrcat(node -> children -> children -> content, node -> children -> next -> children -> content);
		namePlaceholder = xmlStrcat (namePlaceholder, node -> children -> next -> next -> children -> content);
		xmlNodeAddContent(result, namePlaceholder);
	}
	return result;
}


//create the call graph based off of the xource code
int main (int argc, char ** argv) {

	/////////////////////////////////////////////////////////////////////////////
	//open the source document and create a context for it
	xmlDocPtr doc, callGraph;
	doc = xmlParseFile(argv[1]);
	//exit if doc is null
	if (doc == NULL) { std::cout << "could not open file\n"; return (1); }
	xmlXPathContextPtr docCtx = xmlXPathNewContext(doc);
	setSrcmlNS(docCtx);

	/////////////////////////////////////////////////////////////////////////////
	//setup the call graph document (root node, namespaces, xmlContext)
	//setup the context for 
	callGraph = xmlCopyDoc(doc, 0);
	xmlNodePtr rootNode = xmlNewNode(NULL, BAD_CAST "callGraph");
	xmlDocSetRootElement(callGraph, rootNode);

	/////////////////////////////////////////////////////////////////////////////
	//get initial list of functions, the function names, and the parameter lists
	xmlXPathObjectPtr functions = xmlXPathEvalExpression(BAD_CAST "//src:function",docCtx);
	xmlXPathObjectPtr functionNames = xmlXPathEvalExpression(BAD_CAST "//src:function/src:name",docCtx);
	xmlXPathObjectPtr test2 = xmlXPathEvalExpression(BAD_CAST "//src:function/src:parameter_list",docCtx);

	/////////////////////////////////////////////////////////////////////////////
	// create the call graph document
	/////////////////////////////////////////////////////////////////////////////
	//go through each function and get the name, param list, etc
	for (int i = 0; i < functions -> nodesetval -> nodeNr; ++i) {

		//get the current function
		xmlNodePtr current = functions -> nodesetval -> nodeTab[i];
		xmlNodePtr currentName = functionNames -> nodesetval -> nodeTab[i];
		xmlNodePtr function = xmlNewNode(NULL, BAD_CAST "function");
		
		//get the line number
		std::string lineNumber = getLineNumber(current);
		xmlNewChild(function, NULL, BAD_CAST "line_number", BAD_CAST lineNumber.c_str());
		
		//add the name of the function
		xmlNodePtr functionName = getFullName(currentName);
		xmlAddChild(function,functionName);

		//set the XPath context to the current node
		//
		xmlXPathSetContextNode(functions -> nodesetval -> nodeTab[i],docCtx);

		//run the xpath 
		xmlXPathObjectPtr paramsXPath = xmlXPathEvalExpression(BAD_CAST "child::src:parameter_list/descendant::src:type/src:name",docCtx);
		xmlNodePtr paramsNode = xmlNewNode(NULL, BAD_CAST "parameter_list");

		/////////////////////////////////////////////////////////////////////////////
		//add parameters to the function
		if (paramsXPath -> nodesetval != NULL) {
			for (int j = 0; j < paramsXPath -> nodesetval -> nodeNr; ++j) {
				xmlNodePtr current = paramsXPath -> nodesetval -> nodeTab[j];
				xmlNodePtr paramName = getFullName(current);
				xmlAddChild(paramsNode,paramName);
			}
		}
		
		/////////////////////////////////////////////////////////////////////////////
		//get the list of calls
		xmlXPathObjectPtr callXPath = xmlXPathEvalExpression(BAD_CAST "child::src:block/descendant::src:call/src:name",docCtx);
		xmlNodePtr callNode = xmlNewNode(NULL, BAD_CAST "calls");
		//add the calls to the function
		if (callXPath -> nodesetval != NULL) {
			for (int j = 0; j < callXPath -> nodesetval -> nodeNr; ++j) {
				xmlNodePtr current = callXPath -> nodesetval -> nodeTab[j];
				xmlNodePtr callName = getFullName(current);
				std::string lineNumber = getLineNumber(current);
				xmlNewChild(callName, NULL, BAD_CAST "line_number", BAD_CAST lineNumber.c_str());
				xmlAddChild(callNode,callName);
			}
		}

		/////////////////////////////////////////////////////////////////////////////	
		//add the function nodes at the end
		xmlAddChild(function,paramsNode);
		xmlAddChild(function,callNode);
		xmlAddChild(rootNode,function);
	}
	
	/////////////////////////////////////////////////////////////////////////////		
	//save file and clean up memory
	if (argc > 2) {
		if (xmlSaveFile(argv[2],callGraph) == -1) {
			std::cout << "could not save to the specified file name\n";
			return (-1);
		}
	} else {
		xmlDocDump(stdout, callGraph);
	}
	xmlFreeDoc(callGraph);
	xmlCleanupParser();
	xmlMemoryDump();

	//fin
	return 0;
}

//llama
