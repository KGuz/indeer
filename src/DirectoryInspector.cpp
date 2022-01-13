#include "DirectoryInspector.h"

// Prints contents of FileInfo to specified output stream
std::ostream& operator<<(std::ostream& os, const FileInfo& fi) {
	return os << fi.lines << " lines, " << fi.tokens << " tokens, " << fi.symbols << " symbols";
}

// Print out analyzed directory to stdout
void DirectoryInspector::print_inspected_directory() {
	if (!results.empty()) {
		std::cout << last_path.string();
		directory_print_helper(last_path, "| ");
		std::cout << std::endl;
	}
}

// Helper function for recursively printing directory structure to stdout
void DirectoryInspector::directory_print_helper(std::filesystem::path cur_dir, std::string prefix) {
	for (auto& it : std::filesystem::directory_iterator(cur_dir)) {
		std::string filename_str = it.path().filename().string();
		std::string path_str = it.path().string();

		std::string cur_prefix = prefix;
		cur_prefix.replace(prefix.size() - 2, 3, "|-");
		std::cout << std::endl << cur_prefix << filename_str;

		if (results.find(path_str) != results.end()) {
			try {
				std::cout << " : " << results[path_str].get();
			}
			catch (const std::exception& e) {
				std::cout << " : exception occured while processing this file - \"" << e.what() << "\"";
			}
		}

		if (std::filesystem::is_directory(it.path())) {
			directory_print_helper(it.path(), prefix + "| ");
		}
	}
}

// Analyze contents of given file
FileInfo DirectoryInspector::inspect_file(const std::filesystem::path& file_path) {
	FileInfo fi = { 0, 0, 0 };
	std::ifstream file(file_path);
	std::string line;

	while (std::getline(file, line))
	{
		fi.lines++;
		std::stringstream line_stream(line);

		while (std::getline(line_stream, line, ' ')) {
			line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
			if (line.empty())
				continue;
				
			fi.tokens++;
			fi.symbols += line.length();
		}
	}
	return fi;
}

// Analyze files inside of given directory
void DirectoryInspector::inspect_directory(const std::filesystem::path directory_path, unsigned int thread_count) {
	if (last_path != directory_path) {
		last_path = directory_path;
		results.clear();

		ThreadPool pool(thread_count);
		for (auto& it : std::filesystem::recursive_directory_iterator(directory_path)) {
			if (std::filesystem::is_regular_file(it.path())) {
				results[it.path().string()] = pool.submit(inspect_file, it.path());
			}
		}
	}
}

// Retrives results of directory inspection
std::unordered_map<std::string, FileInfo> DirectoryInspector::get_results() {
	std::unordered_map<std::string, FileInfo> ans;
	for (auto &[key, val] : results) {
		try {
			ans[key] = val.get();
		}
		catch (const std::exception& e) {
			std::cout << " : exception occured while processing this file - \"" << e.what() << "\"";
		}
	}
	return ans;
}
