#include <iostream>
#include <unordered_set>
#include <string>

class Person {
public:
    Person(const std::string& name, double score) : name_(name), score_(score) {}

    const std::string& getName() const {
        return name_;
    }

    double getScore() const {
        return score_;
    }

private:
    std::string name_;
    double score_;
};

struct PersonHash {
    std::size_t operator()(const Person& person) const {
        std::size_t nameHash = std::hash<std::string>{}(person.getName());
        std::size_t scoreHash = std::hash<double>{}(person.getScore());

        // 结合两个哈希值进行混合
        // 可以使用任意的混合算法，例如位运算、异或运算等
        return nameHash ^ (scoreHash << 1);
    }
};

int main() {
    std::unordered_set<Person, PersonHash> personSet;

    // 创建并插入 Person 对象
    Person p1("Alice", 95.5);
    Person p2("Bob", 87.2);
    Person p3("Alice", 95.5);

    personSet.insert(p1);
    personSet.insert(p2);
    personSet.insert(p3);

    // 输出集合中的 Person 对象
    for (const auto& person : personSet) {
        std::cout << "Name: " << person.getName() << ", Score: " << person.getScore() << std::endl;
    }

    return 0;
}
