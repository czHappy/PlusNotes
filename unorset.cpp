#include <iostream>
#include <unordered_set>
#include <string>

class Person {
public:
    Person(const std::string& name, int age) : name_(name), age_(age) {}

    const std::string& getName() const {
        return name_;
    }

    int getAge() const {
        return age_;
    }

private:
    std::string name_;
    int age_;
};
struct PersonHash {
    std::size_t operator()(const Person& person) const {
        // 自定义哈希逻辑，可以根据需要选择适合的哈希算法
        // 这里简单地将姓名哈希和年龄相加作为哈希值
        std::size_t nameHash = std::hash<std::string>{}(person.getName());
        std::size_t ageHash = std::hash<int>{}(person.getAge());
        return nameHash + ageHash;
    }
};

struct PersonEqual {
    bool operator()(const Person& lhs, const Person& rhs) const {
        // 自定义相等比较逻辑，比较姓名和年龄是否相等
        return lhs.getName() == rhs.getName() && lhs.getAge() == rhs.getAge();
    }
};
bool operator==(const Person &A, const Person &B) {
    return (A.getAge() == B.getAge());
}
class mycmp {
public:
    bool operator()(const Person &A, const Person &B) const {
        return (A.getName() == B.getName()) && (A.getAge() == B.getAge());
    }
};
int main() {
    // std::unordered_set<Person, PersonHash> personSet;
    // std::unordered_set<Person, PersonHash, PersonEqual> personSet;
    std::unordered_set<Person, PersonHash, mycmp> personSet;
    // 创建并插入 Person 对象
    Person p1("Alice", 25);
    Person p2("Bob", 30);
    Person p3("Alice", 25);

    personSet.insert(p1);
    personSet.insert(p2);
    personSet.insert(p3);

    // 输出集合中的 Person 对象
    for (const auto& person : personSet) {
        std::cout << "Name: " << person.getName() << ", Age: " << person.getAge() << std::endl;
    }

    return 0;
}
