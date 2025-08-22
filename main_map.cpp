
// C++ Program to Implement Red Black Tree
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <functional>
#include <cmath>  // pour fabs()
#include "piecewise_map.cpp" 



void printFunction(const PiecewiseLinearFunction& f, const std::string& name) {
    std::cout << "--- " << name << " ---" << std::endl;
    std::cout << "f(" << 0.0 << ") = " << f.evaluate(0.0) << std::endl;
    std::cout << "f(" << 1.0 << ") = " << f.evaluate(1.0) << std::endl;
    std::cout << "f(" << 2.0 << ") = " << f.evaluate(2.0) << std::endl;
    std::cout << "f(" << 3.0 << ") = " << f.evaluate(3.0) << std::endl;
    std::cout << "f(" << 4.0 << ") = " << f.evaluate(4.0) << std::endl;
    std::cout << "f(" << 5.0 << ") = " << f.evaluate(5.0) << std::endl;
    std::cout << "f(" << 6.0 << ") = " << f.evaluate(6.0) << std::endl;
    std::cout << std::endl;
}

int main() {
    std::cout << "--- CREATION ET EVALUATION ---" << std::endl;
    PiecewiseLinearFunction f(1); // f(0) = 5
 //   f.addBreakpoint(1.0, 0.0); // Ajout d'un saut de +3 à x=2
  //  f.addBreakpoint(7.0, -10); // Ajout d'un saut de -1 à x=5
    printFunction(f, "Fonction f");

    std::cout << "--- OPERATIONS SUR LES FONCTIONS ---" << std::endl;
    PiecewiseLinearFunction g; // g(0) = 0
    g.addBreakpoint(3, 0);
    g.addBreakpoint(5, -10);
    g.addBreakpoint(7, 10);
    printFunction(g, "Fonction g");

    g.sum(f);
    printFunction(g, "Fonction f+g");

    std::cout << "--- MIN/MAX AVEC UNE CONSTANTE ---" << std::endl;
    PiecewiseLinearFunction f2(2);
    f2.addBreakpoint(2.0, 5);
    f2.addBreakpoint(4.0, -7.0);
    f2.addBreakpoint(6.0, -5.0);
    printFunction(f2, "Fonction f2 (avant min)");
    f2.maxfunction(5);
    printFunction(f2, "Fonction min(f2, 2.5)");

    std::cout << "--- EVALUATION MIN/MAX SUR UN INTERVALLE ---" << std::endl;
    std::cout << "Min de f2 sur [0, 5]: " << f2.evaluate_min(0, 5) << std::endl;
    std::cout << "Max de f2 sur [0, 5]: " << f2.evaluate_max(0, 5) << std::endl;
    std::cout << std::endl;

    std::cout << "--- TEST DES UPDATES CBR ---" << std::endl;
    PiecewiseLinearFunction cbr_func;
    cbr_func.addBreakpoint(5.0, 10.0);
    cbr_func.addBreakpoint(10.0, -10.0);
    printFunction(cbr_func, "Fonction CBR initiale");
    
    // Exemple d'utilisation de update_cbr_stmin
    std::cout << "Mise a jour cbr_stmin..." << std::endl;
    double stmin_old = 5.0, stmin = 7.0, ctmin = 10.0;
    double cap_min = -100.0, cap_max = 100.0;
    cbr_func.update_cbr_stmin(stmin_old, stmin, ctmin, cap_min, cap_max);
    printFunction(cbr_func, "Apres update_cbr_stmin");

    std::cout << "--- EXPORTATION ---" << std::endl;
    g.exportFunction("cbr_function.txt");

    return 0;
}