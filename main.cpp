#include "RBT_sarah.cpp"

int main() {
    // Création de f : une fonction en escalier
    // RedBlackTree<DeltaPoint> f;
    // f.insert({0.0, 1.0});
    // f.insert({7.0, 0.0});
    RedBlackTree<DeltaPoint> f;



    
    RedBlackTree<DeltaPoint> g;
  //  g.insert({0.0, 2.0});    // départ au-dessus
    g.insert({1.0, -3.0});   // descend
    g.insert({2.0, 4.0});    // remonte
    g.insert({2.5, -4.0});   // redescend
    g.insert({3.0, 1.5});    // remonte
    g.insert({4.0, -1.0});   // descend
    g.insert({5.0, 5.0});    // saute haut
    g.insert({6.0, -6.0});   // descend fort


      //g.remove({2.5, 1.0});
     //  g.remove({3.0, 1.5});
     // g.remove({5.0, -1.5});


    cout << "\n--- Arbre g ---\n";
    g.printTree();
    
    f = g.maxWithC(-1);

   // g.minfunction(-1);

    // Affichage des arbres
    // cout << "--- Arbre f ---\n";
    // f.printTree();

    cout << "\n--- Arbre g ---\n";
    g.printTree();
    // g.negate();
    // g.negate();
    // f.minus(g);
    

 
    cout << "\n--- Arbre f + g ---\n";
    f.printTree();

    // Évaluation en quelques points
    cout << "\n--- Évaluation de f + g ---\n";
    vector<double> testPoints = {0.0, 1, 2, 2.5, 3.0, 4, 4.5 , 5.0, 6.0};

    for (double x : testPoints) {
        cout << "f(" << x << ") = " << f.eval(x) << endl;
    }
    f.exportFunction("rrr.txt");
    return 0;
}
