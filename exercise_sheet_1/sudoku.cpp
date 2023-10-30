#include <iostream>
#include <vector>
#include "../cadical/src/cadical.hpp"
#include <fstream>
#include <string>
#include <dirent.h>
#include <unistd.h>
#include <limits.h>
#include <sstream>
#include <cmath>


struct Sudoku {
    int dimension;
    int lenght;
    int size;
    std::vector<std::vector<int>> board;
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
                sudoku.dimension = number;
                sudoku.lenght = number * number;
                sudoku.size = sudoku.lenght * sudoku.lenght;
            } else {
                while (iss >> number) {
                    numbers.push_back(number);
                }

                sudoku.board.push_back(numbers);
            }
        }
        file.close();
    }
}

int value(int row, int column, int cellNumber) {
    
    return (sudoku.size * (row - 1) +  sudoku.lenght * (column - 1) + cellNumber);
}

void solveSudoku() {
    CaDiCaL::Solver * solver = new CaDiCaL::Solver;    

    for (int row = 1; row <= sudoku.lenght; ++row) {
        for (int column = 1; column <= sudoku.lenght; ++column) {

            // ensure that each cell has at least one number
            for (int cellNumber = 1; cellNumber <= sudoku.lenght; ++cellNumber) {
                solver->add(value(row, column, cellNumber));

                if (cellNumber == sudoku.lenght) {
                    solver->add(0);
                }
            }
            // ensure each cell has only one number
            for (int cellNumber = 1; cellNumber <= sudoku.lenght; ++cellNumber) {
                for (int nextNumber = cellNumber + 1; nextNumber <= sudoku.lenght; ++nextNumber) {
                    solver->add(-value(row, column, cellNumber));
                    solver->add(-value(row, column, nextNumber));
                    solver->add(0);
                }
            }
        }
    }

    // Clauses for each row cell has different number
    for (int row = 1; row <= sudoku.lenght; ++row) {
        for (int column = 1; column <= sudoku.lenght; ++column) {
            for (int cellNumber = 1; cellNumber <= sudoku.lenght; ++cellNumber){
                for (int nextCellNumber = column + 1; nextCellNumber <= sudoku.lenght; ++nextCellNumber) {
                    solver->add(-value(row, column, cellNumber));
                    solver->add(-value(row, nextCellNumber, cellNumber));
                    solver->add(0);
                }
            }
        }
    }

    // Clauses for each column cell has different number
    for (int column = 1; column <= sudoku.lenght; ++column) {
        for (int row = 1; row <= sudoku.lenght; ++row) {
            for (int number = 1; number <= sudoku.lenght; ++number){
                for (int nextCell = row + 1; nextCell <= sudoku.lenght; ++nextCell) {
                    solver->add(-value(row, column, number));
                    solver->add(-value(nextCell, column, number));
                    solver->add(0);
                }
            }
        }
    }

    // TODO: Clauses for each block
    for (int blockRow = 0; blockRow < sudoku.lenght; blockRow += sudoku.dimension) {
        for (int blockColumn = 0; blockColumn < sudoku.lenght; blockColumn += sudoku.dimension) {
            for (int cellNumber = 1; cellNumber <= sudoku.lenght; ++cellNumber) {
                for (int row = 1; row <= sudoku.dimension; ++row) {
                    for (int column = 1; column <= sudoku.dimension; ++column) {
                        for (int nextColumn = column + 1; nextColumn <= sudoku.dimension; ++nextColumn) {
                            solver->add(-value(blockRow + row, blockColumn + column, cellNumber));
                            solver->add(-value(blockRow + row, blockColumn + nextColumn, cellNumber));
                            solver->add(0);
                        }
                        for (int nextRow = row + 1; nextRow <= sudoku.dimension; ++nextRow) {
                            for (int nextColumn = 1; nextColumn <= sudoku.dimension; ++nextColumn) {
                                solver->add(-value(blockRow + row, blockColumn + column, cellNumber));
                                solver->add(-value(blockRow + nextRow, blockColumn + nextColumn, cellNumber));
                                solver->add(0);
                            }
                        }
                    }
                }
            }
        }
    }

    //Clauses for existing numbers in the given sudoku
    for (int row = 1; row <= sudoku.lenght; ++row) {
        for (int column = 1; column <= sudoku.lenght; ++column) {
            int cellValue = sudoku.board[row - 1][column - 1];

            if (cellValue != 0) {
                solver->add(value(row, column, cellValue));
                solver->add(0);
            }
        }
    }
    int result = solver->solve();

    if (result == 10) {
        std::cout << "SAT: Lösung gefunden!" << std::endl;
        
        for (int row = 1; row <= sudoku.lenght; ++row) {
            for (int column = 1; column <= sudoku.lenght; ++column) {
                int correctValue;

                for (int number = 1; number <= sudoku.lenght; ++number) {
                    int var = solver->val(value(row, column, number));

                    if (var > 0) {
                        correctValue = var % sudoku.lenght;

                        if (correctValue == 0) {
                            correctValue = sudoku.lenght;
                        }
                        break;
                    }
                }
                std::cout << correctValue << " ";
                sudoku.board[row -1][column - 1] = correctValue;
            }
            std::cout << std::endl;
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
    //std::string fileName = "puzzle03c.sudoku";
    std::string fileName;
    std::cout << "Geben Sie einen Dateinamen ein: ";
    std::cin >> fileName;
    std::string currentPath = getcwd(buffer, sizeof(buffer));
    std::string folderPath = currentPath + "/exercise_sheet_1/sudokus";    
    std::string filePath = folderPath + "/" + fileName;
    std::ifstream file(filePath);
    convertSudoku(file);
    solveSudoku();
    return 0;
}