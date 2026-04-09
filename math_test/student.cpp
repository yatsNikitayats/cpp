#include "student.h"
#include "equation.h"
#include <cstdlib>

static void makeWrongAnswer(double& root1, double& root2, int& solutionType) {
    solutionType = 1;
    root1 = root2 = 0;
}

Student::Student(const std::string& n, int t)
    : name(n), type(t) {
}

const std::string& Student::getName() const {
    return name;
}

void Student::solve(double a, double b, double c,
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