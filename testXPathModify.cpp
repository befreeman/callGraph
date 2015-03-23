#include <iostream>
#include <cassert>
#include <cstdlib>
#include "callGraphUtils.cpp"
//libxml2
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

#if defined(LIBXML_XPATH_ENABLED) && defined(LIBXML_SAX1_ENABLED) && defined(LIBXML_OUTPUT_ENABLED)

const bool DEBUG = true;

static int example4(const char *filename, const xmlChar *xpathExpr, const xmlChar *value);

int main (int argc, char **argv) {

	if (DEBUG) { std::cout << "\n\n### start of program ###\n\n"; }

	if (DEBUG) {
		//print out command line parameters
		for (int i = 0; i < argc; ++i) { std::cout << "parameter #" << i << ": " << argv[i] << "\n"; }
	}
	
	//initialize the libxml parser
	xmlInitParser();
	LIBXML_TEST_VERSION

	//do stuff
	if (example4(argv[1], BAD_CAST argv[2], BAD_CAST argv[3]) != 0 ) //main function does not run
  {
		usage(argv[0]);
		return -1;
	}

	//shutdown libxml 
	xmlCleanupParser();

	//debug for memory regression tests
	xmlMemoryDump();
	
	if (DEBUG) { std::cout << "\n\n### end of program ###\n\n"; }

	return 0;
}

static int example4(const char *filename, const xmlChar *xpathExpr, const xmlChar *value) {
	xmlDocPtr doc;
	xmlXPathContextPtr xpathCtx;
	xmlXPathObjectPtr xpathObj;

	assert(filename && xpathExpr && value);

	// load the xml
	doc = xmlParseFile(filename);
	if (doc == NULL) { 
		std::cerr << "Error: unable to parse file: " << filename << "\n"; 
		return -1;
	}

	// create an xpath evaluation context
	xpathCtx = xmlXPathNewContext(doc);
	if (xpathCtx == NULL) { 
		std::cerr << "Error: unable to create new XPath Context\n";
		return -1;
	}

	xmlXPathRegisterNs(xpathCtx, BAD_CAST "src", BAD_CAST "http://www.sdml.info/srcML/src");
	xmlXPathRegisterNs(xpathCtx, BAD_CAST "cpp", BAD_CAST "http://www.sdml.info/srcML/cpp");

	xpathObj = xmlXPathEvalExpression(xpathExpr, xpathCtx);
	if (xpathObj == NULL) {
		std::cerr << "Error, unable to evaluate xpath expression " << xpathExpr << "\n";
		xmlXPathFreeContext(xpathCtx);
		xmlFreeDoc(doc);
		return -1;
	}	

	print(xpathObj -> nodesetval,stdout);

	update(xpathObj->nodesetval, value);

	//clean up
	xmlXPathFreeObject(xpathObj);
	xmlXPathFreeContext(xpathCtx);
	xmlDocDump(stdout, doc);
	xmlFreeDoc(doc);


	return 0;
}


#endif
