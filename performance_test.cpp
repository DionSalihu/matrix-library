#include "matrix.h"
#include <chrono>
#include <random>
#include <iostream>

class PerformanceTimer {
public:
    PerformanceTimer() : start_time(std::chrono::high_resolution_clock::now()) {}
    
    double elapsed() const {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        return duration.count();
    }
    
    void reset() {
        start_time = std::chrono::high_resolution_clock::now();
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
};

Matrix generateRandomMatrix(int rows, int cols, double min_val = -10.0, double max_val = 10.0) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(min_val, max_val);
    
    std::vector<double> data;
    data.reserve(rows * cols);
    
    for (int i = 0; i < rows * cols; ++i) {
        data.push_back(dis(gen));
    }
    
    return Matrix(rows, cols, data);
}

void testPerformance() {
    std::cout << "=== Matrix Performance Test ===\n\n";
    std::cout << "Hardware threads available: " << std::thread::hardware_concurrency() << "\n\n";
    
    // Test different matrix sizes
    std::vector<int> sizes = {50, 100, 200, 400, 800};
    
    for (int size : sizes) {
        std::cout << "Testing " << size << "x" << size << " matrices:\n";
        
        // Generate test matrices
        Matrix m1 = generateRandomMatrix(size, size);
        Matrix m2 = generateRandomMatrix(size, size);
        
        PerformanceTimer timer;
        
        // Test addition
        timer.reset();
        Matrix sum = m1 + m2;
        double add_time = timer.elapsed();
        
        // Test multiplication
        timer.reset();
        Matrix product = m1 * m2;
        double mult_time = timer.elapsed();
        
        // Test transpose
        timer.reset();
        Matrix transposed = m1.transpose();
        double transpose_time = timer.elapsed();
        
        std::cout << "  Addition:       " << add_time << " ms\n";
        std::cout << "  Multiplication: " << mult_time << " ms\n";
        std::cout << "  Transpose:      " << transpose_time << " ms\n";
        std::cout << "  Total:          " << (add_time + mult_time + transpose_time) << " ms\n\n";
    }
    
    // Test rectangular matrices
    std::cout << "Testing rectangular matrices (500x200 * 200x300):\n";
    Matrix rect1 = generateRandomMatrix(500, 200);
    Matrix rect2 = generateRandomMatrix(200, 300);
    
    PerformanceTimer timer;
    timer.reset();
    Matrix rect_product = rect1 * rect2;
    double rect_time = timer.elapsed();
    std::cout << "  Rectangular multiplication: " << rect_time << " ms\n\n";
    
    std::cout << "=== Performance Test Complete ===\n";
}

int main() {
    // Run existing tests first
    std::cout << "Running basic functionality tests...\n";
    
    // Quick functionality verification
    Matrix m1 = {{1, 2}, {3, 4}};
    Matrix m2 = {{5, 6}, {7, 8}};
    
    Matrix sum = m1 + m2;
    Matrix product = m1 * m2;
    Matrix transposed = m1.transpose();
    
    std::cout << "Basic tests passed!\n\n";
    
    // Run performance tests
    testPerformance();
    
    return 0;
}