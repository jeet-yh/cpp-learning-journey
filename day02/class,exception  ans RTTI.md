# Day02

## 类

### 类中成员的返回对象问题

- 在类的成员函数中返回对象本身或对象的成员（包括值或引用）时应注意哪些问题？

- 返回对象本身
    ```cpp
    class Test{
    public:
        Test copy(){
            return *this;
        }
    };
    ```
    - 优点：
    - 不会暴露类内部状态
    - 返回的是一个新的对象(拷贝或移动)
    - 返回值对原对象没有任何影响
    - 缺点:
    - 效率低，每次调用都会拷贝或移动构造一次对象

- 返回对象本身的引用
    ```cpp
    class Test{
    public:
        Test& copy(int x){
            m_x = x;
            return *this;
        }
    private:
        int m_x;
    };
    ```
    - 优点：
    - 高效，无需拷贝
    - 可用于链式调用
    ```cpp
    class Test {
    public:
        Test& setAge(int in_age) {
            m_age = in_age;
            return *this;
        }
        Test& setName(const std::string& in_name) {
            m_name = in_name;
            return *this;
        }
        void show() {
            std::cout << m_age << ' ' << m_name << std::endl;
        }
    private:
        int m_age;
        std::string m_name;
    };
    int main() {
        Test tom;
        tom.setAge(50).setName("Tom");//链式调用
        tom.show();
    }
    ```

- 按值返回成员变量
    - 安全，但是效率低(会进行拷贝)
    ```cpp
    class Test{
    public:
        std::string get_name(){
            return m_name;
        }
        //如果执行std::string a = t.get_name();
        //会先将m_name赋值给一个临时变量然后在赋值给a
    private:
        std::string m_name;
    };
    ```

- 返回成员变量的引用
    - 效率高，但是可能会破坏封装性
    ```cpp
    std::string& get_name(){
        return m_name;
    }
    ```
    - 更优写法：
    - 返回const std::string&保证对象变量不会被改变
    - 函数体用const修饰，保证在函数内部成员变量不会被改变
    ```cpp
    const std::string& getName() const {
        return name;
    }
    ```
    - 这样效率高，且具有不错的封装性

- 返回临时对象的引用(严重错误，大忌)
    ```cpp
    std::string& get_name(){
        std::string name("123");
        return name;
    }
    int main(){
        std::string a = get_name();
	    std::cout << a;//不报错，但是出现严重错误
    }
    ```

- 重载operator=
    ```cpp
    Test& operator=(const Test&other){
        if(this==&other)return *this;
        //拷贝逻辑...
        return *this;
    }
    ```
    - 为什么要返回引用？
    - 考虑下面情况
    ```cpp
    a = b = c
    ```
    - 这里的 a = b = c 等价与 a = (b = c) 即会先调用b=c然后返回一个临时对象Test x(这里记为x),此时调用a = x。
    - b = c的结果赋给临时变量造成了不必要的拷贝。

## 继承

- 一个类继承另一个类的成员，实现代码复用和功能扩展

    ```cpp
    class Base {
    public:
        void show() {
            std::cout << "我是Base" << std::endl;
        }
    };
    class Test : public Base {//注意是单个:而不是::

    };
    int main() {
        Test t;
        t.show();
    }
    ```

- 继承的三种类型

    | 继承方式 | 基类public成员在子类中变成 | 基类protected成员在子类中变成 | 基类private成员在子类中变成 |
    |-----------|-----------------------------|----------------------------------|--------------------------|
    | public    | public                      | protected                        | 不可访问                |
    | protected | protected                   | protected                        | 不可访问                |
    | private   | private                     | private                          | 不可访问                |

    - 简单示例

    ```cpp
    class Base {
    public:
        int m_a = 1;
    protected:
        int m_b = 2;
    private:
        int m_c = 3;
    };
    class TestA: public Base {
    //m_a是public m_b是protected m_c不可访问
    };
    class TestB: protected Base {
    //m_a是protected m_b是protected m_c不可访问
    };
    class TestC: private Base {
    //m_a是private m_b是private m_c不可访问
    };
    class TestD: Base{
    //如果不写默认为private
    }
    ``` 

