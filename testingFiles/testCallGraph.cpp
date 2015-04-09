#include <iostream>
#include <stdio.h>

//libxml libs
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

int main (int argc, char ** argv) {

	//open the xml file
	xmlDocPtr doc;
	doc = xmlParseFile(argv[1]);

	//make context
	xmlXPathContextPtr docCtx;
	docCtx = xmlXPathNewContext(doc);

	//get XPath for functions
	xmlXPathRegisterNs(docCtx, BAD_CAST "src", BAD_CAST "http://www.sdml.info/srcML/src");
	xmlXPathRegisterNs(docCtx, BAD_CAST "cpp", BAD_CAST "http://www.sdml.info/srcML/cpp");
	xmlXPathObjectPtr functionNodes = xmlXPathEvalExpression(BAD_CAST "//src:function", docCtx);
		
	//create new doc for the cal graph
	xmlDocPtr callGraph;
	callGraph = xmlNewDoc(BAD_CAST "1.0");
	xmlNodePtr rootNode;
	rootNode = xmlNewNode(NULL, BAD_CAST "callGraph");
	xmlDocSetRootElement(callGraph,rootNode);


	//add function nodes to graph
	xmlNodePtr * temp = functionNodes -> nodesetval -> nodeTab;
	int count = functionNodes -> nodesetval -> nodeNr;
	for (int i =0; i < count; ++i) {
		xmlNodePtr functionNode = xmlNewNode(NULL, BAD_CAST "function");
		xmlNodePtr typeNode = xmlNewNode(NULL, BAD_CAST "type");
		xmlNodePtr typeNameNode = xmlNewNode(NULL, BAD_CAST "name");

		std::cout << temp[i] -> children -> children -> children -> content;
		std::cout << temp[i] -> children -> content;

		xmlAddChild(typeNode,typeNameNode);
		xmlAddChild(functionNode,typeNode);
		xmlAddChild(rootNode, functionNode);
		std::cout << temp[i] -> name << " ";
	}
	std::cout << "\n";

	//create dtd??
	//xmlDtdPtr callGraphDTD;
	//callGraphDTD = xmlCreateIntSubset(callGraph, BAD_CAST "root", NULL, BAD_CAST "call_graph");

	//create root node
		
	
	//test new doc
	xmlDocDump(stdout,callGraph);
	//	xmlDocDump(stdout,doc);

	//clean up
	xmlFreeDoc(doc);
	xmlCleanupParser();
	xmlMemoryDump();
	return 0;
}
