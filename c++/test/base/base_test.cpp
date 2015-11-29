
#include <iostream>

using namespace std;

class A
{
	public:
		//static 成员不占用类对象的内存空间,且必须在类外定义
		const static int css;
		static const int cs; 
		static int s;
		static int s2;
		int n;
		char ch;//4字节对齐,类A对象占用8个字节

		//类对象并不为函数成员分配内存
		void func(void);		
		static void staticfunc(void);//static函数类中声明,类外定义
		static void staticfunc2(void)//static函数在类中声明并定义
		{
			cout << "class A static func2 test..." << endl;
		}
		virtual void vfunc(void);//当类中含有虚函数时,类对象的大小＋4,vtable pointer, &vptr地址即类对象的地址 

		virtual void vfunc2(void)
		{
			cout << "class A virtual func2 test..." << endl;	
		}

		//包含纯虚函数的类不能定义对象，一般作为抽象类, 抽象类不能实例化
		//virtual void purevfunc(void)  =0;
};

int A::s = 1;//成员s 被分配在全局静态区,只能通过类名进行访问
int A::s2 = 2;
const int A::css = 0;//成员css 被分配在常量区,也只能通过类名进行访问
const int A::cs = 0;


void A::func(void)
{
	cout << "class A func test" << endl;
}

void A::staticfunc(void)
{
	cout << "class A static func test..." << endl;
	cout << "s=" << A::s << endl;
}

void A::vfunc()
{
	cout << "class A virtual func test..." << endl;
}

//一个空类的对象占用1个字节的空间.
class B
{
};

int main(int argc, char** argv)
{
	A a;
	B b;
	cout << "sizeof b:"<< sizeof(b)  << endl;	

	cout << "sizeof class a:"<< sizeof(a)  << endl;	
	cout << "a adr:" << &a << endl;
	cout << "a.n addr" << &a.n << endl;

	cout << "static s:" << A::s << "    addr s:" << &(A::s) <<  endl;
	cout << "static s2:" << A::s2 << "    addr s2:" << &A::s2 <<  endl;
	cout << "const static css:" << A::css << "     addr css:" << &(A::css) <<  endl;
	//a.cs = 10;
	cout << "const cs:" << A::cs<< "    addr cs: " << &A::cs << endl;
	
	//想测试成员函数的地址，但打印和预想不一样
	a.func();
	void (A::*p)(void)  = &A::func ;
	cout << "func addr : "<< p << endl;


	A::staticfunc();//类名直接调用的方式
	a.staticfunc();//对象调用的方式
	A::staticfunc2();
	a.staticfunc2();

	a.vfunc();
	//A::vfunc();//不能通过类名调用虚函数
	a.vfunc2();

	return 0;
}
