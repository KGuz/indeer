#include "DirectoryInspector.h"
#include <exception>
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include <random>
#include <sstream>

namespace fs = std::filesystem;

std::filesystem::path create_temporary_directory(unsigned long long max_tries = 1000) {
    auto tmp_dir = std::filesystem::temp_directory_path();
    unsigned long long i = 0;
    std::random_device dev;
    std::mt19937 prng(dev());
    std::uniform_int_distribution<uint64_t> rand(0);
    std::filesystem::path path;
    while (true) {
        std::stringstream ss;
        ss << std::hex << rand(prng);
        path = tmp_dir / ss.str();
        if (std::filesystem::create_directory(path)) {
            break;
        }
        if (i == max_tries) {
            throw std::runtime_error("could not find non-existing directory");
        }
        i++;
    }
    return path;
}

TEST(DirectoryInspectorTest, EmptyFile) {
    fs::path temp_dir = create_temporary_directory();
    fs::path filepath(temp_dir / "file.txt");

    std::ofstream file(filepath);
    file << "";
    file.close();

    DirectoryInspector d;
    auto fi = d.inspect_file(filepath);

    ASSERT_EQ(fi.lines, 0);
    ASSERT_EQ(fi.tokens, 0);
    ASSERT_EQ(fi.symbols, 0);

    fs::remove_all(temp_dir);
}

TEST(DirectoryInspectorTest, FileWithOnlyNewLines) {
    fs::path temp_dir = create_temporary_directory();
    fs::path filepath(temp_dir / "file.txt");

    std::ofstream file(filepath);
    file << "\n\n\n";
    file.close();

    DirectoryInspector d;
    auto fi = d.inspect_file(filepath);

    ASSERT_EQ(fi.lines, 3);
    ASSERT_EQ(fi.tokens, 0);
    ASSERT_EQ(fi.symbols, 0);

    fs::remove_all(temp_dir);
}

TEST(DirectoryInspectorTest, FileWithSingleToken) {
    fs::path temp_dir = create_temporary_directory();
    fs::path filepath(temp_dir / "file.txt");

    std::ofstream file(filepath);
    std::string token = "singletoken";
    file << token;
    file.close();

    DirectoryInspector d;
    auto fi = d.inspect_file(filepath);

    ASSERT_EQ(fi.lines, 1);
    ASSERT_EQ(fi.tokens, 1);
    ASSERT_EQ(fi.symbols, token.length());

    fs::remove_all(temp_dir);
}

TEST(DirectoryInspector_AnalyzeFileTest, FileWithMultipleTokens) {
    fs::path temp_dir = create_temporary_directory();
    fs::path filepath(temp_dir / "file.txt");

    std::ofstream file(filepath);
    std::string s = "multiple\t  tokens \n and   lines";
    file << s;
    file.close();

    DirectoryInspector d;
    auto fi = d.inspect_file(filepath);

    ASSERT_EQ(fi.lines, 2);
    ASSERT_EQ(fi.tokens, 4);

    s.erase(std::remove_if(s.begin(), s.end(), ::isspace), s.end());
    ASSERT_EQ(fi.symbols, s.length());

    fs::remove_all(temp_dir);
}

TEST(DirectoryInspector_AnalyzeDirectoryTest, EmptyDirectory) {
    fs::path temp_dir = create_temporary_directory();

    DirectoryInspector d;
    d.inspect_directory(temp_dir, 4);
    auto map = d.get_results();

    EXPECT_EQ(map.empty(), true);

    fs::remove_all(temp_dir);
}

TEST(DirectoryInspector_AnalyzeDirectoryTest, FileInNestedDirectory) {
    fs::path temp_dir = create_temporary_directory();
    fs::create_directories(temp_dir / "a/b/c");
    fs::path filepath(temp_dir / "a/b/c/file.txt");

    std::ofstream file(filepath);
    file << "Lorem ipsum\ndolor sit amet\n";
    file.close();

    DirectoryInspector d;
    d.inspect_directory(temp_dir, 2);
    auto map = d.get_results();

    EXPECT_EQ(map.size(), 1);

    fs::remove_all(temp_dir);
}

TEST(DirectoryInspector_AnalyzeDirectoryTest, FilesInMultipleDirectories) {
    fs::path temp_dir = create_temporary_directory();
    fs::create_directories(temp_dir / "a/b/c");
    fs::create_directories(temp_dir / "a/d");

    fs::path filepath1(temp_dir / "a/b/c/file1.txt");
    fs::path filepath2(temp_dir / "a/d/file2.txt");

    std::ofstream file1(filepath1);
    file1 << "Lorem ipsum\ndolor sit amet\n";
    file1.close();

    std::ofstream file2(filepath2);
    file2 << "Lorem ipsum\ndolor sit amet\n";
    file2.close();

    DirectoryInspector d;
    d.inspect_directory(temp_dir, 2);
    auto map = d.get_results();

    EXPECT_EQ(map.size(), 2);

    fs::remove_all(temp_dir);
}

int main(int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
