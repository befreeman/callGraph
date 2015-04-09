#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

#ifdef LIBXML_SAX1_ENABLED
#ifdef LIBXML_TREE_ENABLED

static void usage(const char *name);
int runXpath(const char* filename, const xmlChar *xpath, const xmlChar *nsList);
int register_namespaces(xmlXPathContextPtr xpathCtx, const xmlChar* nsList);
void print(xmlNodeSetPtr nodes, FILE* output);


int main (int argc, char ** argv) {

	//init libxml
	xmlInitParser();
	LIBXML_TEST_VERSION
	
	//run main job
	if (runXpath( "xml.xml", BAD_CAST "//person", 
//			"<src>=<http://www.sdml.info/srcML/src> <cpp=http://www.sdml.info/srcML/cpp>"
			NULL
			) < 0) {
		usage(argv[0]);
		return(-1);
	}

	//shutdown libxml
	xmlCleanupParser();

	//debug memory
	xmlMemoryDump();

	return 0;
}

//print usage information
static void usage(const char *name) {
	assert(name);

	fprintf(stderr, "Usage: %s <xml-file> <xpath-expr> [<known-ns-list>]\n", name);
  fprintf(stderr, "where <known-ns-list> is a list of known namespaces\n");
  fprintf(stderr, "in \"<prefix1>=<href1> <prefix2>=href2> ...\" format\n");
}

// @filename: input xml filename
// @xpath:		the xpath expression
// @nsList:		options list of known namespaces as "<prefix1>=<href1>..."
// parses input file, evaluates xpath, prints results
// returns 0 on success and negative otherwise
int runXpath(const char* filename, const xmlChar *xpath, const xmlChar *nsList) {
	xmlDocPtr doc;
	xmlXPathContextPtr xpathCtx;
	xmlXPathObjectPtr xpathObj;

	assert(filename);
	assert(xpath);

	// Load the xml file
	doc = xmlParseFile(filename);
	if (doc == NULL) {
		fprintf(stderr, "Error: unable to parse file \"%s\"\n", filename);
	}

	//create xpath evaluation context
	xpathCtx = xmlXPathNewContext(doc);
	if (xpathCtx == NULL) {
		fprintf(stderr, "Error: unable to create new XPath context\n");
		xmlFreeDoc(doc);
		return (-1);
	}
	
	//register namespaces from the list (not inplementing
	if ((nsList != NULL) && (register_namespaces(xpathCtx, nsList)) < 0) {
		fprintf(stderr, "Error: failed to register namespaces list \"%s\"\n", nsList);
		xmlXPathFreeContext(xpathCtx);
		xmlFreeDoc(doc);
		return (-1);
	}

	//evaluate the xpath expression (what I've been waiting for!)
	xpathObj = xmlXPathEvalExpression(xpath, xpathCtx);
	if (xpathObj == NULL) {
		fprintf(stderr, "Error: unable to evaluate xpath expression \"%s\"\n",xpath);
		xmlXPathFreeContext(xpathCtx);
		xmlFreeDoc(doc);
		return (-1);
	}

	//print results
	print(xpathObj -> nodesetval, stdout);

	//spring cleaning
	xmlXPathFreeObject(xpathObj);
	xmlXPathFreeContext(xpathCtx);
	xmlFreeDoc(doc);

	return (0);
}

int register_namespaces(xmlXPathContextPtr xpathCtx, const xmlChar* nsList) {
	xmlChar* nsListDup;
	xmlChar* prefix;
	xmlChar* href;
	xmlChar* next;

	assert(xpathCtx);
	assert(nsList);

	nsListDup = xmlStrdup(nsList);
	if (nsListDup == NULL) {
		fprintf(stderr, "Error: unable to strdup namespaces list\n");
		return(-1);
	}

	next = nsListDup;
	while (next != NULL) {
		while ((*next) == ' ') {next++;}
		if ((*next) == '\0') {break;}

		prefix = next;
		next = (xmlChar*)xmlStrchr(next, '=');
		if (next == NULL) {
			fprintf(stderr, "Error: invalid namespaces list format\n");
			xmlFree(nsListDup);
			return(-1);
		}
		*(next++) = '\0';

		href = next;
		next = (xmlChar*)xmlStrchr(next, ' ');
		if (next != NULL) {
			*(next++) = '\0';
		}

		if (xmlXPathRegisterNs(xpathCtx, prefix, href) != 0) {
			fprintf(stderr, "Error: unable to register NS wirh prefix=\"%s\" and href =\"%s\"\n",prefix,href);
			xmlFree(nsListDup);
			return(-1);
		}
	}

	xmlFree(nsListDup);
	return (0);


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
