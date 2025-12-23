/*
 * RAII Wrapper Example
 *
 * RAII (Resource Acquisition Is Initialization) is a C++ idiom for managing resources
 * such as memory, file handles, sockets, or locks. The core idea is that resource
 * allocation happens during object construction, and resource release happens
 * automatically during object destruction. This ensures that resources are
 * properly cleaned up even in the presence of exceptions or early returns.
 *
 * This file demonstrates a simple RAII wrapper class for a FILE* resource. The
 * FileRAII class:
 *   - Acquires a file handle in its constructor.
 *   - Automatically closes the file in its destructor.
 *   - Prevents copying to avoid double resource release.
 *   - Supports move semantics for safe transfer of ownership.
 *
 * Benefits:
 *   - Eliminates manual cleanup and reduces resource leaks.
 *   - Works seamlessly with exceptions.
 *   - Makes code safer, clearer, and more maintainable.
 */

#include <iostream>
#include <cstdio>    // For FILE*
#include <stdexcept> // For exceptions

// RAII wrapper for a FILE* resource
class FileRAII {
    FILE* file_;
public:
    // Constructor opens the file
    explicit FileRAII(const char* filename, const char* mode) {
        file_ = std::fopen(filename, mode);
        if (!file_) {
            throw std::runtime_error("Failed to open file");
        }
    }

    // Delete copy operations to prevent double-close
    FileRAII(const FileRAII&) = delete;
    FileRAII& operator=(const FileRAII&) = delete;

    // Move operations to transfer ownership

    // Move constructor
    FileRAII(FileRAII&& other) noexcept : file_(other.file_) {
        other.file_ = nullptr;
    }

    // Move assignement operator
    FileRAII& operator=(FileRAII&& other) noexcept {
        if (this != &other) {
            close();
            file_ = other.file_;
            other.file_ = nullptr;
        }
        return *this;
    }

    // Access the underlying FILE*
    FILE* get() const { return file_; }

    // Destructor closes the file
    ~FileRAII() {
        close();
    }

private:
    void close() {
        if (file_) {
            std::fclose(file_);
            file_ = nullptr;
        }
    }
};

int main() {
    try {
        FileRAII f("example_from_raii_cpp.txt", "w");
        std::fprintf(f.get(), "Hello RAII!\n");

        // File automatically closed when going out of scope
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
}
