#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

#ifdef LIBXML_TREE_ENABLED 
#ifdef LIBXML_SAX1_ENABLED

void print(xmlNodeSetPtr nodes, FILE* output);

int main () {
	xmlDocPtr doc;
	xmlXPathContextPtr xpathCtx;
	xmlXPathObjectPtr xpathFunction, xpathCall;

	doc = xmlParseFile( "xml.xml" );
	xpathCtx = xmlXPathNewContext(doc);

	xmlXPathRegisterNs(xpathCtx, BAD_CAST "src", BAD_CAST "http://www.sdml.info/srcML/src");
	xmlXPathRegisterNs(xpathCtx, BAD_CAST "cpp", BAD_CAST "http://www.sdml.info/srcML/cpp");


	xpathFunction = xmlXPathEvalExpression(BAD_CAST "//src:function",xpathCtx);
	xpathCall = xmlXPathEvalExpression(BAD_CAST "//src:call",xpathCtx);

	print(xpathFunction -> nodesetval, stdout);
	print(xpathCall -> nodesetval, stdout);

	return 0;
}


void print(xmlNodeSetPtr nodes, FILE* output) {
	xmlNodePtr current;
	int size, i;

	assert(output);
	size = (nodes) ? nodes -> nodeNr : 0;

	fprintf(output, "Result (%d nodes):\n",size);	
	for (i = 0; i < size; ++i) {
		assert(nodes -> nodeTab[i]);
		
		if (nodes -> nodeTab[i] -> type == XML_NAMESPACE_DECL) {
			printf("XML_NAMESPACE_DECL\n");
		} else if (nodes -> nodeTab[i] -> type == XML_ELEMENT_NODE) {
			printf("XML_ELEMENT_NODE\n");

			current = nodes->nodeTab[i];   	    
	    if(current->ns) { 
    	        fprintf(output, "= element node \"%s:%s\"\n", 
		    current->ns->href, current->name);
							fprintf(output, "   node name: %s\n", current->name);
	    } else {
    	        fprintf(output, "= element node \"%s\"\n", 
		    current->name);
	    }


		} else {
			current = nodes -> nodeTab[i];
			fprintf(output, "=node \"%s\": type %d\n", current -> name, current -> type);
		}
	}
}



#endif
#endif
