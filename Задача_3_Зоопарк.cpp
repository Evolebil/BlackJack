// ============================================================
// ЗАДАЧА 3: Полиморфизм — «Зоопарк»
// Цель: освоить виртуальные функции, указатели на базовый
//       класс, динамическую диспетчеризацию
// ============================================================
#include <iostream>
#include <vector>
#include <memory>
using namespace std;

// ============================================================
// АБСТРАКТНЫЙ БАЗОВЫЙ КЛАСС: любое животное
// ============================================================
class Animal {
public:
    virtual void speak() const = 0;  // чисто виртуальная функция
    virtual ~Animal() = default;
};

class Dog : public Animal {
public:
    void speak() const override { cout << "Гав!\n"; }
};

class Cat : public Animal {
public:
    void speak() const override { cout << "Мяу!\n"; }
};

class Bird : public Animal {
public:
    void speak() const override { cout << "Чирик!\n"; }
};

int main() {
    vector<unique_ptr<Animal>> zoo;
    zoo.push_back(make_unique<Dog>());
    zoo.push_back(make_unique<Cat>());
    zoo.push_back(make_unique<Bird>());

    for (const auto& animal : zoo)
        animal->speak();

    return 0;
}
