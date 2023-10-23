#include <iostream>
#include <vector>
#include "../cadical/src/cadical.hpp"
#include <fstream>
#include <string>
#include <dirent.h>
#include <unistd.h>
#include <limits.h>
#include <sstream>

struct Sudoku {
    int n;
    std::vector<std::vector<int>> sudokuBoard;
};

Sudoku sudoku;

void convertSudoku(std::ifstream& file) {
    if (file.is_open()) {
        std::string line;

        while (std::getline(file, line)) {
            std::vector<int> numbers;
            std::istringstream iss(line);
            int number;

            if (line.length() == 1) {
                iss >> number;
                sudoku.n = number;
            } else {
                while (iss >> number) {
                    numbers.push_back(number);
                }

                sudoku.sudokuBoard.push_back(numbers);
            }
        }
        file.close();
    }
}

void solveSudoku() {
    int n = sudoku.n;
    int m = n * n;
    int vars = m * m * m;
    int clauses = 0;

    CaDiCaL::Solver * solver = new CaDiCaL::Solver;

    for (int r = 1; r <= m; ++r) {
            for (int c = 1; c <= m; ++c) {
                if (sudoku.sudokuBoard[r - 1][c - 1] != 0) {
                    int val = sudoku.sudokuBoard[r - 1][c - 1];
                    int var = (r - 1) * m * m + (c - 1) * m + val;
                    solver->add(var);
                    solver->add(0);
                }
            }
        }
    int result = solver->solve();

    if (result == 10) {
        std::cout << "SAT: Lösung gefunden!" << std::endl;
        for (int i = 1; i <= n; i++) {
            int value = solver->val(i);
            std::cout << value << " ";
        }
        std::cout << "\n";
    } else if (result == 20) {
        std::cout << "UNSAT: Keine Lösung gefunden." << std::endl;
    } else {
        std::cout << "CaDiCal-Fehler: " << result << std::endl;
    }
    delete solver;
}

int main() {
    char buffer[PATH_MAX];
    std::string fileName;
    std::cout << "Geben Sie einen Dateinamen ein: ";
    std::cin >> fileName;
    std::string currentPath = getcwd(buffer, sizeof(buffer));
    std::string folderPath = currentPath + "/exercise_1/sudokus";    
    std::string filePath = folderPath + "/" + fileName;
    std::ifstream file(filePath);
    convertSudoku(file);
    solveSudoku();
    return 0;
}