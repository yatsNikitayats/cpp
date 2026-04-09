#pragma once
#include <string>
#include "student.h"

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

class MailQueue {
private:
    Mail* head;
    Mail* tail;

public:
    MailQueue();
    ~MailQueue();

    void push(double a, double b, double c,
        double root1, double root2,
        int solutionType,
        const std::string& name);

    Mail* pop();
    bool empty() const;
};

class ResultsTable {
private:
    Result* head;

public:
    ResultsTable();
    ~ResultsTable();

    void add(const std::string& name, bool correct);
    void print() const;
};


int loadEquations(const std::string& filename,
    double equations[][3],
    int maxEquations);

void generateMails(MailQueue& queue,
    const double equations[][3],
    int eqCount,
    const Student students[],
    int studentCount);

void processMails(MailQueue& queue,
    ResultsTable& results);