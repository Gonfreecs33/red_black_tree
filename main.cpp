#include  "piecewise_RBT.cpp"
int main() {
    RedBlackTree<DeltaPoint> tree;

    tree.insert({0.0, 2.0});
    tree.insert({3.5, -1.0});
    tree.insert({6.0, 2.5});
    tree.insert({7.0, 0.0});
    tree.insert({8.0, 0.0});
    tree.insert({10.5, 1});
    tree.insert({12, -1});

    tree.printTree();

    cout << "\nEvaluation de f(x) :\n";
    cout << "f(0.0) = " << tree.eval(0.0) << endl;
    cout << "f(4.0) = " << tree.eval(4.0) << endl;
    cout << "f(6.0) = " << tree.eval(6.0) << endl;

    tree.exportFunction("function.txt");

    return 0;
}
