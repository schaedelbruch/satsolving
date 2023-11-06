# SAT Solving

## Pre-requirements
- GCC

## Installation
1. Clone this repository
```
git clone https://github.com/schaedelbruch/satsolving.git
```

2. Clone CaDiCal into the root folder
```
cd satsolving

git clone https://github.com/arminbiere/cadical.git
```

3. Build CaDiCal library
```
cd cadical

./configure && make
```

## Use CaDiCal SAT Solver
This section asumes, that cadical is located at the root folder of the project. 
1. Include cadical header into the .cpp program
```
#include "../cadical/src/cadical.hpp"
```
2. Compile the program
```
g++ -o my_program path/to/my_program.cpp -I./cadical/src -L./cadical/build -l:libcadical.a
```
3. Run the program
```
./my_program
```

## Exercise Sheet 1
To compile and run the programs, it is necessary to install CaDiCal (follow the steps above).
### Pythagorean Triples
1. Head to the project root
```
cd project/root/
```
2. Compile the Code
```
g++ -o pythagorean_triples exercise_sheet_1/pythagorean_triples.cpp -I./cadical/src -L./cadical/build -l:libcadical.a

or

g++ -o pythagorean_triples exercise_sheet_1/pythagorean_triples_ipasir.cpp -I./cadical/src -L./cadical/build -l:libcadical.a
```
3. Run the programm
```
./pythagorean_triples
```

The program requires an input of a number n for the length of the sequence to be checked, for example 1000. After the calculation, a coloring of the corresponding numbers is displayed. Negative variables in red, positive in green and all invalid or unused variables/numbers in white.

### Sudoku Solver
1. Place the Sudoku-Benchmarks folder unzipped in satsolving/exercise_sheet_1/
2. Compile the Code
```
g++ -o sudoku_solver exercise_sheet_1/sudoku.cpp -I./cadical/src -L./cadical/build -l:libcadical.a
```
3. Run the programm
```
./sudoku_solver puzzle03a.sudoku
```