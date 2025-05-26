#include<iostream>

class Student {
public:
	Student(): m_name(""), m_age(0) {
		std::cout << "默认构造被调用" << std::endl;
	}
	Student(const std::string& in_name, int in_age): m_name(in_name), m_age(in_age) {
		std::cout << "带参构造被调用" << std::endl;
	}
	Student(const Student& other): m_name(other.m_name), m_age(other.m_age) {
		std::cout << "拷贝构造被调用" << std::endl;
	}
	Student& operator= (const Student& other) {
		std::cout << "拷贝赋值运算符被调用" << std::endl;
		m_name = other.m_name;
		m_age = other.m_age;
		return *this;
	}
	~Student() {
		std::cout << "析构被调用" << std::endl;
	}
private:
	std::string m_name;
	int m_age;
};



int main() {
	Student s1("Tom", 20);
	Student s2 = s1;     // 拷贝构造
	Student s3;
	s3 = s1;             // 拷贝赋值
	return 0;
}

/*
如果你没有写构造函数，编译器会为你生成哪些函数？

会生成6种函数
1、默认构造函数
2、析构函数
3、拷贝构造函数
4、拷贝赋值函数
5、移动构造函数
6、移动赋值运算符

构造函数和拷贝构造的本质区别是什么？

构造函数是根据传入参数进行赋值构造，而拷贝构造是根据传入的对象进行赋值

为什么需要显式写析构函数？什么时候会出问题？

写显示析构函数可以防止在类是动态存储时候出现问题
如在动态存储时候其中内部通过new分配的空间默认析构函数不会挥手故需要写显示析构函数

*/