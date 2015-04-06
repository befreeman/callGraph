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

	//set namespaces for XPath expressions
	xmlXPathRegisterNs(docCtx, BAD_CAST "src", BAD_CAST "http://www.sdml.info/srcML/src");
  	xmlXPathRegisterNs(docCtx, BAD_CAST "cpp", BAD_CAST "http://www.sdml.info/srcML/cpp");

  	//evaluate XPath expressions and find all functions and calls
  	xmlXPathObjectPtr functionTypeXPath, functionNameXPath, functionParamsXPath, callsXPath, functionXPath;
  	functionXPath = xmlXPathEvalExpression(BAD_CAST "//src:function/src:name", docCtx);
  	functionNameXPath = xmlXPathEvalExpression(BAD_CAST "//src:function | //src:function/src:parameter_list", docCtx);

  	for (int i=0; i < functionNameXPath -> nodesetval -> nodeNr; ++i) {
  		printChildren(functionNameXPath -> nodesetval -> nodeTab[i]);
  	}

  	std::cout << "\n\n";

  	//set up the callGraph xml
  	xmlNodePtr root_node = NULL;
	root_node = xmlNewNode(NULL, BAD_CAST "call_graph");
	xmlDocSetRootElement(callGraph, root_node);

	//add nodes from functionXPath
	addFunctionNodes(functionXPath -> nodesetval, root_node);

	printSiblings(functionXPath -> nodesetval -> nodeTab[0]);

	printInfo(functionXPath -> nodesetval, std::cout);

	//std::cout << "### - this is the original - ###\n";	
	//xmlDocDump(stdout,doc);
	std::cout << "\n### - this is the callGraph - ###\n";
	xmlDocDump(stdout,callGraph);
	xmlSaveFile("output.xml",callGraph);

	return 0;
}
