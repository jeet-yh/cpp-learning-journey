# Day01

## 类

### 构造函数与拷贝构造

```c++
class Student {
public:
	// 默认构造函数
	Student() {}
	// 构造函数
	Student(std::string name, int age) : m_name(name), m_age(age) {}
	//拷贝构造函数
	Student(Student& other) {
		m_name = other.m_name;
		m_age = other.m_age;
	}
	//析构函数
	~Student() {}

private:
	std::string m_name;
	int m_age;
};
```

### 友元函数

- c++提供友元的方式，实现从类外部访问类内部的数据形式，通过加friend关键字实现

- 为什么需要友元?
如果你要实现下面例子，即在类外访问类内的protected或private成员的时候就需要友元

    ```cpp
    Student tom("Tom",18);
    std::cout<<tom.m_name;//报错，因为是private无法访问
    ```

    ```cpp
    class Student {
        friend void print(const Student& s); // 声明
    private:
        std::string m_name = "Tom";
    };

    void print(const Student& s) {//注意要在类外部实现
        std::cout << s.name << std::endl; // ✅ 可以访问私有成员
    }
    ```
    
- 友元可以修饰三种类型
    - 友元函数
    如上面例子就是一个友元函数的例子


    - 友元类
    一个类成为另一个类的“朋友”，可以访问对方私有成员。
    ```cpp
    class Student {
	friend class Teacher; // Teacher 是 Student 的朋友
    private:
        int age = 18;
    };

    class Teacher {
    public:
        void access(Student& s) {
            std::cout << s.age << std::endl; // ✅ 访问成功
        }
    };

    ```


    - 友元成员函数
    把另一个类的某个成员函数声明为当前类的朋友。
    ```cpp
    class Teacher {
    public:
        void accessStudent(Student& s);
    };

    class Student {
        friend void Teacher::accessStudent(Student&); // 👈 仅这个函数是朋友
    private:
        int score = 95;
    };

    void Teacher::accessStudent(Student& s) {
        std::cout << s.score << std::endl; // ✅ 访问成功
    }
    ```
    - 注意：友元是单向的
    ```cpp
    class A {
	friend class B;  // B 是 A 的朋友
    private:
        int a_data = 10;
    };

    class B {
    public:
        void accessA(A& obj) {//B可以访问A 但是A不能访问B
            std::cout << "Access A: " << obj.a_data << std::endl; // ✅ 合法
        }

    private:
        int b_data = 20;
    };
    ```

### 运算符重载

- 运算符重载和函数重载类似，其本质上是根据运算符操作数实现的运算符重载。重载运算符需要使用关键字operator加运算符实现。

    ```cpp
    class Operator_Test{
        Operator_Test operator+ (const Operator_Test& other)const;
    };
    ```

    - 运算符重载通常可以使用成员函数和友元函数

    ```cpp
    class Operator_Test{
        Operator_Test operator+ (const Operator_Test& other)const{

        }
    }; 
    class Operator_Test {
	friend Operator_Test operator+ (const Operator_Test& other);
    };
    Operator_Test operator+ (const Operator_Test& other) {

    }
    ```

    | 类别   | 可重载运算符          |
    | ---- | ------------------------ |
    | 算术运算 | + - \* / %               |
    | 比较运算 | ==  !=   >  <   >=  <=   |
    | 逻辑运算 | ! && \|\|                |
    | 赋值运算 | =                        |
    | 自增减  | ++ --（前缀/后缀）         |
    | 下标运算 | \[]                      |
    | 函数调用 | ()                       |
    | 指针访问 | ->                       |
    | 输入输出 | << >>（通常为非成员函数 + friend） |


- 输入输出运算符重载
    - 要让输入输出符重载，这必须是非成员函数，此时最好配合友元

    ```cpp
    class Person {
    public:
        Person(std::string n, int a) : name(n), age(a) {}

        friend std::ostream& operator<<(std::ostream& os, const Person& p);

    private:
        std::string name;
        int age;
    };  

    std::ostream& operator<<(std::ostream& os, const Person& p) {
        os << "Name: " << p.name << ", Age: " << p.age;
        return os;
    }
    int main() {
        Person a("123", 18);
        std::cout << a;
    }
    ```
- 重载前置++和后置++(--也一样，不一一举出)

    ```cpp
    class Counter {
    int value;
    public:
    Counter(int v = 0) : value(v) {}
    // 前置 ++
    Counter& operator++() {
        ++value;
        return *this;//返回修改后的
    }
    // 后置 ++（注意 int 参数用于区分）
    Counter operator++(int) {
        Counter temp = *this;
        ++value;
        return temp;//返回修改前的
        }
    };

    ```

