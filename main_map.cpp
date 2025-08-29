
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
    std::cout << "f(" << 2 << ") = " << f.evaluate(2) << std::endl;
    std::cout << "f(" << 4 << ") = " << f.evaluate(4) << std::endl;
    std::cout << "f(" << 10 << ") = " << f.evaluate(10) << std::endl;
    std::cout << "f(" << 20 << ") = " << f.evaluate(20) << std::endl;
    std::cout << "f(" << 28 << ") = " << f.evaluate(28) << std::endl;
    std::cout << "f(" << 30 << ") = " << f.evaluate(30) << std::endl;
    std::cout << std::endl;
}

int main() {
    std::cout << "--- CREATION ET EVALUATION ---" << std::endl;
    PiecewiseLinearFunction f(1); // f(0) = 5
 //   f.addBreakpoint(1.0, 0.0); // Ajout d'un saut de +3 à x=2
  //  f.addBreakpoint(7.0, -10); // Ajout d'un saut de -1 à x=5
    printFunction(f, "Fonction f");

    std::cout << "--- OPERATIONS SUR LES FONCTIONS ---" << std::endl;
    PiecewiseLinearFunction g1; // g(0) = 0
    g1.addBreakpoint(2, 0);
    g1.addBreakpoint(4, -10);

    PiecewiseLinearFunction g2; // g(0) = 0
    g2.addBreakpoint(20, 0);
    g2.addBreakpoint(22, -10);

    PiecewiseLinearFunction f1;
    f1.addBreakpoint(0, 0);
    f1.addBreakpoint(10, 60);

    PiecewiseLinearFunction f2; 
    f2.addBreakpoint(20, 0);
    f2.addBreakpoint(28, 48);





    PiecewiseLinearFunction levelmin;
    PiecewiseLinearFunction levelmax;
    PiecewiseLinearFunction totalmin;
    PiecewiseLinearFunction totalmax;
    levelmin.sum(g1);
    levelmin.sum(f2);
    totalmin.sum(g1);
    totalmin.sum(f2);
    levelmax.sum(g2);
    levelmax.sum(f1);
    totalmax.sum(g2);
    totalmax.sum(f1);


    PiecewiseLinearFunction kmin = levelmin;
    PiecewiseLinearFunction kmax = levelmax;

    totalmax.negate();
    totalmin.negate(); 

    kmin.sum(totalmax);
    kmax.sum(totalmin);


    kmin.sum(f1);
    kmax.sum(f2);



    printFunction(kmin, " kmin ");
    
    printFunction(kmax, " kmax ");

    std::cout << "--- EXPORTATION ---" << std::endl;
    kmin.exportFunction("kmin.txt");
    kmax.exportFunction("kmax.txt");
    return 0;
}