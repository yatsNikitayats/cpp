#include "mail_system.h"
#include "equation.h"
#include <iostream>
#include <fstream>



MailQueue::MailQueue() : head(nullptr), tail(nullptr) {}

MailQueue::~MailQueue() {
    while (!empty()) delete pop();
}

void MailQueue::push(double a, double b, double c,
    double root1, double root2,
    int solutionType,
    const std::string& name) {

    Mail* m = new Mail{ a, b, c, root1, root2, solutionType, name, nullptr };

    if (!head) head = tail = m;
    else {
        tail->next = m;
        tail = m;
    }
}

Mail* MailQueue::pop() {
    if (!head) return nullptr;

    Mail* t = head;
    head = head->next;

    if (!head) tail = nullptr;

    return t;
}

bool MailQueue::empty() const {
    return head == nullptr;
}



ResultsTable::ResultsTable() : head(nullptr) {}

ResultsTable::~ResultsTable() {
    while (head) {
        Result* t = head;
        head = head->next;
        delete t;
    }
}

void ResultsTable::add(const std::string& name, bool correct) {
    if (!correct) return;

    Result* cur = head;

    while (cur) {
        if (cur->name == name) {
            cur->solved++;
            return;
        }
        cur = cur->next;
    }

    head = new Result{ name, 1, head };
}

void ResultsTable::print() const {
    std::cout << "\n=== Итоги зачетной недели ===\n";

    Result* cur = head;
    while (cur) {
        std::cout << cur->name;

        for (size_t i = cur->name.length(); i < 20; i++)
            std::cout << " ";

        std::cout << "| " << cur->solved << "\n";
        cur = cur->next;
    }
}



int loadEquations(const std::string& filename,
    double equations[][3],
    int maxEquations) {

    std::ifstream file(filename);
    int count = 0;

    if (!file.is_open()) {
        std::cout << "Ошибка открытия файла\n";
        return 0;
    }

    while (count < maxEquations &&
        file >> equations[count][0] >> equations[count][1] >> equations[count][2]) {
        count++;
    }

    std::cout << "Загружено уравнений: " << count << "\n";
    return count;
}

void generateMails(MailQueue& queue,
    const double equations[][3],
    int eqCount,
    const Student students[],
    int studentCount) {

    for (int i = 0; i < eqCount; i++) {
        double a = equations[i][0];
        double b = equations[i][1];
        double c = equations[i][2];

        for (int j = 0; j < studentCount; j++) {
            double r1, r2;
            int type;

            students[j].solve(a, b, c, r1, r2, type);
            queue.push(a, b, c, r1, r2, type, students[j].getName());
        }
    }

    std::cout << "Сгенерировано писем: "
        << eqCount * studentCount << "\n";
}

void processMails(MailQueue& queue,
    ResultsTable& results) {

    int checked = 0;

    while (!queue.empty()) {
        Mail* m = queue.pop();

        bool ok = checkAnswer(
            m->a, m->b, m->c,
            m->root1, m->root2,
            m->solutionType
        );

        results.add(m->studentName, ok);

        delete m;
        checked++;
    }

    std::cout << "Проверено писем: " << checked << "\n";
}