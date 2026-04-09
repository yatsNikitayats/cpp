#include <iostream>
#include <ctime>
#include <locale>
#include "student.h"
#include "mail_system.h"

int main() {
    srand((unsigned)time(0));

    setlocale(LC_ALL, "");
    std::locale::global(std::locale(""));

    const int MAX_EQUATIONS = 100;
    double equations[MAX_EQUATIONS][3];

    int eqCount = loadEquations("data/equations.txt", equations, MAX_EQUATIONS);

    if (eqCount == 0) {
        std::cout << "Нет уравнений\n";
        return 1;
    }

    Student students[] = {
        {"Яцишин", GOOD},
        {"Куликова", MID},
        {"Хаймин", BAD},
        {"Славгородский", MID},
        {"Фролов", GOOD}
    };

    int studentCount = sizeof(students) / sizeof(students[0]);

    MailQueue queue;
    generateMails(queue, equations, eqCount, students, studentCount);

    ResultsTable results;
    processMails(queue, results);

    results.print();

    return 0;
}