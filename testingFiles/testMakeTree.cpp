#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "callGraphUtils.cpp"

#if defined(LIBXML_TREE_ENABLED) && defined(LIBXML_OUTPUT_ENABLED)

int main (int argc, char **argv) 
{
	xmlDocPtr doc = NULL;
	xmlNodePtr root_node = NULL, node = NULL, node1 = NULL;
	xmlDtdPtr dtd = NULL;
	char buff[256];
	int i=0, j=0;
	
	LIBXML_TEST_VERSION;

	doc = xmlNewDoc(BAD_CAST "1.0");
	root_node = xmlNewNode(NULL, BAD_CAST "root");
	xmlDocSetRootElement(doc, root_node);

	dtd = xmlCreateIntSubset(doc, BAD_CAST "root", NULL, BAD_CAST "tree2.dtd");

	//with content
	xmlNewChild(root_node, NULL, BAD_CAST "function",BAD_CAST "Namd_of_function");

	//without content
	xmlNewChild(root_node, NULL, BAD_CAST "function2", NULL);

	node = xmlNewChild(root_node, NULL, BAD_CAST "node3", BAD_CAST "attributes");
	xmlNewProp(node, BAD_CAST "isAttribute", BAD_CAST "yes");
	xmlNewProp(node, BAD_CAST "type", BAD_CAST "int");

	node = xmlNewNode(NULL, BAD_CAST "node4");
	node1 = xmlNewText(BAD_CAST "Method 2");
	xmlAddChild(node, node1);
	xmlAddChild(root_node,node);

	xmlSaveFile("output", doc);

	xmlDocDump(stdout, doc);

	xmlFreeDoc(doc);
	xmlCleanupParser();
	xmlMemoryDump();

	return 0;
}

#endif
