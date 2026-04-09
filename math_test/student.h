#pragma once
#include <string>

const int GOOD = 1;
const int MID = 2;
const int BAD = 3;

class Student {
private:
    std::string name;
    int type;

public:
    Student(const std::string& n, int t);

    const std::string& getName() const;

    void solve(double a, double b, double c,
        double& root1, double& root2,
        int& solutionType) const;
};