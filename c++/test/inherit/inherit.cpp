#include <iostream>

using namespace std;

/*
 *对于纯虚函数可以实现也可以不实现,但派生类一定要覆盖基类的纯虚函数.
 *
 *如果派生类覆盖基类的虚函数,则基类的指针或引用实际指向子类对象,那么通过该指针或引用调用虚函数时,将调用子类的虚函数;如果不是虚函数,则调用父类的函数.
 *
 */
class PEOPLE
{
public:
	int age;
	//派生类必须实现抽象类的纯虚函数,否则派生类仍然是抽象类
	virtual void speak() =0;
	virtual void play();
	virtual void play(int);
	void eat();
};

void PEOPLE::speak()
{
	cout << "PEOPLE speak..." << endl;
}

void PEOPLE::eat()
{
	cout << "PEOPLE eat..." << endl;
}

void PEOPLE::play()
{
	cout << "PEOPLE play..." << endl;
}

void PEOPLE::play(int id)
{
	cout << "PEOPLE play, id=" << id << endl;
}

class STUDENT :public PEOPLE
{
	public:
		STUDENT()
		{
			id = 1;	
		}

		void speak()
		{
			cout << "stu speak..."<< endl;
		}

		void eat()
		{
			cout << "stu eat" << endl;
		}

		
		void play()
		{
			cout << "stu play..." << endl;
		}

		void play(int id)
		{
			cout << "stu play, id = " << id << endl;
		}

		int id;

};

int main(int argc, char** argv)
{
	//抽象类不能实例化
	//PEOPLE p;	
	//cout << "sizeof PEOPLE:" << sizeof(p) << endl;

	STUDENT stu;
	cout << "sizeof STUDENT:" << sizeof(stu) << endl;
	cout << "stu addr:" << &stu << endl;
	stu.age = 1;
	cout << "stu age:" << stu.age << "	age addr:" << &stu.age << endl;
	cout << "stu id:" << stu.id << "	id addr:" << &stu.id << endl;

	STUDENT* pstu = &stu;
	PEOPLE* p = &stu;	
	cout << "p=" << p << endl;
	cout << "age:" << p->age << "	age addr:" << &p->age << endl;
	//派生类必须实现纯虚函数speak();
	p->speak();
	//仍然调用基类的非虚函数,没有多态特性
	p->eat();
	pstu->eat();
	//若派生类没有覆盖基类的虚函数play(),那么调用基类的play();
	//若派生类覆盖了基类的虚函数play(),那么调用派生类的play()
	p->play();
	pstu->play();

	p->play(10);
	pstu->play(10);

	//函数重载是类内部同名函数之间的关系,函数覆盖是父子类虚函数之间的关系
	return 0;
}
