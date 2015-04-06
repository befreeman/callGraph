#include <iostream>
#include <cassert>

//libxml2 libs
#include <libxml/tree.h> 
#include <libxml/parser.h> 
#include <libxml/xpath.h> 
#include <libxml/xpathInternals.h>

void printSiblings(xmlNodePtr current);
void printChildren(xmlNodePtr current);
void update(xmlNodeSetPtr nodes, const xmlChar *value);
int parseFile(xmlDocPtr& doc, const char *filename);
int createXPathEvalContext(xmlXPathContextPtr& xpathCtx, xmlDocPtr& doc);
void usage(const char *name);
void addFunctionNodes(xmlNodeSetPtr nodes, xmlNodePtr root);
void printInfo(xmlNodeSetPtr nodes, std::ostream& out);
void print(xmlNodeSetPtr nodes, FILE* output);
void createNodesFromXPath(xmlXPathObjectPtr xpath);

void printSiblings(xmlNodePtr current) {
	std::cout << "\n\n### - printSiblings - ###\n";
	if (current) {
		xmlNodePtr check = NULL;
		check = current;
		
	} else {
		std::cout << "no siblings \n";
	}
	std::cout << "\n\n";
	//xmlNodePtr next = current -> next;
}

void printChildren(xmlNodePtr current) {
	std::cout << "\n\n### - printChildren - ###\n";
	if (current) {
		xmlNodePtr temp = current;
		while (temp != NULL) {
			std::cout << temp -> name << " - ";
			temp = temp -> children;
		}
	}
}

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

void addFunctionNodes(xmlNodeSetPtr nodes, xmlNodePtr root) {
	std::cout << "### - in addFunctionNodes - ###\n";
	

	//if there are nodes, then get the number of nodes, else 0
	int size = (nodes) ? nodes -> nodeNr : 0;

	//out << "### - results - ###\n";
	for (int i = 0; i < size; ++i) {
		assert(nodes->nodeTab[i]);
		xmlNodePtr current = nodes->nodeTab[i];
		xmlNodePtr function = xmlNewNode(NULL, BAD_CAST "function");
		xmlNodePtr fname = xmlNewChild(function, NULL, BAD_CAST current -> name,BAD_CAST current -> children -> content);
		xmlAddChild(root, function);

		current = current -> next;
		std::cout << current -> name << "\n";
	}
}

void printInfo(xmlNodeSetPtr nodes, std::ostream& out) {
	assert(out);
	xmlNodePtr current;
	//if there are nodes, then get the number of nodes, else 0
	int size = (nodes) ? nodes -> nodeNr : 0;

	out << "### - results - ###\n";
	for (int i = 0; i < size; ++i) {
		assert(nodes->nodeTab[i]);
		current = nodes->nodeTab[i];
		out << current -> name << " : ";
		if (current -> children -> content) { 
			std::cout << current -> children -> content << "\n";
		} else {
			std::cout << " (null)\n";
		}
	}
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

std::string getContent(xmlNodePtr node) {
	xmlNodePtr current = node;
	while (current -> children != NULL) {
		current = current -> children;
	}
//	 s = current -> content;
	return "s";
}
