// ============================================================
// ЗАДАЧА 1: Класс и объекты — «Библиотечная книга»
// Цель: освоить классы, конструкторы, инкапсуляцию, объекты
// ============================================================
#include <iostream>
#include <string>
using namespace std;

// ============================================================
// КЛАСС Book: описывает книгу с учётом прочитанных страниц
// ============================================================
class Book {
private:
    string title;
    string author;
    int year;
    int pages;
    int pagesRead = 0;  // по умолчанию 0 — ещё не читали

public:
    // Конструктор с параметрами (pagesRead не передаётся — всегда 0)
    Book(string newAuthor, string newTitle, int newYear, int newPages)
        : author(newAuthor), title(newTitle), year(newYear), pages(newPages) {}

    // Вывод информации о книге
    void printInfo() const {
        cout << "Название: " << title  << "\n";
        cout << "Автор:    " << author << "\n";
        cout << "Год:      " << year   << "\n";
    }

    // Увеличиваем счётчик прочитанных страниц, но не больше общего числа
    void read(int n) {
        pagesRead = min(pagesRead + n, pages);
        cout << "Прочитано " << pagesRead << " из " << pages << " страниц.\n";
    }

    // Геттеры
    int getPages()     const { return pages; }
    int getPagesRead() const { return pagesRead; }
};

int main() {
    Book warAndPeace("Лев Толстой", "Война и Мир", 1869, 1400);
    Book masterAndMargarita("Михаил Булгаков", "Мастер и Маргарита", 1967, 480);
    Book crimeAndPunishment("Фёдор Достоевский", "Преступление и Наказание", 1866, 608);

    warAndPeace.printInfo();
    warAndPeace.read(200);
    warAndPeace.read(1300);  // не превысит 1400 страниц
    cout << "\n";

    masterAndMargarita.printInfo();
    masterAndMargarita.read(480);  // прочитали всё
    cout << "\n";

    crimeAndPunishment.printInfo();
    crimeAndPunishment.read(100);

    return 0;
}
