#include <iostream>
#include <stdio.h>
#include "callGraphUtils.cpp"

//libxml libs
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

int main (int argc, char ** argv) {

	if (argc != 2) {exit(1);}

	//set up documents
	xmlDocPtr doc, callGraph;
	doc = xmlParseFile(argv[1]);
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

	xmlXPathObjectPtr test = xmlXPathEvalExpression(BAD_CAST "//src:function",docCtx);
	xmlXPathObjectPtr functionNames = xmlXPathEvalExpression(BAD_CAST "//src:function/src:name",docCtx);
	xmlXPathObjectPtr test2 = xmlXPathEvalExpression(BAD_CAST "//src:function/src:parameter_list",testCtx);

	for (int i = 0; i < test -> nodesetval -> nodeNr; ++i) {
		xmlNodePtr current = test -> nodesetval -> nodeTab[i];
		xmlNodePtr currentName = functionNames -> nodesetval -> nodeTab[i];

		//add names
		xmlNodePtr function = xmlNewNode(NULL, BAD_CAST "function");
		if (currentName -> children -> content) {
			xmlNodePtr functionName = xmlNewChild(function,NULL, BAD_CAST "name", currentName -> children -> content);
		}
		
		//run new XPath and get param list
		xmlXPathSetContextNode(test -> nodesetval -> nodeTab[i],testCtx);
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
		} else {
			//do nothing
		}	

		//add the function nodes at the end
		xmlAddChild(function,paramsNode);
		xmlAddChild(function,callNode);
		xmlAddChild(rootNode,function);
	}

	xmlDocDump(stdout, callGraph);
	xmlSaveFile("CALLGRAPH.xml",callGraph);
	xmlFreeDoc(callGraph);
	xmlCleanupParser();
	xmlMemoryDump();

	return 0;
}
