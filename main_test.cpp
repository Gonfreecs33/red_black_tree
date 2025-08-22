#include "RBT_sarah.cpp"
#include <iostream>



int main() {


   // Création de g : une autre fonction en escalier
   RedBlackTree<DeltaPoint> g;
    g.insert({2.0, -1.0});
    g.insert({2.5, 1.0});
    g.insert({3.0, 1.5});
    g.insert({5.0, -1.5});
    g.insert({6.0, -6.5});


    // Affichage des arbres
    cout << "--- Arbre g ---\n";
    g.printTree();

    // ... insérer des points dans f ...
    auto points = g.to_points_compact_bis(2.1,5.7);
    cout<< "===print des points de la fonction (x,f(x))==="<< endl;
    for (auto& p : points) {

        std::cout << "(" << p.first << ", " << p.second << ")" << std::endl;
    }
    
 
    cout << "\n--- Arbre f + g ---\n";
   g.printTree();

    // Évaluation en quelques points
    cout << "\n--- Évaluation de f + g ---\n";
    vector<double> testPoints = {0.0, 1.5, 2, 2.33, 3.0, 4.33, 4.5 , 5.0, 6.0};

    for (double x : testPoints) {
        cout << "g(" << x << ") = " << g.eval(x) << endl;
    }
    g.exportFunction("rrr.txt");
    return 0;
}
