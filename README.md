# SAT Solving

## Installation
1. Clone CaDiCal into root folder
```
cd SAT\ Solving/

git clone https://github.com/arminbiere/cadical.git
```

2. Build CaDiCal library
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

