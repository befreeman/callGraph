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

	//for each function
	for (int i = 0; i < functionXPath -> nodesetval -> nodeNr; ++i) {
		xmlXPathSetContextNode(functionXPath -> nodesetval -> nodeTab[i], testCtx);	
		xmlXPathObjectPtr testXPath = xmlXPathEvalExpression(BAD_CAST "child::src:name",testCtx);
		printInfo(testXPath -> nodesetval, std::cout);

		xmlNodePtr current = testXPath -> nodesetval -> nodeTab[i];
		
		xmlNodePtr functionNode = xmlNewNode(NULL, BAD_CAST "function");
		xmlNewChild(functionNode, NULL, BAD_CAST "name", BAD_CAST "name_here");
		xmlAddChild(rootNode,functionNode);
		
	}



	for (int i = 0; i < functionXPath -> nodesetval -> nodeNr; ++i) {
		xmlXPathSetContextNode(functionXPath -> nodesetval -> nodeTab[i], testCtx);
		xmlNodePtr test = testCtx -> node;
		std::cout << "\n" << test -> name << "\n";

		std::cout << "resetContext\n";
		xmlXPathObjectPtr testXPath = xmlXPathEvalExpression(BAD_CAST "descendant::src:call",testCtx);
		std::cout << "print stuff\n";
		printInfo(testXPath -> nodesetval, std::cout);
}

//	printInfo(functionXPath -> nodesetval, std::cout);

	std::cout << " \n\n\n### - call graph - ###\n";	
	xmlDocDump(stdout,callGraph);
	std::cout << "\n\n";

	xmlFreeDoc(callGraph);
	xmlFreeDoc(doc);
	xmlCleanupParser();
	xmlMemoryDump();

	return 0;
}
