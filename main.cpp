#include <iostream>

void foo(int,char,double);
void foo1();

int main (int argc) 
{
	foo();
	foo();
	foo1();
	foo();
	foo1();
	return 0;
}

void foo(int i,char c, double d) { foo1(); foo1();}
void foo1() {}
