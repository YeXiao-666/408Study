#include <iostream>
#include <Windows.h>
#include <string>
#include <string.h>

using namespace std;

// 定义一个“人类”
class Human {
public:
    const static int max = 10;
    static int count;
    static int getCount(Human& h)
    {
        count = 10;
        h.name = "sadada";
    }
    void PrintHuman(Human& h) const
    {
        cout << h.name;
    }
    Human();
    Human(int age, int salary);
    Human(const Human&);	//不定义拷贝构造函数，编译器会生成“合成的拷贝构造函数”
    ~Human();
    void eat();
    void sleep();
    void play();
    void work();

    string getName();
    int getAge() const;
    int getSalary();

    void setAddr(const char* newAddr);
    const char* getAddr();

    const Human* compare1(const Human*);
    const Human& compare1(const Human&);

private:
    string name = "Unknown";
    int age = 28;
    int salary;
    char* addr;
};

Human::Human() {
    name = "无名氏";
    age = 18;
    salary = 30000;
    this->addr = NULL;
}

Human::Human(int age, int salary) {
    cout << "调用自定义的构造函数" << endl;
    this->age = age;	//this 是一个特殊的指针，指向这个对象本身 
    this->salary = salary;
    name = "无名";
    addr = new char[64]; strcpy_s(addr, 64, "China");
}

Human::Human(const Human& human) {
    cout << "调用自定义的拷贝构造函数" << endl;
    this->age = human.age;	//this 是一个特殊的指针，指向这个对象本身 
    this->salary = human.salary;
    name = "无名";
    this->addr = new char[64];
    strcpy_s(this->addr, 64, human.addr);
}
Human::~Human()
{

    if (this->addr != NULL)
    {
        delete[] addr;

        this->addr = NULL;
        cout << "调用析构函数" << endl;
    }
}

const Human* Human::compare1(const Human* other) {
    if (!other)
    {
        cout << "NullReferenceException!" << endl;
        return NULL;
    }

    if (age > other->age) {
        return	this; //没有创建新的对象
    }
    else {
        return other;
    }
}
const Human& Human::compare1(const Human& other) {

    if (age > other.age) {
        return	*this; //没有创建新的对象
    }
    else {
        return other;
    }
}

void Human::eat() {
    cout << "吃炸鸡，喝啤酒！" << endl;
}

void Human::sleep() {
    cout << "我正在睡觉!" << endl;
}

void Human::play() {
    cout << "我在唱歌! " << endl;
}

void Human::work() {
    cout << "我在工作..." << endl;
}

string Human::getName() {
    return name;
}

int Human::getAge() const {
    return age;
}

int Human::getSalary() {
    return salary;
}

void Human::setAddr(const char* newAddr) {
    if (!newAddr) {
        return;
    }
    strcpy_s(addr, 64, newAddr);
}

const char* Human::getAddr() {
    return addr;
}
//class Person
//{
//public:
//    Person(string name, int age)
//    {
//        this->m_Age = age;
//        this->m_Name = name;
//    }
//    int m_Age;
//    string m_Name;
//    bool operator<(const Person& p1)const
//    {
//        return p1.m_Age < this->m_Age;
//    }
//};
//
//class ComparePerson
//{
//public:
//    bool operator()(const Person& p1, const Person& p2)const
//    {
//        return p1.m_Age > p2.m_Age;
//    }
//};
/*set<Person, ComparePerson>s1;
    Person p1("关羽", 56 );
    Person p2( "张飞",  55  );
    Person p3( "刘备", 68 );
    Person p4("赵云", 75 );

    s1.insert(p1);
    s1.insert(p2);
    s1.insert(p3);
    s1.insert(p4);*/

    /*for (set<Person, ComparePerson>::iterator it = s1.begin(); it != s1.end(); it++)
    {
        cout << it->m_Name << it->m_Age << endl;
    }*/
    //int main(void) {
    //    //Human	h1(25, 35000);	// 使用自定义的默认构造函数 
    //    //Human	h2(h1);	// 使用自定义的拷贝构造函数
    //    //Human	h3;
    //    //cout << "h1 addr:" << h1.getAddr() << endl;
    //    //cout << "h2 addr:" << h2.getAddr() << endl;
    //    //h1.setAddr("长沙");
    //    //cout << "h1 addr:" << h1.getAddr() << endl;
    //    //cout << "h2 addr:" << h2.getAddr() << endl;
    //
    //    //Human h1(25, 30000);
    //    //Human h2(18, 8000);
    //
    //    //cout << h1.compare1(&h2) << endl;
    //    //cout << &h1 << endl;
    //    //cout << &(h1.compare1(h2)) << endl;
    //
    //
    //
    //    return 0;
    //}
#include <vector>
using namespace std;
int main7()
{
    int n, m;
    cout << Human::max;
    int a, b, c, d;
    cin >> n >> m;
    vector<int> arr1(n);
    vector<int> arr2(m);
    for (int i = 0; i < n; ++i)cin >> arr1[i];
    for (int i = 0; i < m; ++i)cin >> arr2[i];
    cin >> a >> b;
    cin >> c >> d;
    vector<int> arr(m + n);
    int j = 0;
    while (a <= b)
    {
        int index = a - 1;
        arr[j++] = arr1[index];
        ++a;
    }
    while (c <= d)
    {
        int index = c - 1;
        arr[j++] = arr1[index];
        ++c;
    }

    cout << arr[j >> 1];
    cout << arr[j >> 1];
    return 0;
}