- 不能重载的运算符有:: . sizeof typeof

### 类的类型转换

- 类的类型转化有两种
- 1.构造参数实现类型转化(其他类型到当前类)

    ```cpp
    class Test {
    public:
        Test(int x, int y): m_x(x), m_y(y) {}
        void show() {
            std::cout << m_x << ' ' << m_y << std::endl;
        }
    private:
        int m_x;
        int m_y;
    };
    int main() {
        Test t = {1, 2};
        t.show();
    }
    ```
    - 如果你想控制这种隐式转化不发生可以使用explicit修饰构造参数
    - explict会禁止隐式转化，只能够显示转换
    ```cpp
    explicit Test(int x, int y): m_x(x), m_y(y) {}
    ```
- 2.类型转换函数(当前类转化为其他类型)

    ```cpp
    class Test {
    public:
        Test(int x) : x(x) {}

        // 类型转换函数：MyClass => int
        operator int() const {//最好是const不会改变对象状态
            return x;
        }

    private:
        int m_x;
    };

    int main() {
        MyClass a(10);
        int b = a;      // 隐式调用 operator int()
        std::cout << b << std::endl;  // 输出：10
    }
    ```

    ### 类中的静态数据成员和函数
    
    - 类中的静态数据成员
        - 类的数据成员可以通过static关键字来定义为静态数据类型。
        - 类的静态成员不是类的对象的一部分，它是单独存储的，即所有类的对象共享同一个静态成员
        - 静态成员的初始化需要在类外定义，不能在函数中定义
    ```cpp
    class Test {
    public:
        static int s_val;//这里只是声明，没有定义
    };
    int Test::s_val = 0;
    int main() {
        //int Test::s_val = 0;错误
        std::cout << Test::s_val;
        return 0;
    }
    ```
    - 类中的静态函数
        - 与静态成员类似，是整个类共享的
        - 由于其储存的关系，在调用静态函数的时候不能调用类对象中的普通数据成员
    ```cpp
    #include<iostream>
    class Test {
    public:
        Test(int x): m_x(x) {}
        static void print() {
            std::cout << "123" << std::endl;
        //	std::cout << m_x << std::endl;错误
        }
    private:
        int m_x;
    };
    int main() {
        Test a(5);
        a.print();
        Test::print();
        return 0;
    }
    ```
### 类中的动态存储
- 类中的动态存储形式，指的是类的某些成员（特别是数组或对象）在运行时动态申请内存。
- 特点
    - 内存大小可以在运行时决定(灵活)
    - 不会随着对象生命周期释放，需要手动释放
    - 用new分别,delete释放
    ```cpp
    class Test {
    public:
        Test(int size): m_size(size) {
            m_p = new int[size];
        }
        ~Test() {
            delete[] m_p;
        }
    private:
        int m_size;
        int *m_p;
    };
    ```
- 浅拷贝和深拷贝
    - 浅拷贝
    ```cpp
    Test a(50);
    Test b=a;//浅拷贝
    ```
    - 上面示例中是浅拷贝，b其实是和a共享一块内存，并且在析构的时候这一块内存会被释放两次，引发严重错误

    - 深拷贝
    - 深拷贝解决了上面问题，需要自定义拷贝函数或赋值运算符
    ```cpp
    class Test {
    public:
        Test(int size): m_size(size) {
            m_p = new int[size];
        }
        Test(const Test& other) {//const防止other被修改
            m_size = other.m_size;
            m_p = new int[other.m_size];
            //此处可以加上拷贝数据的语句
        }
        Test& operator= (const Test& other) {//返回类型为Test&避免不必要拷贝
            if (this == &other) return *this; // 自我赋值保护
            delete[] m_p;//先释放
            m_size = other.m_size;
            m_p = new int[other.m_size];
            //此处可以加上拷贝数据的语句
            return *this;
        }
        ~Test() {
            delete[] m_p;
        }
    private:
        int m_size;
        int *m_p;
    };
    ```
### C++会替你默认生成的6个成员函数

- 构造函数
    ```cpp
    ClassName();
    ```
    - 在创建对象的时候，如果你没有传递任何参数，就会调用默认构造函数(实际上是一个空实现)，成员变量用默认构造或保持未初始化(基本类型如 int 不会自动初始化)。
    ```cpp
    class Test {
    public:
        void show() {
            std::cout << x << std::endl;
        }
    private:
        int x;//x值可能是任意值
        // int x=5;一定是5
    };
    ```

