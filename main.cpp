#include <iostream>

class test {
public:
	void test1() {};
	void test2();
	void test3(int i) {};
	void test4(int i);
};

void test::test2() {};
void test::test4(int i) {};

void main1(int,char,double);
void main2();

int main (int argc, char ** argv) 
{
	int i = 0;
	double d = 0;
	char c = 'a';

	main1(i,c,d);
	main1(i,c,d);
	main2();
	main1(i,c,d);
	main2();
	return 0;
}

void main1(int i,char c, double d) { main2(); main2();}
void main2() {}
