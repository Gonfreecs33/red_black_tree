#include "rbt_new.cpp"

int main() {
    // Création de f : une fonction en escalier
    // RedBlackTree<DeltaPoint> f;
    // f.insert({0.0, 1.0});
    // f.insert({7.0, 0.0});
    RedBlackTree<DeltaPoint> f;
    f.insert({1.0, 0});
    f.insert({7.0, 0});



    // Création de g : une autre fonction en escalier
    RedBlackTree<DeltaPoint> g;
    g.insert({2.0, -1.0});
    g.insert({2.5, 1.0});
    g.insert({3.0, 1.5});
    g.insert({5.0, -1.5});
    g.insert({6.0, -6.5});

    g.minfunction(-0.5);

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