- 析构函数
    ```cpp
    ~ClassName();
    ```
    - 对象生命周期结束时，自动调用。默认析构函数会逐个调用成员对象的析构函数。

- 拷贝构造函数
    ```cpp
    ClassName(const ClassName& other);
    ```
    - 当你使用对象初始化另一个对象时候，会调用它。实际上是浅拷贝
    - 注意：若定义了拷贝函数，编译器就不会自动生成默认移动构造函数。
    ```cpp
    class Test {
    public:
        void show() {
            std::cout << x << std::endl;
        }
    private:
        int x;
    };
    int main() {//b c都调用了默认构造函数
        Test a;
        a.show();
        Test b = a;//注意和下面拷贝赋值运算符区分！易错
        b.show();
        Test c(a);
        c.show();
        return 0;
    }
    ```

- 拷贝赋值运算符
    ```cpp
    ClassName& operator=(const ClassName& other);
    ```
    - 对象已经存在时，进行赋值调用拷贝赋值运算符。注意：一定是已经存在时，在创建对象的时候调用的是拷贝构造函数
    ```cpp
    class Test {
    public:
        void show() {
            std::cout << x << std::endl;
        }
    private:
        int x;
    };
    int main() {
        Test a;
        a.show();
        Test b = a;//拷贝构造函数
        b.show();
        Test c;
        c = a;//拷贝赋值运算符
        c.show();
        return 0;
    }
    ```
- 在移动构造函数、移动赋值运算符之前，我们先来简单聊聊左值和右值
    - 左值
    - 可以取地址、有名字的对象
    - 表达式结束后依然在
    ```cpp
    int x=10;//x是左值
    int *p=x;//可以取地址
    ```
    - 右值
    - 临时对象、没有名字、不能取地址
    - 表达式结束后就被销毁
    ```cpp
    int y=x+5;//(x+5)就为右值
    int z=5;//5是右值常量
    ```
    - 为什么要有左值、右值和移动语义？
    ```cpp
    std::string a = "hello";
    std::string b = a;//拷贝构造
    std::string c = std::move(a); // 不再调用拷贝构造，而是移动构造！
    ```
    - 没有移动语义，b=a会拷贝，效率低
    - 使用移动语义，可以让c偷走a的内存，不需要拷贝，a的内存被偷走后就没有资源了
    ```cpp
    std::cout << a << ' ' << b << ' ' << c << std::endl;//输出" hello hello"
    ```
    - 右值引用：它只绑定右值
    ```cpp
    int&& r = 10;
    ```
- 移动构造函数
    ```cpp
    class Test {
    public:
        Test(Test&& other) noexcept {
            this->p = other.p;
            other.p = nullptr; // 原对象不再拥有资源
        }
    };
    ```
- 移动赋值运算符
    ```cpp
    Test& operator=(Test&& other) noexcept {
        if (this != &other) {
            delete[] p;
            p = other.p;
            other.p = nullptr;
        }
        return *this;
    }
    ```
- std::move和std::forward
    - std::move(x)：将x强制转化为右值引用，它只允许你移动它的资源，并不移动它本身
    ```cpp
    std::string s1 = "hello";
    std::string s2 = std::move(s1); // s1 的资源移动到 s2，s1变成空壳
    ```
    - std::forward(x)
    ```cpp
    void test(int&& a){}
    int main(){
        int a=5;
        test(a);//报错，只能传入右值
        test(5);
    }
    ```
    - 转发引用
    ```cpp
    template<typename T>
    void test(T&& a) {}
    int main() {
        int a = 5;
        test(a);
        test(5);
        //均不报错，当你在模板函数中写 T&&，这个 && 并不一定是右值引用！
        //传入&则是左值引用，&&则是右值引用！
    }
    ```
    - 这个机制叫做折叠引用，是左值引用还是右值引用，取决于你传入的值
    - 而std::forward(x)它能智能判断T值的类别，完美保持左/右值属性
    ```cpp
    void print(int& x)       { std::cout << "左值引用\n"; }
    void print(int&& x)      { std::cout << "右值引用\n"; }

    template<typename T>
    void test(T&& val) {
        print(std::forward<T>(val));  // 完美转发
    }

    int main() {
        int a = 10;
        test(a);        // 推导为 T = int&，val 是 int&
        test(20);       // 推导为 T = int，val 是 int&&
    }
    ```

- 如果你希望你的对象只能被移动，你可以
    ```cpp
    Test(const Test&) = delete;
    Test& operator=(const Test&) = delete;
    ```
