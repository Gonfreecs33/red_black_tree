#include "RBT_sarah.cpp"
#include <iostream>



int main() {
    // Création de f : une fonction en escalier
    // RedBlackTree<DeltaPoint> f;
    // f.insert({0.0, 1.0});
    // f.insert({7.0, 0.0});
    RedBlackTree<DeltaPoint> f1;
    f1.insert({1, 0});
    f1.insert({22, -10});
    // f1.insert({3, 0});
    // f1.insert({5, -10});

    RedBlackTree<DeltaPoint> f2;
    f2.insert({0, 0});
    f2.insert({6, -10});

    // f2.insert({4, 1});
    // f2.insert({6, 1});

 //f1.negate();
    f2.sum(f1);

    // Création de g : une autre fonction en escalier
   // RedBlackTree<DeltaPoint> g;
    // g.insert({2.0, -1.0});
    // g.insert({2.5, 1.0});
    // g.insert({3.0, 1.5});
    // g.insert({5.0, -1.5});
    // g.insert({6.0, -6.5});

  //   auto g = cba_profile(-5, 2, 3.5);

  //   bool t = g.isLessOrEqual(f1);

  //   g.update_cbr_stmin(5, 6, 10, -3, -3);

  //   cout << " g plus petit que f ?  "<< t << endl;

  // //  g.minfunction(-20);
  //   cout <<"g.evaluate_min()= " << g.evaluate_max(3,3) << endl;
    // Affichage des arbres
    cout << "--- Arbre f1 ---\n";
    f1.printTree();

    cout << "\n--- Arbre f2 ---\n";
    f2.printTree();
  //   // g.negate();
  //   // g.negate();
  //   // f.minus(g);
    

    // ... insérer des points dans f ...
    auto points = f1.to_points();
    cout<< "===print des points de la fonction (x,f(x))==="<< endl;
    for (auto& p : points) {

        std::cout << "(" << p.first << ", " << p.second << ")" << std::endl;
    }
    
 
    cout << "\n--- Arbre f + g ---\n";
    f1.printTree();

    // Évaluation en quelques points
    cout << "\n--- Évaluation de f + g ---\n";
    vector<double> testPoints = {0.0, 1.5, 2, 2.33, 3.0, 4.33, 4.5 , 5.0, 6.0,22};

    for (double x : testPoints) {
        cout << "f+g(" << x << ") = " << f2.eval(x) << endl;
    }
    f2.exportFunction("rrr.txt");
    return 0;
}
