#include <iostream>
#include <vector>
#include "../cadical/src/ipasir.h"

// Anzahl Variablen: n
// Anzahl Triple: n * (n-1) * (n-2)
// Anzahl Pythagoranischer Triple: Abhängig von n und zutreffender Triple

struct Triple {
    int a, b, c;
};

int main() {
    int n;
    std::cout << "Geben Sie einen Wert für n ein: ";
    std::cin >> n;
    std::vector<Triple> triples;
    bool usedVars[n] = {false};

    for (int a = 1; a <= n; a++) {
        for (int b = a + 1; b <= n; b++) {
            for (int c = b + 1; c <= n; c++) {
                triples.push_back({a, b, c});
            }
        }
    }

    auto * solver = ipasir_init();

    for (const Triple& triple : triples) {
        if (triple.a * triple.a + triple.b * triple.b == triple.c * triple.c) {
            ipasir_add(solver, triple.a);
            ipasir_add(solver, triple.b);
            ipasir_add(solver, triple.c);
            ipasir_add(solver, 0);

            ipasir_add(solver, -triple.a);
            ipasir_add(solver, -triple.b);
            ipasir_add(solver, -triple.c);
            ipasir_add(solver, 0);

            usedVars[triple.a - 1] = true;
            usedVars[triple.b - 1] = true;
            usedVars[triple.c - 1] = true;
        }
    }

    int result = ipasir_solve(solver);

    if (result == 10) {
        std::cout << "SAT: Lösung gefunden!" << std::endl;
        for (int i = 1; i <= n; i++) {
            int value = ipasir_val(solver, i);
            bool isUsed = usedVars[i-1];
            if (isUsed) {
                if (value > 0) {
                    std::cout << "\033[1;32m" << value << "\033[0m ";
                } else {
                    std::cout << "\033[1;31m" << value << "\033[0m ";
                }
            } else {
                std::cout << value << " ";
            }
        }
        std::cout << "\n";
    } else if (result == 20) {
        std::cout << "UNSAT: Keine Lösung gefunden." << std::endl;
    } else {
        std::cout << "CaDiCal-Fehler: " << result << std::endl;
    }

    ipasir_release(solver);
    return 0;
}
