#pragma once

#include <vector>
#include <iostream>
#include <stdexcept>
#include <cassert>
#include <initializer_list>
#include <iomanip>
#include <thread>
#include <algorithm>
#include <future>

class Matrix {
private:
    std::vector<double> data; // row-major order
    int rows, cols;
    
    // Thread optimization settings
    static constexpr int MIN_SIZE_FOR_THREADING = 64;  // Minimum matrix size to use threading
    static const unsigned int MAX_THREADS;             // Will be set to hardware concurrency

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

private:
    // Helper functions for threaded operations
    void addSubtractRange(const Matrix& other, std::vector<double>& result, 
                         int start_idx, int end_idx, bool is_addition) const;
    void multiplyRowRange(const Matrix& other, std::vector<double>& result, 
                         int start_row, int end_row) const;
    void transposeRange(std::vector<double>& result, int start_row, int end_row) const;
};