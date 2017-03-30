#include <iostream>
#include "extern_c.hpp"

using namespace std;

void func1()
{
	cout << "func1" << endl;
}

int main(int argc, char** argv)
{
	func1();
	
	return 0;
}
