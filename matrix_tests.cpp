#include "matrix.h"
#include <iostream>
#include <cassert>
#include <vector>

void test_constructors() {
    std::cout << "Testing Constructors...\n";
    
    // Test default constructor
    Matrix m1;
    assert(m1.getRows() == 0);
    assert(m1.getCols() == 0);
    
    // Test dimension constructor
    Matrix m2(3, 4);
    assert(m2.getRows() == 3);
    assert(m2.getCols() == 4);
    
    // Test values constructor
    std::vector<double> values = {1, 2, 3, 4, 5, 6};
    Matrix m3(2, 3, values);
    assert(m3.getRows() == 2);
    assert(m3.getCols() == 3);
    assert(m3(0, 0) == 1);
    assert(m3(1, 2) == 6);
    
    // Test initializer-list constructor
    Matrix m4 = {{1, 2, 3}, {4, 5, 6}};
    assert(m4.getRows() == 2);
    assert(m4.getCols() == 3);
    assert(m4(0, 0) == 1);
    assert(m4(1, 2) == 6);
    
    // Test exception for zero dimensions
    try {
        Matrix m5(0, 5);
        assert(false); // Should not reach here
    } catch (const std::invalid_argument&) {
        // Expected
    }
    
    // Test exception for differing row lengths
    try {
        Matrix m6 = {{1, 2}, {3, 4, 5}};
        assert(false); // Should not reach here
    } catch (const std::invalid_argument&) {
        // Expected
    }
    
    std::cout << "Constructor tests passed!\n\n";
}

void test_element_access() {
    std::cout << "Testing Element Access...\n";
    
    Matrix m = {{1, 2, 3}, {4, 5, 6}};
    
    // Test const access
    assert(m(0, 0) == 1);
    assert(m(0, 1) == 2);
    assert(m(1, 2) == 6);
    
    // Test non-const access and modification
    m(0, 0) = 10;
    assert(m(0, 0) == 10);
    
    std::cout << "Element access tests passed!\n\n";
}

void test_addition() {
    std::cout << "Testing Addition...\n";
    
    Matrix m1 = {{1, 2}, {3, 4}};
    Matrix m2 = {{5, 6}, {7, 8}};
    Matrix result = m1 + m2;
    
    assert(result(0, 0) == 6);
    assert(result(0, 1) == 8);
    assert(result(1, 0) == 10);
    assert(result(1, 1) == 12);
    
    // Test dimension mismatch
    Matrix m3 = {{1, 2, 3}};
    try {
        Matrix bad_result = m1 + m3;
        assert(false); // Should not reach here
    } catch (const std::invalid_argument&) {
        // Expected
    }
    
    std::cout << "Addition tests passed!\n\n";
}

void test_subtraction() {
    std::cout << "Testing Subtraction...\n";
    
    Matrix m1 = {{5, 6}, {7, 8}};
    Matrix m2 = {{1, 2}, {3, 4}};
    Matrix result = m1 - m2;
    
    assert(result(0, 0) == 4);
    assert(result(0, 1) == 4);
    assert(result(1, 0) == 4);
    assert(result(1, 1) == 4);
    
    std::cout << "Subtraction tests passed!\n\n";
}

void test_multiplication() {
    std::cout << "Testing Multiplication...\n";
    
    Matrix m1 = {{1, 2}, {3, 4}};
    Matrix m2 = {{5, 6}, {7, 8}};
    Matrix result = m1 * m2;
    
    // Expected: [[1*5+2*7, 1*6+2*8], [3*5+4*7, 3*6+4*8]]
    //           [[19, 22], [43, 50]]
    assert(result(0, 0) == 19);
    assert(result(0, 1) == 22);
    assert(result(1, 0) == 43);
    assert(result(1, 1) == 50);
    
    // Test non-square multiplication
    Matrix m3 = {{1, 2, 3}, {4, 5, 6}};  // 2x3
    Matrix m4 = {{7, 8}, {9, 10}, {11, 12}};  // 3x2
    Matrix result2 = m3 * m4;  // Should be 2x2
    
    assert(result2.getRows() == 2);
    assert(result2.getCols() == 2);
    // Expected: [[1*7+2*9+3*11, 1*8+2*10+3*12], [4*7+5*9+6*11, 4*8+5*10+6*12]]
    //           [[58, 64], [139, 154]]
    assert(result2(0, 0) == 58);
    assert(result2(0, 1) == 64);
    assert(result2(1, 0) == 139);
    assert(result2(1, 1) == 154);
    
    // Test dimension incompatibility
    Matrix m5 = {{1, 2, 3}};  // 1x3
    Matrix m6 = {{1, 2}};     // 1x2
    try {
        Matrix bad_result = m5 * m6;
        assert(false); // Should not reach here
    } catch (const std::invalid_argument&) {
        // Expected
    }
    
    std::cout << "Multiplication tests passed!\n\n";
}

void test_transpose() {
    std::cout << "Testing Transpose...\n";
    
    Matrix m = {{1, 2, 3}, {4, 5, 6}};  // 2x3
    Matrix transposed = m.transpose();   // Should be 3x2
    
    assert(transposed.getRows() == 3);
    assert(transposed.getCols() == 2);
    assert(transposed(0, 0) == 1);
    assert(transposed(0, 1) == 4);
    assert(transposed(1, 0) == 2);
    assert(transposed(1, 1) == 5);
    assert(transposed(2, 0) == 3);
    assert(transposed(2, 1) == 6);
    
    // Test square matrix transpose
    Matrix square = {{1, 2}, {3, 4}};
    Matrix square_t = square.transpose();
    assert(square_t(0, 0) == 1);
    assert(square_t(0, 1) == 3);
    assert(square_t(1, 0) == 2);
    assert(square_t(1, 1) == 4);
    
    std::cout << "Transpose tests passed!\n\n";
}

void test_output() {
    std::cout << "Testing Output...\n";
    
    Matrix m = {{1.5, 2.7, 3.14159}, {4.0, 5.5, 6.9}};
    std::cout << "Matrix output test:\n" << m << "\n";
    
    std::cout << "Output test completed!\n\n";
}

int main() {
    std::cout << "=== Matrix Library Tests ===\n\n";
    
    test_constructors();
    test_element_access();
    test_addition();
    test_subtraction();
    test_multiplication();
    test_transpose();
    test_output();
    
    std::cout << "=== All Tests Passed! ===\n";
    return 0;
}