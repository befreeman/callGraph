#include <iostream>

int foo();
int foo(int i);


int main () {

	std::cout << foo();
	foo(5);

	return 0;
}

int foo() {
	return 4;
}

int foo(int i) {
	return 4*i;
}

