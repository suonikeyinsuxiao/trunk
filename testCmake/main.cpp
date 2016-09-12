#include <iostream>
#include "hello.h"

int main()
{
    std::cout<<"Hello World!"<<std::endl;

#ifdef HAVE_PL_TEST1
    std::cout<<"HAVE_PL_TEST1!"<<std::endl;
#else
    std::cout<<"no HAVE_PL_TEST1!"<<std::endl;
#endif
	return 0;

}