- 继承中的构造和析构
    - 构造函数的调用顺序是先调用基类再调用子类
    - 析构函数的调用顺序是先调用子类再调用基类(恰好相反)
    ```cpp
    class Base {
    public:
        Base() {
            std::cout << "Base构造函数" << std::endl;
        }
        ~Base() {
            std::cout << "Base析构函数" << std::endl;
        }
    };

    class Test: public Base {
    public:
        Test() {
            std::cout << "Test构造函数" << std::endl;
        }
        ~Test() {
            std::cout << "Test析构函数" << std::endl;
        }
    };

    int main() {
        Test t;
    }
    ```
- 多继承
    - 一个类可以继承多个类，即拥有多个基类的成员

    ```cpp
    class BaseA {
    };
    class BaseB {
    };
    class Test: public BaseA, protected BaseB {//还可以更多
    };
    ```
    - 初始化顺序
        - 先调用父类的构造函数，按照声明的顺序(上例是BaseA然后是BaseB)
        - 然后在调用子类自己的构造函数
        - 成员初始化依然是按声明顺序，出初始化表无关
        - 你可以在初始化表中调用基类的构造参数，若你不写则会自动调用基类的默认构造函数
        ```cpp
        class Test: public BaseA, protected BaseB {//还可以更多
            Test():BaseA(),BaseB(){
                
            }
        };
        ```
        - 如果基类没有默认构造函数则你必须显示地调用基类的构造函数
    - 如果派生类继承的多个基类中有相同名称的成员，可以通过使用作用域运算符::来决定调用那一个成员

- 继承中的多态性

    - 在类的派生过程中，基类的指针可以在不进行显示类型转化下(**隐式**)的情况下指向派生类的对象
    - 基类的引用也可以在不进行显示的转化下(**隐式**)引用派生类的对象
    - 但是基类的指针和引用都只能使用基类的成员，无法使用派生类的成员
    - 类的派生过程中，为派生类对象和基类对象的同名方法实现不同的操作的行为，即继承中的多态性
    - 可以通过函数的隐藏和虚方法实现多态

- 函数的隐藏
    - 派生类如果有与基类的同名函数，会导致派生类隐藏基类的该函数，即派生类对象无法直接调用基类的该函数。(可以在派生类函数中通过作用域运算符调用基类的隐藏函数)
    - 但是如果通过使用指针或者引用调用对象，那么程序将会根据指针或引用的类型，而不是对象，调用相应的函数
    ```cpp
    class Base {
    public:
        void show() {
            std::cout << "Base" << std::endl;
        }
    };
    class Test: public Base {
    public:
        void show() {
            std::cout << "Test" << std::endl;
        }
        void show_Base() {
		    Base::show();
	    }
    };
    int main() {
        Base bb;
        bb.show();//Base
        Test tt;
        tt.show();//Test
        tt.show_Base();//Base
        Base *a = new Base();
        a->show();//Base
        Base *b = new Test();
        b->show();//Base
        Test *c = new Test();
        c->show();//Test
        return 0;
    }
    ```
    - 重写和隐藏的区别

    |区别点	|函数隐藏|函数重写|
    |-------|-------|--------|
    |是否需要 virtual|	不需要|	必须是 virtual 函数|
    |发生条件|	派生类中出现同名函数（参数可以相同也可以不同）|	派生类中定义了与基类**完全相同**函数签名|
    |调用依据|	依据指针/引用的静态类型，即编译期绑定（静态联编）|	依据对象的实际类型，即运行期绑定（动态联编）|
    |是否支持多态|	❌ 否|	✅ 是|
    |调用基类同名函数|	需使用作用域运算符 Base::函数名()|	不需要，默认通过虚函数表调用|

    - 重写和隐藏的坑
    ```cpp
    class Base {
    public:
        virtual void show(int x) {
            std::cout << "Base" << ' ' << x << std::endl;
        }
    };

    class Test : public Base {
    public:
        void show(double x) {
            std::cout << "Test" << x << std::endl;
        }
    };

    int main() {
        Base* t = new Test();
        t->show(4);//调用的Base.show(4)，因为派生类函数签名与基类不完全相同
        return 0;
    }
    ```
