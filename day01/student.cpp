#include <iostream>
#include <string>
using namespace std;

class Student {
public:
    Student() {
        cout << "Default constructor\n";
    }

    Student(const string& name, int age) : name_(name), age_(age) {
        cout << "Parameterized constructor\n";
    }

    Student(const Student& other) {
        name_ = other.name_;
        age_ = other.age_;
        cout << "Copy constructor\n";
    }

    Student& operator=(const Student& other) {
        if (this != &other) {
            name_ = other.name_;
            age_ = other.age_;
            cout << "Copy assignment\n";
        }
        return *this;
    }

    ~Student() {
        cout << "Destructor\n";
    }

private:
    string name_;
    int age_;
};

int main() {
    Student s1("Tom", 20);
    Student s2 = s1;
    Student s3;
    s3 = s1;
    return 0;
}