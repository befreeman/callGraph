#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

static void exampleFunc(const char *filename) {
	printf("%s\n",filename);
	xmlParserCtxtPtr ctxt;
	xmlDocPtr doc;

	//new parser
	ctxt = xmlNewParserCtxt();
	if (ctxt == NULL) {
		fprintf(stderr, "PHAIL!-0\n");
		return;
	}
	
	//originally had XML_PARSE_DTDVALID for the final condition
	//This errored out as, and I could be wrong, srcML does not have a DTD	
	doc = xmlCtxtReadFile(ctxt, filename, NULL, XML_PARSE_RECOVER);
	if (doc == NULL) {
		fprintf(stderr, "PHAIL!-1\n");
	} else {
		int i=ctxt->valid;
		printf("%d\n",i);
		xmlFreeDoc(doc);
	}
	xmlFreeParserCtxt(ctxt);
	
	//
	
	
}

int main(void) {

	exampleFunc("xml.xml");

	xmlCleanupParser();
	xmlMemoryDump();

	printf("\ndone\n");

	return(0);
}
