#include "equation.h"

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

    return correct == 0;
}