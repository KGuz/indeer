#pragma once
#include "ThreadPool.h"
#include <filesystem>
#include <fstream>
#include <future>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

/// Structure for containing number of lines, tokens and symbols of a file
struct FileInfo {
    int lines, tokens, symbols;
};

/**
  Prints contents of FileInfo to specified output stream
  @param os Output stream to which the data will be written
  @param fi File information struct containing data
  @return Result of writting data to output stream
*/
std::ostream &operator<<(std::ostream &os, const FileInfo &fi);

/** 
  Implementation of multithreaded directory analyzer
  Contains methods for inspecting files inside specific directory and displaying information to stdout 
 */
class DirectoryInspector {
    std::unordered_map<std::string, std::future<FileInfo>> results;
    std::filesystem::path last_path;

    /**
	  Helper function for recursively printing directory structure to stdout
	  @param cur_dir Path to the currently explored directory
	  @param prefix String containing the proper indentation symbols
	*/
    void directory_print_helper(std::filesystem::path cur_dir, std::string prefix);

  public:
    /**
	  Analyze contents of given file
	  @param file_path Path to inspected file
	  @return Information about number of lines, tokens and symbols inside a file
	*/
    static FileInfo inspect_file(const std::filesystem::path &file_path);

    /**
	  Analyze files inside of given directory
	  @param directory_path Path to inspected directory
	  @param thread_count Number of worker threads that analyze files
	*/
    void inspect_directory(const std::filesystem::path directory_path, unsigned int thread_count = std::thread::hardware_concurrency());

    /// Print out analyzed directory to stdout
    void print_inspected_directory();

    /**
	  Retrives results of directory inspection
	  @return Returns hashmap containig paths to files and their statistics 
	*/
    std::unordered_map<std::string, FileInfo> get_results();
};