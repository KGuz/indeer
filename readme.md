# indeer <img title="" src="./assets/icon.svg" align="left" width="90" style="padding-right: 0.5ch">
### A simple multi-threaded application for displaying statistics of files inside a given directory

## Installation
Clone the repository from github and run `cmake CMakeLists.txt && make`. The executable will be created in the bin folder.

## Usage
Inside a terminal, run the command `./indeer DIRECTORY-PATH [THREAD-COUNT]`. A sample directory has been included in the repository.
Below is the expected output of the `./bin/indeer sample_directory` command running in the repository home directory.


```
sample_directory
|-directory-a
| |-directory-b
| | |-directory-c
| | | |-file-12.txt : 15 lines, 685 tokens, 4026 symbols
| | |-file-10.txt : 43 lines, 2000 tokens, 11635 symbols
| | |-file-11.txt : 23 lines, 1096 tokens, 6257 symbols
| | |-file-6.txt : 19 lines, 812 tokens, 4748 symbols
| | |-file-7.txt : 21 lines, 928 tokens, 5290 symbols
| | |-file-8.txt : 29 lines, 1358 tokens, 7808 symbols
| | |-file-9.txt : 29 lines, 1427 tokens, 8171 symbols
| |-file-3.txt : 23 lines, 978 tokens, 5639 symbols
| |-file-4.txt : 9 lines, 511 tokens, 2941 symbols
| |-file-5.txt : 23 lines, 1000 tokens, 5815 symbols
|-directory-d
| |-directory-e
| | |-file-14.txt : 29 lines, 1270 tokens, 7372 symbols
| |-directory-f
| | |-directory-g
| | | |-file-15.txt : 35 lines, 1688 tokens, 9567 symbols
| | | |-file-16.txt : 23 lines, 1236 tokens, 6990 symbols
| |-file-13.txt : 7 lines, 319 tokens, 1868 symbols
|-directory-h
| |-directory-i
| | |-file-17.txt : 7 lines, 324 tokens, 1819 symbols
| | |-file-18.txt : 25 lines, 1307 tokens, 7487 symbols
| | |-file-19.txt : 25 lines, 1178 tokens, 6782 symbols
| | |-file-20.txt : 27 lines, 1493 tokens, 8398 symbols
|-directory-j
| |-file-21.txt : 0 lines, 0 tokens, 0 symbols
| |-file-22.txt : 19 lines, 961 tokens, 5528 symbols
| |-file-23.txt : 1 lines, 95 tokens, 570 symbols
| |-file-24.txt : 15 lines, 839 tokens, 4882 symbols
|-file-1.txt : 15 lines, 755 tokens, 4276 symbols
|-file-2.txt : 9 lines, 475 tokens, 2741 symbols
```

## Tests
To compile the tests, locate the unit tests folder and run `cmake CMakeLists.txt && make` (gtest library required). To run the test, just type `./run_tests`. 

## Performance
Program performance has been tested on WSL Ubuntu 20.04 with Ryzen 9 5900HX. All results were obtained by running the command `time ./indeer /home THREAD-COUNT` once for each number of threads and should therefore be considered as informational only.

| Time | 1 Thread  | 2 Threads | 4 Threads | 8 Threads | 16 Threads |
| :--- |   :---:   |   :---:   |   :---:   |   :---:   |   :---:    |
| real | 0m11.352s | 0m4.369s  | 0m2.345s  | 0m1.499s  |  0m1.096s  |
| user | 0m10.060s | 0m8.479s  | 0m8.146s  | 0m9.610s  |  0m12.231s |
| sys  | 0m12.550s | 0m3.930s  | 0m2.106s  | 0m1.275s  |  0m1.001s  |