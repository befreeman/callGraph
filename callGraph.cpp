#include <iostream>
#include <stdio.h>
#include <sstream>
//libxml libs
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

int main (int argc, char ** argv) {

	//set up documents
	xmlDocPtr doc, callGraph;
	doc = xmlParseFile(argv[1]);
	//exit if doc is null
	if (doc == NULL) { std::cout << "could not open file\n"; return (1); }

	//setup the source document 
	callGraph = xmlCopyDoc(doc, 0);
	xmlXPathContextPtr docCtx = xmlXPathNewContext(doc);

	//set root node of the call graph
	xmlNodePtr rootNode = xmlNewNode(NULL, BAD_CAST "callGraph");
	xmlDocSetRootElement(callGraph, rootNode);
	
	//set namespaces for XPath expressions
	xmlXPathRegisterNs(docCtx, BAD_CAST "src", BAD_CAST "http://www.sdml.info/srcML/src");
 	xmlXPathRegisterNs(docCtx, BAD_CAST "cpp", BAD_CAST "http://www.sdml.info/srcML/cpp");

 	//evaluate XPath expressions and find all functions and calls
 	xmlXPathObjectPtr functionXPath = xmlXPathEvalExpression(BAD_CAST "//src:function", docCtx);
	xmlXPathContextPtr testCtx = xmlXPathNewContext(doc);
	xmlXPathRegisterNs(testCtx, BAD_CAST "src", BAD_CAST "http://www.sdml.info/srcML/src");
 	xmlXPathRegisterNs(testCtx, BAD_CAST "cpp", BAD_CAST "http://www.sdml.info/srcML/cpp");

	//get initial list of functions
	xmlXPathObjectPtr functions = xmlXPathEvalExpression(BAD_CAST "//src:function",docCtx);

	//get the corresponding list of names and parameters
	xmlXPathObjectPtr functionNames = xmlXPathEvalExpression(BAD_CAST "//src:function/src:name",docCtx);
	xmlXPathObjectPtr test2 = xmlXPathEvalExpression(BAD_CAST "//src:function/src:parameter_list",testCtx);

	//go through each function and get the name, param list, etc
	for (int i = 0; i < functions -> nodesetval -> nodeNr; ++i) {

		//get the current function
		xmlNodePtr current = functions -> nodesetval -> nodeTab[i];
		xmlNodePtr currentName = functionNames -> nodesetval -> nodeTab[i];
	
		xmlNodePtr function = xmlNewNode(NULL, BAD_CAST "function");
		
		//get the line number (and convert from long to c-str)
		std::stringstream ss;
		ss << xmlGetLineNo(current);
		std::string s = ss.str();
		xmlNewProp(function, BAD_CAST "line_number", BAD_CAST s.c_str() );
		
		//add the name of the function
		if (currentName -> children -> content) {
			xmlNodePtr functionName = xmlNewChild(function,NULL, BAD_CAST "name", currentName -> children -> content);
		} else if (currentName -> children -> children -> content) {
			//probably a namespace, try to get it
			xmlChar * namePlaceholder = xmlStrcat(currentName -> children -> children -> content,currentName -> children -> next -> children -> content);
			namePlaceholder = xmlStrcat (namePlaceholder, currentName -> children -> next -> next -> children -> content);
			std::cout << namePlaceholder << "\n"; 
			xmlNodePtr functionName = xmlNewChild(function, NULL, BAD_CAST "name", namePlaceholder); 
		}
		//run new XPath and get param list
		xmlXPathSetContextNode(functions -> nodesetval -> nodeTab[i],testCtx);
		xmlXPathObjectPtr paramsXPath = xmlXPathEvalExpression(BAD_CAST "child::src:parameter_list/descendant::src:type/src:name",testCtx);
		xmlNodePtr paramsNode = xmlNewNode(NULL, BAD_CAST "parameter_list");
		//add params to the function
		if (paramsXPath -> nodesetval != NULL) {
			for (int j = 0; j < paramsXPath -> nodesetval -> nodeNr; ++j) {
				xmlNodePtr current = paramsXPath -> nodesetval -> nodeTab[j];
				if (current -> children -> content) {
					xmlNodePtr paramName = xmlNewChild(paramsNode, NULL, BAD_CAST "type", current -> children -> content);
				} else {
					//do nothing
				}
			}
		} else {
			//do nothing
		}

		//get the list of calls
		xmlXPathObjectPtr callXPath = xmlXPathEvalExpression(BAD_CAST "child::src:block/descendant::src:call/src:name",testCtx);
		xmlNodePtr callNode = xmlNewNode(NULL, BAD_CAST "calls");
		//add the calls to the function
		if (callXPath -> nodesetval != NULL) {
			for (int j = 0; j < callXPath -> nodesetval -> nodeNr; ++j) {
				xmlNodePtr current = callXPath -> nodesetval -> nodeTab[j];
				if (current -> children -> content) {
					xmlNodePtr callName = xmlNewChild(callNode, NULL, BAD_CAST "name", current -> children -> content);
				} else {
					std::cout << "no content\n";
				}
			}
		}	
	
		//add the function nodes at the end
		xmlAddChild(function,paramsNode);
		xmlAddChild(function,callNode);
		xmlAddChild(rootNode,function);
	}
	

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

	return 0;
}
