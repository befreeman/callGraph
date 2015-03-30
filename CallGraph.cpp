#include <iostream>
#include <stdio.h>

//libxml libs
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

int main (int argc, char ** argv) {

	if (argc != 2) {exit(1);}

	xmlDocPtr doc, callGraph;
	doc = xmlParseFile(argv[1]);
	callGraph = xmlCopyDoc(doc, 0);


	
	xmlDocDump(stdout,doc);
	std::cout << "\t\t\t----------\n";
	xmlDocDump(stdout,callGraph);

	return 0;
}
