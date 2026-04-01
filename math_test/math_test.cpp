#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <ctime>
#include <locale>
#include <filesystem>


const double EPS = 1e-6;
const int GOOD = 1;
const int MID = 2;
const int BAD = 3;

struct Mail {
    double a, b, c;
    double root1, root2;
    int solutionType;
    std::string studentName;
    Mail* next;
};

struct Result {
    std::string name;
    int solved;
    Result* next;
};


void makeWrongAnswer(double& root1, double& root2, int& solutionType) {
    solutionType = 1;
    root1 = root2 = 0;
}

int solveEquation(double a, double b, double c, double& x1, double& x2) {
    if (fabs(a) < EPS) return -1;

    double D = b * b - 4 * a * c;

    if (D < -EPS) return 0;

    if (fabs(D) < EPS) {
        x1 = -b / (2 * a);
        x2 = x1;
        return 1;
    }

    x1 = (-b + sqrt(D)) / (2 * a);
    x2 = (-b - sqrt(D)) / (2 * a);
    return 2;
}

bool checkAnswer(double a, double b, double c,
    double root1, double root2,
    int solutionType) {

    double x1 = 0, x2 = 0;
    int correct = solveEquation(a, b, c, x1, x2);

    if (correct != solutionType)
        return false;

    if (correct == 1)
        return fabs(root1 - x1) < EPS;

    if (correct == 2) {
        return (fabs(root1 - x1) < EPS && fabs(root2 - x2) < EPS) ||
            (fabs(root1 - x2) < EPS && fabs(root2 - x1) < EPS);
    }

    return true;
}



class MailQueue {
    Mail* head = nullptr;
    Mail* tail = nullptr;

public:
    ~MailQueue() {
        while (!empty()) {
            delete pop();
        }
    }

    void push(double a, double b, double c,
        double root1, double root2,
        int solutionType, const std::string& name) {

        Mail* m = new Mail;
        m->a = a;
        m->b = b;
        m->c = c;
        m->root1 = root1;
        m->root2 = root2;
        m->solutionType = solutionType;
        m->studentName = name;
        m->next = nullptr;

        if (!head) {
            head = tail = m;
        }
        else {
            tail->next = m;
            tail = m;
        }
    }

    Mail* pop() {
        if (!head) return nullptr;

        Mail* t = head;
        head = head->next;

        if (!head)
            tail = nullptr;

        return t;
    }

    bool empty() const {
        return head == nullptr;
    }
};


class ResultsTable {
    Result* head = nullptr;

public:
    ~ResultsTable() {
        while (head) {
            Result* t = head;
            head = head->next;
            delete t;
        }
    }

    void add(std::string name, bool correct) {
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

    void print() const {
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
};


struct Student {
    std::string name;
    int type;

    Student(std::string n, int t) : name(n), type(t) {}

    void solve(double a, double b, double c,
        double& root1, double& root2,
        int& solutionType) const {

        if (type == GOOD) {
            solutionType = solveEquation(a, b, c, root1, root2);

            if (solutionType == -1)
                root1 = root2 = 0;
        }
        else if (type == MID) {
            if (rand() % 2 == 0) {
                solutionType = solveEquation(a, b, c, root1, root2);

                if (solutionType == -1)
                    root1 = root2 = 0;
            }
            else {
                makeWrongAnswer(root1, root2, solutionType);
            }
        }
        else {
            makeWrongAnswer(root1, root2, solutionType);
        }
    }
};




int loadEquations(const std::string& filename, double equations[][3], int maxEquations) {
    std::ifstream file(filename);
    int count = 0;

    if (!file.is_open()) {
        std::cout << "Ошибка: не удалось открыть файл " << filename << std::endl;
        return 0;
    }
    while (count < maxEquations) {
        if (!(file >> equations[count][0] >> equations[count][1] >> equations[count][2])) {
            break;  
        }
        count++;
    }

    std::cout << "Загружено уравнений: " << count << std::endl;
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
            double root1, root2;
            int solutionType;

            students[j].solve(a, b, c, root1, root2, solutionType);
            queue.push(a, b, c, root1, root2, solutionType, students[j].name);
        }
    }

    std::cout << "Сгенерировано писем: "
        << eqCount * studentCount << std::endl;
}



void processMails(MailQueue& queue, ResultsTable& results) {
    int checked = 0;

    while (!queue.empty()) {
        Mail* m = queue.pop();

        bool correct = checkAnswer(
            m->a, m->b, m->c,
            m->root1, m->root2,
            m->solutionType
        );

        results.add(m->studentName, correct);

        delete m;
        checked++;
    }

    std::cout << "Проверено писем: " << checked << std::endl;
}



int main() {
    srand((unsigned)time(0));

    
    setlocale(LC_ALL, "");
    std::locale::global(std::locale(""));

    const int MAX_EQUATIONS = 100;
    double equations[MAX_EQUATIONS][3];

    int eqCount = loadEquations("data/equations.txt", equations, MAX_EQUATIONS);
    
    if (eqCount == 0) {
        std::cout << "Нет уравнений для обработки\n";
        return 1;
    }

    Student students[] = {
        {"Иванов", GOOD},
        {"Петров", MID},
        {"Сидоров", BAD},
        {"Козлов", MID},
        {"Смирнов", GOOD}
    };

    int studentCount = sizeof(students) / sizeof(students[0]);

    MailQueue mailQueue;
    generateMails(mailQueue, equations, eqCount, students, studentCount);

    ResultsTable results;
    processMails(mailQueue, results);

    results.print();

    return 0;
}