- 虚函数
    - 上面通过指针或引用调用对象成员函数通过指针或引用的类型来调用的，想要实现通过以对象来调用成员函数则需要使用虚函数
    - 在基类中可以用virtual声明一个虚函数。
    - 然后在派生类中可以重写这个函数。(使用override可以判断这个函数是否被重写)
    - 虚函数不代表这个函数**不能被实现**。
    - 定义虚函数是为了允许基类的指针来调用派生类的这个函数。
    ```cpp
    class Base {
    public:
        virtual void show() {
            std::cout << "Base" << std::endl;
        }
    };
    class Test: public Base {//注意：这里不能是private继承
                            //否则Base的构造函数对于Test是不可访问的
                            //故无法通过Base *t = new Test();创建对象
    public:
        void show() override {//通过override表示这个函数被重载，如果这个函数没有重载某个基类函数会报错
                              //此举可以增加代码可读性
            std::cout << "Test" << std::endl;
        }
    };
    int main() {
        Base *a = new Base();
        a->show();//Base
        Base *b = new Test();
        b->show();//Test
        Test *c = new Test();
        c->show();//Test
        return 0;
    }
    ```

    - 定义一个函数为纯虚函数才代表这个函数没有被实现。
    - 有纯虚函数的类为抽象类，无法创建对象
    - 纯虚函数是为了实现一个接口，派生类必须实现这个函数，于此对应的普通虚函数派生类可以选择重写
    ```cpp
    class Base {
    public:
        virtual int a() = 0;//存虚函数
    };

    class Test: Base {
    public:
        int a() override {//不实现则报错
            return 1;
        }
    };

    int main() {
        Test t;
        t.show();
        return 0;
    }
    ```
- 静态联编和动态联编
    - 联编也被称作绑定
    - 程序在运行时具体使用哪一个可用函数和哪一段代码是由编译器决定的，编译器根据具体情况来选择和确定函数代码，这样的操作被称为联编
    - 静态联编：在编译时就可以确定调用哪一个函数，生成对应的机器码
    - 如：非虚函数、普通成员函数、重载函数、静态函数成员、非虚运算符重载

    ```cpp
    class Test{
    public:
        void show(){
            std::cout<<"Test"<<std::endl;
        }
    };
    int main(){
        Test t;
        t.show();//在编译时就知道会调用Test::show();
    }
    ```

    - 动态联编
    - 编译器无法确定最终调用哪一个函数，只知道函数是virtual的
    - 真正的函数地址会通过"虚函数表"(vtalbe)查表得到
    - 如：虚函数、通过指针和引用调用的虚函数
    ```cpp
    class Base {
    public:
        virtual void show() {
            std::cout << "Base" << std::endl;
        }
    };
    class Test: public Base {
    public:
        void show() override {
            std::cout << "Test" << std::endl;
        }
    };

    int main() {
        Base* p = new Test();
        p->show();//编译时不知道会调用Test::show，在运行时通过vtable决定
    }
    ```
