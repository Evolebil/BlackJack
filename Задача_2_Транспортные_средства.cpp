// ============================================================
// ЗАДАЧА 2: Наследование — «Транспортные средства»
// Цель: понять public-наследование, конструкторы наследников,
//       доступ к полям базового класса
// ============================================================
#include <iostream>
#include <string>
using namespace std;

// ============================================================
// БАЗОВЫЙ КЛАСС: любое транспортное средство
// ============================================================
class Vehicle {
protected:
    int speed;   // км/ч
    int weight;  // кг

public:
    Vehicle(int s, int w) : speed(s), weight(w) {}

    virtual void display() const {
        cout << "Транспорт: скорость " << speed
             << " км/ч, вес " << weight << " кг\n";
    }

    virtual ~Vehicle() = default;
};

// ============================================================
// АВТОМОБИЛЬ: расширяет Vehicle маркой и числом дверей
// ============================================================
class Car : public Vehicle {
private:
    string brand;
    int doors;

public:
    Car(int s, int w, string b, int d)
        : Vehicle(s, w), brand(b), doors(d) {}

    void display() const override {
        cout << "Автомобиль: марка " << brand
             << ", дверей " << doors << " — ";
        Vehicle::display();
    }
};

// ============================================================
// ВЕЛОСИПЕД: расширяет Vehicle типом
// ============================================================
class Bicycle : public Vehicle {
private:
    string type;  // например: "горный", "шоссейный"

public:
    Bicycle(int s, int w, string t)
        : Vehicle(s, w), type(t) {}

    void display() const override {
        cout << "Велосипед: тип \"" << type << "\" — ";
        Vehicle::display();
    }
};

int main() {
    Vehicle plain(15, 8);
    Car     car(180, 1500, "Toyota", 4);
    Bicycle bike(25, 12, "горный");

    plain.display();
    car.display();
    bike.display();

    return 0;
}
