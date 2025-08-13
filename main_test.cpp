#include "RBT_sarah.cpp"
#include <vector>
#include <iostream>
using namespace std;

void testMinFunction(RedBlackTree<DeltaPoint> g, double c) {
    cout << "\n===== Test minfunction avec c = " << c << " =====\n";
    cout << "Avant:\n";
    g.printTree();

    g.minfunction(c);

    cout << "Après:\n" << endl ;
    g.printTree();

    vector<double> testPoints = {0.0, 1.0, 2.0, 2.5, 3.0, 4.0, 5.0, 6.0};
    for (double x : testPoints) {
        cout << "g(" << x << ") = " << g.eval(x) << endl;
    }
}

int main() {
    // Création d’une fonction g en escalier complexe
    RedBlackTree<DeltaPoint> g;
    g.insert({0.0, 2.0});    // départ au-dessus
    g.insert({1.0, -3.0});   // descend
    g.insert({2.0, 4.0});    // remonte
    g.insert({2.5, -4.0});   // redescend
    g.insert({3.0, 1.5});    // remonte
    g.insert({4.0, -1.0});   // descend
    g.insert({5.0, 5.0});    // saute haut
    g.insert({6.0, -6.0});   // descend fort

    // Tests avec plusieurs valeurs de c
    testMinFunction(g, 6);    // seuil très grand (aucune suppression attendue)
    testMinFunction(g, 0);    // seuil à zéro
    testMinFunction(g, -0.5); // seuil négatif
    testMinFunction(g, 2);    // seuil intermédiaire
    testMinFunction(g, -5);   // seuil plus petit que tout

    return 0;
}