- 虚函数表(vtalbe)
    - 每个包含虚函数的类都有一个虚函数表
    - 虚函数表储存了该类对象声明的虚函数的地址，系统会查找对象的虚函数列表，并在找到匹配的虚函数后执行
    - 关于是否虚函数会降低编码或执行效率，过于复杂，初学暂时不讨论
    - 由虚函数性质可知
        - 类的构造函数不能是虚函数，因为派生类的对象生成之前就需要调用基类的构造函数，不存在动态联编为题
        - 类的析构函数建议写成虚函数，这样可以根据指针所指向的对象来调用析构函数，而不是按照指针类型调用
        ```cpp
        class Base {
        public:
            ~Base() {};
        };
        class Test: public Base {
        public:
            Test(int size) {
                a = new int[size];
            }
            ~Test() {
                delete[] a;
            };
        private:
            int *a;
        };

        int main() {
            Base* t = new Test(50);//在t的生命周期结束后只会调用~Base();最终a的资源一直不被释放
                                   //正确做法使用把析构函数写成虚函数virtual ~Base();
        }
        ```
        - 友元函数不能声明为虚函数，因为友元函数不是类的成员
- Protected
    - protected访问权限对类外来说与private类似，对继承类来说与public相似
    ```cpp
    class Base {
    public:
        int m_x = 1;
    protected:
        int m_y = 2;
    private:
        int m_z = 3;
    };
    class Test: public Base {
    public:
        void show() {
            std::cout << m_x << std::endl;
            std::cout << m_y << std::endl;
            //std::cout << t.m_z << std::endl;//派生类无法访问基类private成员
        }
    };

    int main() {
        Test t;
        std::cout << t.m_x << std::endl;
        t.show();
        //std::cout << t.m_y << std::endl;//错误protected在类外类似private无法访问
        //std::cout << t.m_z << std::endl;//private在类外无法访问
    }
    ```
- 内部类
    - 即一个类被定义在另一个类内部(包含类)
    - 内部类对象的接口是不公开的，只在包含类的方法中使用内部类对象的方法
    ```cpp
    class out {
    public:
        void show() {
            std::cout << "out" << std::endl;
            m_t.show();
        }
        class in {
        public :
            void show() {
                std::cout << "in" << std::endl;
            }
        };
    private:
        in m_t;
    };
    int main() {
        out a;
        a.show();
    }
    ```
- 回顾多继承
    - 有4个类，D继承了B和C，而B、C又同时继承了A，出现了菱形结构，这种情况下A会在D中被重复继承两次，导致对象中有两个A类的副本，引起歧义和资源浪费。
    - 虚继承的引入：虚继承通过在派生类中引入一个指向基类的指针，而不是直接包含基类的副本，来解决这个问题。这样最终在D的示例对象中，A只会有一个
    ```cpp
    class A {
    public:
        int value;
        A() : value(0) {}
    };

    class B : public A {
    public:
        B() {
            value = 10;
        }
    };

    class C : public A {
    public:
        C() {
            value = 20;
        }
    };

    class D : public B, public C {
    public:
        D() {
            std::cout << value << std::endl; // 报错value的引用对象不明确
        }
    };
    ```
    - 使用虚继承解决问题
    ```cpp
    class A {
    public:
        int value;
        A() : value(0) {}
    };

    class B : virtual public A {
    public:
        B() {
            value = 10;
        }
    };

    class C : virtual public A {
    public:
        C() {
            value = 20;
        }
    };

    class D : public B, public C {
    public:
        D() {
            std::cout << value << std::endl; // 20
        }
    };
    int main() {
	D d;
	return 0;
    }
    ```
    - 可把派生类通过虚继承的类称作虚基类

    ```cpp
    class Base{};//虚基类
    class Test : virtual Base{};
    ```

    - 多重继承和虚基类
    - 虚基类的初始化需要特别注意，虚基类的构造函数必须又最底层的派生类直接调用，否则将会调用其默认构造参数，没有默认构造参数则编译出错
    - 中间类对虚基类的构造函数调用会被忽略
    ```cpp
    class Base {};
    class Mid : virtual public Base {};
    class Test : public Mid {
    public:
        Test(): Base() {}
    };
    ```
    - 普通多重继承不能隔代调用构造函数
    ```cpp
    class Base {};
    class Mid : public Base {};
    class Test : public Mid {
    public:
        Test(): Base() {}//报错
    };
    ```
