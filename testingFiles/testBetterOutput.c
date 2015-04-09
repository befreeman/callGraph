#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#ifdef LIBXML_TREE_ENABLED

int count = 1;

static void
print_element_names(xmlNode * a_node) {


	xmlNode *current_node = NULL;

	for (current_node = a_node; current_node; current_node = current_node -> next) {
		{
			if (current_node->type != XML_TEXT_NODE) {
				printf("<%s>\n", current_node->name);
				for (int i=0; i < count; ++i) { printf("-");}
				++count;
				print_element_names(current_node -> children);
				--count;
			} else if (current_node -> type == XML_TEXT_NODE) {
//				printf("%s",current_node->content);
			}
//			printf("\n");
		}
	}
}

int
main(void) {

	xmlDoc *doc = NULL;
	xmlNode *root_element = NULL;

	LIBXML_TEST_VERSION

	doc = xmlReadFile("xml.xml",NULL,0);
	
	if (doc == NULL) {
		printf("PHAIL-0!\n");
	} 

	root_element = xmlDocGetRootElement(doc);

	print_element_names(root_element);

	xmlFreeDoc(doc);
	xmlMemoryDump();

	return(0);
}

#endif
