#include <filesystem>
#include "ThreadPool.h"
#include "DirectoryInspector.h"

int main(int argc, char* argv[]) {
	// check for number of arguments passed to main
	if (argc < 2) {
		std::cerr << "Error: No command line argument entered\n";
		std::exit(1);
	}

	// check for valid directory path in passed arguments
	const std::filesystem::path dir_path(argv[1]);
	if (!std::filesystem::is_directory(dir_path)) {
		std::cerr << "Error: The entered argument is not a valid directory path\n";
		std::exit(1);
	}

	// inspect passed directory and print results to stdout
	DirectoryInspector di;
	di.inspect_directory(dir_path, 4);
	di.print_inspected_directory();

	return 0;
}