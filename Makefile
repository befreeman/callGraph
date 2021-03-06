current: clear xml
	clang++ callGraph.cpp -I/usr/include/libxml2/ -lxml2
	./a.out xml.xml CALLGRAPH.xml
	xmllint --format CALLGRAPH.xml > prettyCG.xml

prettify: clear
	clang++ prettify.cpp -I/usr/include/libxml2/ -lxml2 -o prettify
	./prettify CALLGRAPH.xml prettyGraph.xml
	rm -rf prettify
	

default:
	@echo 'make will run the following targets'
	@echo '   clean'

betterOutput:
	clang testBetterOutput.c -I/usr/include/libxml2/ -lxml2
	./a.out
	rm -rf a.out

callGraphOld:
	clang++ callGraph.cpp -I/usr/include/libxml2/ -lxml2
	./a.out xml.xml 
	rm -rf a.out

makeTree:
	clang++ testMakeTree.cpp -I/usr/include/libxml2/ -lxml2
	./a.out xml.xml //src:function/src:name function2oiuh
	rm -rf a.out


parserContect:
	clang testParserContext.c -I/usr/include/libxml2/ -lxml2
	./a.out
	rm -rf a.out


xmlReadFile:
	clang testXmlReadFile.c -I/usr/include/libxml2/ -lxml2
	./a.out
	rm -rf a.out

xpath:
	clang testXPath.c -I/usr/include/libxml2/ -lxml2
	./a.out
	rm -rf a.out

xpathExpr:
	clang testXPathExpr.c -I/usr/include/libxml2/ -lxml2
	./a.out
	rm -rf a.out

xpathModify:
	clang++ testXPathModify.cpp -I/usr/include/libxml2/ -lxml2
	./a.out xml.xml //src:function/src:name function2oiuh
	rm -rf a.out



# compile files
func.o:
	clang++ -c func.cpp 

main.o:
	clang++ -c main.cpp

# create executables
main: main.o func.o
	clang++ main.o func.o -o test

# create srcML files
xml:
	make clean
	srcml main.cpp -o xml.xml

# run everything and clean
run: main 
	./test
	make clean
# clean files


clear:
	clear

clean:
	rm -rf *.o *.out *.gch test xml a.out xml.xml *.o
	clear
