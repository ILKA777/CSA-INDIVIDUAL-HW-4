#include <iostream>
#include <stdio.h>
#include <string>
#include <omp.h>
#include <chrono>

#include <parallel/algorithm>

using namespace std;

// Класс книга
// из экземпляров книги будет создаваться массив.
class Book {
public:
    string name;
    int line;
    int closet;
    int number_in_closet;

    Book() {
        name = "null";
        line = 0;
        closet = 0;
        number_in_closet = 0;
    }

    Book(string Name, int Line, int Closet, int Number_in_closet) {
        name = Name;
        line = Line;
        closet = Closet;
        number_in_closet = Number_in_closet;
    }


};

// Метод для создания основного каталога из всех книг
// просто сортирует по названию, для наглядности использовал сортировку пузырьком.
// В теории можно было бы распаралелить сортировку с помощью pragma omp for,
// но в даанном случае я поступил иначе.
Book sortCatalog(Book books[], int n) {
    Book temp;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (books[j].name > books[j + 1].name) {
                temp = books[j];
                books[j] = books[j + 1];
                books[j + 1] = temp;
            }
        }
    }
    return *books;
}

// Метод для создания каталога по ряду
// берем книги нужного нам ряда и формируем из них новый каталог
// тут я поступил более хитро так как вычленяю книги из основного каталога
// где они уже отсортированы
Book createCatalogForLine(Book books[], Book books2[], int n, int line_number) {

    int k = 0;

    for (int i = 0; i < n; ++i) {
        if (books[i].line == line_number) {
            books2[k] = books[i];
            k++;
        }
    }
    return *books2;
}


int main() {
    int n;
    int line_number;
    cout << "Введите кол-во книг для составления каталога: ";
    cin >> n;
    cout << "Введите номер ряда для создания студентом каталога: ";
    cin >> line_number;

    // Будущий основной каталог.
    Book books[n];
    // Будущий каталог по ряду.
    Book books2[n];

    string name;
    int line;
    int closet;
    int number_in_closet;

    // Замер времени работы.
    auto start = chrono::steady_clock::now();
 // Используем omp для создания ряда потоков.
#pragma omp parallel shared (books) num_threads(4)
    {
 // Используем параллельный цикл.
#pragma omp for
        for (int i = 0; i < n; ++i) {
 // Используем critical, чтобы считывание происходило корректно.
#pragma omp critical
            {
                cin >> name;
                cin >> line;
                cin >> closet;
                cin >> number_in_closet;
                books[i] = Book(name, line, closet, number_in_closet);
            }
        }
// Распараллеливаем задачи.
// Создаем основной каталог.
#pragma omp task
        {
            sortCatalog(books, n);
        }

// Создаем каталог по ряду.
#pragma omp task
        {
            createCatalogForLine(books, books2, n, line_number);
        }
    }
    auto end = chrono::steady_clock::now();
    cout << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms" << endl;

    // Вывод содержимого основного каталога.
    for (Book bk: books) {
        cout << bk.name <<" " << bk.line <<" " << bk.closet <<" " <<bk.number_in_closet << endl;
    }

    cout << endl;
    // Вывод содержимого каталога по ряду.
    for (Book bk: books2) {
        cout << bk.name <<" " << bk.line <<" " << bk.closet <<" " <<bk.number_in_closet << endl;
    }


    return 0;
}
