#pragma once

#include <vector>
#include <iostream>
#include <stdexcept>
#include <cassert>
#include <initializer_list>
#include <iomanip>

class Matrix {
private:
    std::vector<double> data; // row-major order
    int rows, cols;

public:
    // Constructors
    Matrix(); // Default constructor
    Matrix(int r, int c); // Dimension constructor (fill with zeros)
    Matrix(int r, int c, const std::vector<double>& values);
    Matrix(std::initializer_list<std::initializer_list<double>> init_list); // Required initializer-list constructor

    // Element access
    double& operator()(int i, int j);
    double operator()(int i, int j) const;

    // Getters
    int getRows() const;
    int getCols() const;

    // Matrix operations
    Matrix operator+(const Matrix& other) const;
    Matrix operator-(const Matrix& other) const;
    Matrix operator*(const Matrix& other) const;
    Matrix transpose() const;

    // Print
    void print() const;
    friend std::ostream& operator<<(std::ostream& os, const Matrix& mat);
};