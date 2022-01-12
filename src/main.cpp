#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "ThreadPool.hpp"

namespace fs = std::filesystem;

struct FileInfo {
	int lines, words, letters;
};

FileInfo analyze_file(const fs::path& p);
std::ostream& operator<<(std::ostream& os, const FileInfo& fi);
void print_directory_tree(fs::path cur_dir, std::unordered_map <std::string, std::future<FileInfo>>& map, std::string prefix = "| ");


int main(int argc, char* argv[]) {
	if (argc < 2) {
		std::cerr << "Error: No command line argument entered\n";
		std::exit(1);
	}

	const fs::path dir_path(argv[1]);
	if (!fs::is_directory(dir_path)) {
		std::cerr << "Error: The entered argument is not a valid directory path\n";
		std::exit(1);
	}

	ThreadPool pool(4);
	std::unordered_map <std::string, std::future<FileInfo>> results;
	for (auto& it : fs::recursive_directory_iterator(dir_path)) {
		if (fs::is_regular_file(it.path())) {
			results[it.path().string()] = pool.submit(analyze_file, it.path());
		}
	}

	std::cout << dir_path.string();
	print_directory_tree(dir_path, results);
	std::cout << std::endl;
	return 0;
}


FileInfo analyze_file(const fs::path& p) {
	FileInfo fi = { 0, 0, 0 };
	std::ifstream file(p);
	std::string line;

	while (std::getline(file, line))
	{
		fi.lines++;
		std::stringstream line_stream(line);

		while (std::getline(line_stream, line, ' ')) {
			fi.words++;
			fi.letters += line.length();
		}
	}
	return fi;
}

std::ostream& operator<<(std::ostream& os, const FileInfo& fi) {
	return os << fi.lines << " lines, " << fi.words << " words, " << fi.letters << " letters";
}

void print_directory_tree(fs::path cur_dir, std::unordered_map <std::string, std::future<FileInfo>>& map, std::string prefix) {
	for (auto& it : fs::directory_iterator(cur_dir)) {
		std::string filename_str = it.path().filename().string();
		std::string path_str = it.path().string();

		std::string cur_prefix = prefix;
		cur_prefix.replace(prefix.size() - 2, 3, "|-");
		std::cout << std::endl << cur_prefix << filename_str;

		if (map.find(path_str) != map.end()) {
			try {
				std::cout << " : " << map[path_str].get();
			}
			catch (const std::exception& e) {
				std::cout << " : exception occured while processing this file - \"" << e.what() << "\"";
			}
		}

		if (fs::is_directory(it.path())) {
			print_directory_tree(it.path(), map, prefix + "| ");
		}
	}
}