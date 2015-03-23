#include <iostream>
#include <cassert>

//libxml2 libs
#include <libxml/tree.h> 
#include <libxml/parser.h> 
#include <libxml/xpath.h> 
#include <libxml/xpathInternals.h>


//int parseFile(const char *filename);
//void usage(const char *name);

void update(xmlNodeSetPtr nodes, const xmlChar *value) {
	int size;
	int i;

	assert(value);
	size = (nodes) ? nodes->nodeNr : 0;
	for (i = size - 1; i >= 0; --i) {
		xmlNodeSetContent(nodes->nodeTab[i], value);
		if (nodes->nodeTab[i]->type != XML_NAMESPACE_DECL) {
			nodes->nodeTab[i] = NULL;
		}
	}
}

int parseFile(xmlDocPtr& doc, const char *filename) {
	doc = xmlParseFile(filename);
	if (doc == NULL) {
		std::cerr << "Error: unable to parse file: " << filename << "\n";
		return -1;
	}
	return 0;
}

int createXPathEvalContext(xmlXPathContextPtr& xpathCtx, xmlDocPtr& doc) {
	xpathCtx = xmlXPathNewContext(doc);
	if (1) {;}
	return 0;
}

/* print usage information */
void usage(const char *name) {
	assert(name);
	std::cerr << "Usage: " << name << " <xml-file> <xpath-expr> <value>\n";	
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


