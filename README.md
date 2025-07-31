# Matrix Library

A reusable C++ matrix library supporting basic linear algebra operations with elements stored contiguously in row-major order.

## Design Choices

### Storage Strategy
- **Row-major storage**: Elements are stored contiguously in a `std::vector<double>` using row-major order for cache-friendly access patterns and compatibility with most linear algebra libraries.
- **Single vector approach**: All matrix data is stored in one contiguous block, enabling efficient memory access and simple indexing with formula `index = i * cols + j`.

### Constructor Design
- **Multiple constructors**: Default, dimension-based, value-based, and initializer-list constructors provide flexibility for different use cases.
- **Initializer-list support**: Enables intuitive syntax like `Matrix m = {{1,2},{3,4}}` with validation for equal row lengths.
- **Exception safety**: All constructors validate input and throw `std::invalid_argument` for invalid dimensions or mismatched row sizes.

### Rule of Zero Implementation
- **Compiler-generated special members**: Relies on automatically generated copy constructor, copy assignment, move constructor, move assignment, and destructor.
- **RAII compliance**: Uses `std::vector` for automatic memory management, eliminating need for custom resource management.

### Element Access
- **Bounds checking**: Uses `assert()` for debug-time bounds checking while maintaining release performance.
- **Dual access operators**: Provides both const and non-const versions of `operator()` for read and write access.

## Complexity Analysis

### Space Complexity
- **Storage**: O(rows × cols) - linear space proportional to matrix size
- **No overhead**: Minimal memory overhead beyond the actual matrix data

### Time Complexity
- **Element access**: O(1) - constant time indexing
- **Addition/Subtraction**: O(rows × cols) - linear scan through all elements
- **Matrix multiplication**: O(rows₁ × cols₂ × cols₁) - standard three-nested-loop algorithm
- **Transpose**: O(rows × cols) - single pass through all elements with reordering

### Algorithm Choices
- **Standard multiplication**: Uses the straightforward O(n³) algorithm suitable for general-purpose use
- **Element-wise operations**: Optimized for cache locality by processing elements in storage order
- **In-place operations**: Not implemented to maintain immutability and thread safety

## Build Instructions

```bash
mkdir build && cd build
cmake ..
make
./matrix_tests
```

## Usage Examples

```cpp
// Different construction methods
Matrix m1(3, 3);                          // 3x3 zero matrix
Matrix m2 = {{1, 2}, {3, 4}};             // 2x2 from initializer list
Matrix m3(2, 2, {1, 2, 3, 4});            // 2x2 from vector

// Basic operations
Matrix sum = m2 + m3;                      // Matrix addition
Matrix product = m2 * m3;                  // Matrix multiplication
Matrix difference = m2-m1;                 // Matrix substraction
Matrix transposed = m2.transpose();        // Matrix transpose

// Element access
double element = m2(0, 1);                 // Read element
m2(1, 0) = 5.0;                            // Write element
```

## Library Features

**Exception safety**: Proper error handling for invalid operations
**Type safety**: Strong typing with compile-time interface checking  
**Memory safety**: No raw pointers or manual memory management
**Debug support**: Bounds checking in debug builds
**Standard compliance**: Uses only standard C++ containers and facilities