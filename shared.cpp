#include <iostream>
#include <queue>
#include <memory>

struct Person {

    Person(const std::string& name, int age) : name_(name), age_(age) {}

    std::string getName() const {
        return name_;
    }

    int getAge() const {
        return age_;
    }

    std::string name_;
    int age_;
};

int main() {
    // 创建一个存储 Person 对象的队列
    std::queue<std::shared_ptr<Person>> personQueue;
    // 创建几个 Person 对象，并将其添加到队列中
    std::shared_ptr<Person> person1 = std::make_shared<Person>("Alice", 25);
    std::shared_ptr<Person> person2 = std::make_shared<Person>("Bob", 30);
    std::shared_ptr<Person> person3 = std::make_shared<Person>("Charlie", 35);
    personQueue.push(person1);
    personQueue.push(person2);
    personQueue.push(person3);

    // 从队列中取出并打印 Person 对象的信息
    while (!personQueue.empty()) {
        std::shared_ptr<Person> frontPerson = personQueue.front();
        std::cout << "Name: " << frontPerson->getName() << ", Age: " << frontPerson->getAge() << std::endl;
        personQueue.pop();
    }
    return 0;
}
