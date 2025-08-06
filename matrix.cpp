#include "matrix.h"

// Initialize thread count based on hardware
const unsigned int Matrix::MAX_THREADS = std::max(1u, std::thread::hardware_concurrency());

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

// Initializer-list constructor
Matrix::Matrix(std::initializer_list<std::initializer_list<double>> init_list) {
    if (init_list.size() == 0)
        throw std::invalid_argument("Matrix dimensions must be positive");
    
    rows = static_cast<int>(init_list.size());
    cols = static_cast<int>(init_list.begin()->size());
    
    if (cols == 0)
        throw std::invalid_argument("Matrix dimensions must be positive");
    
    // Check all rows have same length
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

// Helper function for threaded addition/subtraction
void Matrix::addSubtractRange(const Matrix& other, std::vector<double>& result, 
                             int start_idx, int end_idx, bool is_addition) const {
    if (is_addition) {
        for (int i = start_idx; i < end_idx; ++i) {
            result[i] = data[i] + other.data[i];
        }
    } else {
        for (int i = start_idx; i < end_idx; ++i) {
            result[i] = data[i] - other.data[i];
        }
    }
}

// Optimized addition with threading
Matrix Matrix::operator+(const Matrix& other) const {
    if (rows != other.rows || cols != other.cols)
        throw std::invalid_argument("Matrix dimensions must match for addition");
    
    std::vector<double> result_data(rows * cols);
    
    // Use threading for larger matrices
    if (rows * cols >= MIN_SIZE_FOR_THREADING) {
        const int total_elements = rows * cols;
        const unsigned int num_threads = std::min(MAX_THREADS, static_cast<unsigned int>(total_elements / MIN_SIZE_FOR_THREADING + 1));
        const int elements_per_thread = total_elements / num_threads;
        
        std::vector<std::thread> threads;
        threads.reserve(num_threads);
        
        for (unsigned int t = 0; t < num_threads; ++t) {
            int start_idx = t * elements_per_thread;
            int end_idx = (t == num_threads - 1) ? total_elements : (t + 1) * elements_per_thread;
            
            threads.emplace_back(&Matrix::addSubtractRange, this, std::cref(other), 
                               std::ref(result_data), start_idx, end_idx, true);
        }
        
        for (auto& thread : threads) {
            thread.join();
        }
    } else {
        // Single-threaded for smaller matrices
        for (int i = 0; i < rows * cols; ++i)
            result_data[i] = data[i] + other.data[i];
    }
    
    return Matrix(rows, cols, result_data);
}

// Optimized subtraction with threading
Matrix Matrix::operator-(const Matrix& other) const {
    if (rows != other.rows || cols != other.cols)
        throw std::invalid_argument("Matrix dimensions must match for subtraction");
    
    std::vector<double> result_data(rows * cols);
    
    // Use threading for larger matrices
    if (rows * cols >= MIN_SIZE_FOR_THREADING) {
        const int total_elements = rows * cols;
        const unsigned int num_threads = std::min(MAX_THREADS, static_cast<unsigned int>(total_elements / MIN_SIZE_FOR_THREADING + 1));
        const int elements_per_thread = total_elements / num_threads;
        
        std::vector<std::thread> threads;
        threads.reserve(num_threads);
        
        for (unsigned int t = 0; t < num_threads; ++t) {
            int start_idx = t * elements_per_thread;
            int end_idx = (t == num_threads - 1) ? total_elements : (t + 1) * elements_per_thread;
            
            threads.emplace_back(&Matrix::addSubtractRange, this, std::cref(other), 
                               std::ref(result_data), start_idx, end_idx, false);
        }
        
        for (auto& thread : threads) {
            thread.join();
        }
    } else {
        // Single-threaded for smaller matrices
        for (int i = 0; i < rows * cols; ++i)
            result_data[i] = data[i] - other.data[i];
    }
    
    return Matrix(rows, cols, result_data);
}

// Helper function for threaded matrix multiplication
void Matrix::multiplyRowRange(const Matrix& other, std::vector<double>& result, 
                             int start_row, int end_row) const {
    for (int i = start_row; i < end_row; ++i) {
        for (int j = 0; j < other.cols; ++j) {
            double sum = 0.0;
            for (int k = 0; k < cols; ++k) {
                sum += (*this)(i, k) * other(k, j);
            }
            result[i * other.cols + j] = sum;
        }
    }
}

// Optimized multiplication with threading
Matrix Matrix::operator*(const Matrix& other) const {
    if (cols != other.rows)
        throw std::invalid_argument("Matrix dimensions must be compatible for multiplication");

    std::vector<double> result_data(rows * other.cols, 0.0);
    
    // Use threading for larger matrices
    if (rows >= MIN_SIZE_FOR_THREADING || other.cols >= MIN_SIZE_FOR_THREADING) {
        const unsigned int num_threads = std::min(MAX_THREADS, static_cast<unsigned int>(rows));
        const int rows_per_thread = std::max(1, rows / static_cast<int>(num_threads));
        
        std::vector<std::thread> threads;
        threads.reserve(num_threads);
        
        for (unsigned int t = 0; t < num_threads; ++t) {
            int start_row = t * rows_per_thread;
            int end_row = (t == num_threads - 1) ? rows : std::min(rows, static_cast<int>((t + 1) * rows_per_thread));
            
            if (start_row < rows) {
                threads.emplace_back(&Matrix::multiplyRowRange, this, std::cref(other), 
                                   std::ref(result_data), start_row, end_row);
            }
        }
        
        for (auto& thread : threads) {
            thread.join();
        }
    } else {
        // Single-threaded for smaller matrices
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < other.cols; ++j) {
                for (int k = 0; k < cols; ++k) {
                    result_data[i * other.cols + j] += (*this)(i, k) * other(k, j);
                }
            }
        }
    }

    return Matrix(rows, other.cols, result_data);
}

// Helper function for threaded transpose
void Matrix::transposeRange(std::vector<double>& result, int start_row, int end_row) const {
    for (int i = start_row; i < end_row; ++i) {
        for (int j = 0; j < cols; ++j) {
            result[j * rows + i] = (*this)(i, j);
        }
    }
}

// Optimized transpose with threading
Matrix Matrix::transpose() const {
    std::vector<double> result_data(cols * rows);
    
    // Use threading for larger matrices
    if (rows >= MIN_SIZE_FOR_THREADING || cols >= MIN_SIZE_FOR_THREADING) {
        const unsigned int num_threads = std::min(MAX_THREADS, static_cast<unsigned int>(rows));
        const int rows_per_thread = std::max(1, rows / static_cast<int>(num_threads));
        
        std::vector<std::thread> threads;
        threads.reserve(num_threads);
        
        for (unsigned int t = 0; t < num_threads; ++t) {
            int start_row = t * rows_per_thread;
            int end_row = (t == num_threads - 1) ? rows : std::min(rows, static_cast<int>((t + 1) * rows_per_thread));
            
            if (start_row < rows) {
                threads.emplace_back(&Matrix::transposeRange, this, std::ref(result_data), 
                                   start_row, end_row);
            }
        }
        
        for (auto& thread : threads) {
            thread.join();
        }
    } else {
        // Single-threaded for smaller matrices
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                result_data[j * rows + i] = (*this)(i, j);
            }
        }
    }

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