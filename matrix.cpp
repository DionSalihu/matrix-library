#include "matrix.h"

// Default constructor
Matrix::Matrix() : rows(0), cols(0) {}

// Dimension constructor (fill with zeros)
Matrix::Matrix(int r, int c) : rows(r), cols(c) {
    if (r <= 0 || c <= 0)
        throw std::invalid_argument("Matrix dimensions must be positive");
    data.resize(r * c, 0.0);
}

// Constructor with values
Matrix::Matrix(int r, int c, const std::vector<double>& values) : rows(r), cols(c) {
    if (r <= 0 || c <= 0)
        throw std::invalid_argument("Matrix dimensions must be positive");
    if (values.size() != static_cast<size_t>(r * c))
        throw std::invalid_argument("Incorrect number of values");
    data = values;
}

// Initializer-list constructor (REQUIRED by assignment)
Matrix::Matrix(std::initializer_list<std::initializer_list<double>> init_list) {
    if (init_list.size() == 0)
        throw std::invalid_argument("Matrix dimensions must be positive");
    
    rows = static_cast<int>(init_list.size());
    cols = static_cast<int>(init_list.begin()->size());
    
    if (cols == 0)
        throw std::invalid_argument("Matrix dimensions must be positive");
    
    // Check all rows have same length (required by assignment)
    for (const auto& row : init_list) {
        if (static_cast<int>(row.size()) != cols)
            throw std::invalid_argument("All rows must have the same length");
    }
    
    data.reserve(rows * cols);
    for (const auto& row : init_list) {
        for (double val : row) {
            data.push_back(val);
        }
    }
}

// Element access
double& Matrix::operator()(int i, int j) {
    assert(i >= 0 && i < rows && j >= 0 && j < cols);
    return data[i * cols + j];
}

double Matrix::operator()(int i, int j) const {
    assert(i >= 0 && i < rows && j >= 0 && j < cols);
    return data[i * cols + j];
}

// Getters
int Matrix::getRows() const { return rows; }
int Matrix::getCols() const { return cols; }

// Addition
Matrix Matrix::operator+(const Matrix& other) const {
    if (rows != other.rows || cols != other.cols)
        throw std::invalid_argument("Matrix dimensions must match for addition");
    
    std::vector<double> result_data(rows * cols);
    for (int i = 0; i < rows * cols; ++i)
        result_data[i] = data[i] + other.data[i];
    
    return Matrix(rows, cols, result_data);
}

// Subtraction
Matrix Matrix::operator-(const Matrix& other) const {
    if (rows != other.rows || cols != other.cols)
        throw std::invalid_argument("Matrix dimensions must match for subtraction");
    
    std::vector<double> result_data(rows * cols);
    for (int i = 0; i < rows * cols; ++i)
        result_data[i] = data[i] - other.data[i];
    
    return Matrix(rows, cols, result_data);
}

// Multiplication (linear algebra)
Matrix Matrix::operator*(const Matrix& other) const {
    if (cols != other.rows)
        throw std::invalid_argument("Matrix dimensions must be compatible for multiplication");

    std::vector<double> result_data(rows * other.cols, 0.0);
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < other.cols; ++j)
            for (int k = 0; k < cols; ++k)
                result_data[i * other.cols + j] += (*this)(i, k) * other(k, j);

    return Matrix(rows, other.cols, result_data);
}

// Transpose
Matrix Matrix::transpose() const {
    std::vector<double> result_data(cols * rows);
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            result_data[j * rows + i] = (*this)(i, j);

    return Matrix(cols, rows, result_data);
}

// Print
void Matrix::print() const {
    std::cout << *this;
}

// operator<<
std::ostream& operator<<(std::ostream& os, const Matrix& mat) {
    for (int i = 0; i < mat.rows; ++i) {
        os << "[";
        for (int j = 0; j < mat.cols; ++j) {
            os << std::setw(8) << std::fixed << std::setprecision(2) << mat(i, j);
            if (j < mat.cols - 1) os << " ";
        }
        os << "]\n";
    }
    return os;
}