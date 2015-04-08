#include <iostream>

void foo(int,char,double);
void foo1();

int main (int argc) 
{
	int i = 0;
	double d = 0;
	char c = 'a';

	foo(i,c,d);
	foo(i,c,d);
	foo1();
	foo(i,c,d);
	foo1();
	return 0;
}

void foo(int i,char c, double d) { foo1(); foo1();}
void foo1() {}