- 模板类
    - 模板类是用来定义一组以类型参数化的类结构，适用于任意数据类型
    ```cpp
    template<typename T>//typename可以用class代替
    class Test{};//使用T作为占用符
    ```
    - 支持多个模板参数
    ```cpp
    template<typename T, typename U>
    class Test{};
    ```
    - 模板类的成员函数定义方式
    - 类内
    ```cpp
    template<typename T>
    class Test {
    public:
        Test(T data):m_data(data) {}
        void show() {
            std::cout << m_data << std::endl;
        }
    private:
        T m_data;
    };
    int main() {
        Test<std::string> t("123");
        t.show();
        Test<int>tt(1);
        tt.show();
        return 0;
    }
    ```
    - 类外
    ```cpp
    template<typename T>
    class Test {
    public:
        Test(T data);
        void show();
    private:
        T m_data;
    };
    template<typename T>
    Test<T>::Test(T data): m_data(data) {}
    template<typename T>
    void Test<T>::show() {
        std::cout << m_data << std::endl;
    }
    int main() {
        Test<std::string> t("123");
        t.show();
        Test<int>tt(1);
        tt.show();
        return 0;
    }
    ```
    - 模板类的特化
    - 如果你想对某种类型做特殊处理
    ```cpp
    template<typename T>
    class Test {
    public:
        void show() {
            std::cout << "标准" << std::endl;
        }
    };
    template<>
    class Test<int> {
    public:
        void show() {
            std::cout << "特殊" << std::endl;
        }
    };
    int main() {
        Test<std::string> a;
        a.show();
        Test<int>b;
        b.show();
        return 0;
    }
    ```
    - 模板定义中也可以使用非类型参数
    ```cpp
    template<typename T,int>
    ```
    - 模板定义也可以使用默认参数
    ```cpp
    template<typename T1,typename T2 = int>
    ```

## 异常
- 程序在运行过程中，例如出现除0，打开文件失败，数组越界等，这些错误无法使用普通流程处理时，就会"抛出异常"
    - 处理异常的基本语法try-throw-catch
    ```cpp
    try{
        if(something_wrong)
            throw "Error!";
    }catch (const std::string& error){
        std::cout<<error<<std::endl;
    }
    ```
    - try 包围需要检测是否出现异常的代码块
    - throw 出现异常时候会抛出异常对象
    - 捕获异常对象
    - 抛出异常的对象可以是任意类型，不过推荐抛出异常类对象
- 异常类
    - 常用异常类(头文件stdexcept)

    |异常类|说明|
    |-----|----|
    |std::exception|	所有异常的基类|
    |std::logic_error|	逻辑错误，如越界、非法参数|
    |std::runtime_error|	运行时错误，如IO失败等|
    |std::out_of_range|	越界访问|
    |std::invalid_argument|	参数非法|
    |std::overflow_error|	溢出错误|

    ```cpp
    void test(int x) {
    if (x == 0)
        throw std::invalid_argument("x不能是0！");
    }

    int main() {
        try {
            test(0);
        } catch (const std::invalid_argument& e) {
            std::cout << "捕获异常： " << e.what() << std::endl;
        }
    }
    ```

    -当然，你也可以自定义异常类
    ```cpp
    #include <exception>

    class MyException : public std::exception {//需要继承exception类
    public:
        const char* what() const noexcept override {
            return "我的异常类";
        }
    };
    throw MyException();//调用
    ```
    - c++11以上更加推荐使用noexcept来承诺不出现异常
    - 如果会出现异常就不写这个参数即可
    - noexcept有利于性能优化
- 异常的传播机制
    - 异常抛出后，如果当前没有函数处理，会一直向上传播，直到受理(catch)，如果一直没受理，程序会调用std::terminate()终止运行
