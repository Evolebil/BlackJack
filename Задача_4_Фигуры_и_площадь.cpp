// ============================================================
// ЗАДАЧА 4: Комбинированная — «Фигуры и площадь»
// Цель: объединить классы, наследование и полиморфизм
//       в одном мини-проекте
// ============================================================
#include <iostream>
#include <vector>
#include <memory>
#include <iomanip>
#include <cmath>
using namespace std;

// ============================================================
// АБСТРАКТНЫЙ БАЗОВЫЙ КЛАСС: любая геометрическая фигура
// ============================================================
class Shape {
public:
    virtual double area()      const = 0;
    virtual void   printType() const = 0;
    virtual ~Shape() = default;
};

// ============================================================
// КРУГ
// ============================================================
class Circle : public Shape {
private:
    double radius;

public:
    Circle(double r) : radius(r) {}

    double area() const override {
        return M_PI * radius * radius;
    }

    void printType() const override {
        cout << "Круг";
    }
};

// ============================================================
// ПРЯМОУГОЛЬНИК
// ============================================================
class Rectangle : public Shape {
private:
    double width;
    double height;

public:
    Rectangle(double w, double h) : width(w), height(h) {}

    double area() const override {
        return width * height;
    }

    void printType() const override {
        cout << "Прямоугольник";
    }
};

int main() {
    vector<unique_ptr<Shape>> shapes;
    shapes.push_back(make_unique<Circle>(5.0));
    shapes.push_back(make_unique<Rectangle>(4.0, 6.0));
    shapes.push_back(make_unique<Circle>(3.0));

    for (const auto& s : shapes) {
        s->printType();
        cout << " — площадь: "
             << fixed << setprecision(2) << s->area() << "\n";
    }

    return 0;
}
