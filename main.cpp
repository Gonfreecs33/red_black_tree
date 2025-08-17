#include "RBT_sarah.cpp"
#include <iostream>



int main() {
    // Création de f : une fonction en escalier
    // RedBlackTree<DeltaPoint> f;
    // f.insert({0.0, 1.0});
    // f.insert({7.0, 0.0});
    RedBlackTree<DeltaPoint> f;
    f.insert({1.0, 0});
    f.insert({7.0, -5});



    // Création de g : une autre fonction en escalier
   // RedBlackTree<DeltaPoint> g;
    // g.insert({2.0, -1.0});
    // g.insert({2.5, 1.0});
    // g.insert({3.0, 1.5});
    // g.insert({5.0, -1.5});
    // g.insert({6.0, -6.5});

    auto g = cba_profile(-5, 2, 3.5);

    bool t = g.isLessOrEqual(f);

    g.update_cbr_stmin(5, 6, 10, -3, -3);

    cout << " g plus petit que f ?  "<< t << endl;

  //  g.minfunction(-20);
    cout <<"g.evaluate_min()= " << g.evaluate_max(3,3) << endl;
    // Affichage des arbres
    cout << "--- Arbre f ---\n";
    f.printTree();

    cout << "\n--- Arbre g ---\n";
    g.printTree();
    // g.negate();
    // g.negate();
    // f.minus(g);
    

 
    cout << "\n--- Arbre f + g ---\n";
    f.printTree();

    // Évaluation en quelques points
    cout << "\n--- Évaluation de f + g ---\n";
    vector<double> testPoints = {0.0, 1.5, 2, 2.33, 3.0, 4.33, 4.5 , 5.0, 6.0};

    for (double x : testPoints) {
        cout << "f+g(" << x << ") = " << g.eval(x) << endl;
    }
    g.exportFunction("rrr.txt");
    return 0;
}