- 多个catch处理机制
    ```cpp
    try{
        throw std::out_of_range("范围越界");
    }
    catch (std::runtime_error& e) {
    // 不匹配：out_of_range 是 runtime_error 的子类，但更具体的类应放前面
    }
    catch (std::out_of_range& e) {
        // 匹配成功：执行这里
    }
    ```
- try-throw-catch结构可以嵌套
    ```cpp
    try {
        try {
            throw std::runtime_error("出错了");
        } catch (...) {
            throw;  // 重新抛出异常
        }
    } catch (const std::exception& e) {
        std::cout << "外层捕获到异常: " << e.what() << std::endl;
    }
    ```

- 异常不会破坏对象的生命周期
    - 对象会在异常抛出后正常调用析构函数

## RTTI(运行阶段类型识别)

- 由于类的派生和继承，因此对象指针在程序中运行中指向的对象需要进行类型识别，才能做出正确的操作
- RTTI(运行阶段类型识别)的功能就是让程序在运行阶段动态监测对象的实际类型
    - typeip:获取对象信息(必须是有虚函数的类)

    ```cpp
    #include <typeinfo>
    #include<iostream>
    class Base {
        virtual void foo() {}
    };
    class Test : public Base {};
    int main() {
        Base* ptr = new Test();
        std::cout << typeid(*ptr).name() << std::endl; // 输出"4Test"（名称修饰后的）;
    }
    ```
    - 常用示例
    ```cpp
     if(typeid(*p)==typeid(Test))//判断p指向的对象类型是不是Test
     const std::type_info& ti = typeid(some_obj);
     std::cout << ti.name();       // 类型名称（编译器相关，有可能是编码的）
    ```

    - type_info类：存储类型信息（需包含typeinfo头文件）//更多明天补充
        - 当你使用typeip的时候其实返回的就是一个type_info类

    - dynamic_cast运算符:安全向下转型(必须是有虚函数的类)
    - 指针：不成功返回nullptr，成功则转化为你指定的类型
    - 引用：不成功抛出std::bad_cast异常，成功则转化为你指定的类型
    ```cpp
    class Base {
    public:
        virtual ~Base() {}
    };

    class Test : public Base {
    public:
        void print() {
            std::cout << "转化成功\n";
        }
    };

    void test(Base* b) {
        Test* d = dynamic_cast<Test*>(b);
        if (d) {
            d->print();
        } else {
            std::cout << "转化失败\n";
        }
    }

    int main() {
        Base *b = new Test();//成功
        Base *bb = new Base();//失败
        test(b);
        test(bb);
    }
    ```
    - static_cast运算符：仅当两个类别可以隐式转化时才可以使用，通常用于低风险转换(不依赖RTTI)
    ```cpp
    double d = 3.14;
    int i = static_cast<int>(d); // 正确，丢失小数部分

    Base* b = new Test();
    Test* d2 = static_cast<Test*>(b); // 编译通过，需程序员确保b确实指向Test
    ```

    - const_cast运算符：去除或添加const、volatile限定符的类型转换符
    ```cpp
    const_cast<new_type>(expression)
    ```
    - 下例会出现未定义行为，因为你试图改变只读内存，危险！
    ```cpp
    void modify(int* p) {
        *p = 100;
    }

    int main() {
        const int a = 10;
        modify(const_cast<int*>(&a)); // ⛔ 未定义行为
    }
    ```
    - 正确使用场景
    - a是可写的，实际也只写了a的内存
    ```cpp
    void modify(int *p) {
	*p = 100;
    }

    int main() {
        int a = 10;
        const int* p = &a;
        modify(const_cast<int*>(p));
    }
    ```
    - reinterpret_cast：进行底层强行转换(如指针变证书，函数指针等等)
    - 最危险的转化方式，慎用！
    - 绝对不要用在日常对象之间的类型转换！

    ```cpp
    int a = 65;
    char* p = reinterpret_cast<char*>(&a); // 将 int* 转换成 char*
    void (*func)();
    int* iptr = reinterpret_cast<int*>(func); // 将函数指针转换为整型指针
    ```