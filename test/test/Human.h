#pragma once

class Human {
public:
    Human();
    Human(int age, int salary);

    int getAge() const;
    const Human* compare1(const Human*);
    const Human& compare2(const Human&);
private:
    int age = 28;
    int salary;
    char* addr;
};