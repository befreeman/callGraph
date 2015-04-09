#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

int main(void) {

	printf("beginning xml test to read and release xml with libxml2 \n");

	printf("\nmake an xmlDocPtr\n");
	
	xmlDocPtr doc;

	printf("\nmade pointer, read in document??\n");

	doc = xmlReadFile("xml.xml",NULL,1);

	printf("\nread in document\n");

	printf("free document\n");

	xmlFreeDoc(doc);

	printf("clean up\n");

	xmlCleanupParser();

	xmlMemoryDump();	

	return(0);
}
