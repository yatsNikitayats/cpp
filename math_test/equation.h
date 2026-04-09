#pragma once
#include <cmath>

const double EPS = 1e-6;

int solveEquation(double a, double b, double c, double& x1, double& x2);

bool checkAnswer(double a, double b, double c,
    double root1, double root2,
    int solutionType);