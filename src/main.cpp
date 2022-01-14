#include "DirectoryInspector.h"
#include "ThreadPool.h"
#include <filesystem>

int main(int argc, char *argv[]) {
    // check for number of arguments passed to main
    if (argc < 2) {
        std::cerr << "Error: Command line argument not entered\n";
        std::exit(1);
    }

    // check for valid directory path in passed arguments
    const std::filesystem::path dir_path(argv[1]);
    if (!std::filesystem::is_directory(dir_path)) {
        std::cerr << "Error: The argument entered is not a valid directory path\n";
        std::exit(1);
    }

    // check for thread count argument
    unsigned int thread_count = std::thread::hardware_concurrency();
    if (argc > 2) {
        try {
            thread_count = std::stoi(argv[2]);
        } catch (const std::invalid_argument &e) {
            std::cerr << "Error: The argument entered is not a valid number\n";
            std::exit(1);
        }
    }

    // inspect given directory and print results to stdout
    DirectoryInspector di;
    di.inspect_directory(dir_path, thread_count);
    di.print_inspected_directory();

    return 0;
}