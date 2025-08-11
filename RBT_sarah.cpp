// C++ Program to Implement Red Black Tree
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <functional>
#include <cmath>  // pour fabs()


const double EPSILON = 1e-4;
using namespace std;


enum Color { RED, BLACK };

struct DeltaPoint {
    double x;         // abscisse du point
    double deltaY;    // y_i - y_{i-1}

    bool operator<(const DeltaPoint& other) const {
        return x < other.x;
    }

    bool operator>(const DeltaPoint& other) const {
        return x > other.x;
    }

    bool operator<=(const DeltaPoint& other) const {
        return x <= other.x;
    }

    bool operator==(const DeltaPoint& other) const {
        return fabs(x - other.x) < EPSILON;
    }
};

template <typename T> class RedBlackTree  {
private:


    struct Node {
    T data;
    Color color;
    Node* left;
    Node* right;
    Node* parent;

    explicit Node(T val) : data(val), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
};

    Node* root;

    //  left rotation
    void leftRotate(Node* x) {
        if (x == nullptr || x->right == nullptr)
            return;

        Node* y = x->right;
        x->right = y->left;
        if (y->left != nullptr)
            y->left->parent = x;
        y->parent = x->parent;
        if (x->parent == nullptr)
            root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
        y->left = x;
        x->parent = y;
    }

    //  right rotation
    void rightRotate(Node* y) {
        if (y == nullptr || y->left == nullptr)
            return;

        Node* x = y->left;
        y->left = x->right;
        if (x->right != nullptr)
            x->right->parent = y;
        x->parent = y->parent;
        if (y->parent == nullptr)
            root = x;
        else if (y == y->parent->left)
            y->parent->left = x;
        else
            y->parent->right = x;
        x->right = y;
        y->parent = x;
    }

    // fix violations after inserting a node
    void fixInsert(Node* z) {
        while (z != root && z->parent->color == RED) {
            if (z->parent == z->parent->parent->left) {
                Node* y = z->parent->parent->right;
                if (y != nullptr && y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->right) {
                        z = z->parent;
                        leftRotate(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rightRotate(z->parent->parent);
                }
            } else {
                Node* y = z->parent->parent->left;
                if (y != nullptr && y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        rightRotate(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    leftRotate(z->parent->parent);
                }
            }
        }
        root->color = BLACK;
    }

    // Transplant function used in deletion
    void transplant(Node* u, Node* v) {
        if (u->parent == nullptr)
            root = v;
        else if (u == u->parent->left)
            u->parent->left = v;
        else
            u->parent->right = v;
        if (v != nullptr)
            v->parent = u->parent;
    }

//    delete a node
    void deleteNode(Node* z) {
        if (z == nullptr)
            return;

        Node* y = z;
        Node* x = nullptr;
        Color y_original_color = y->color;

        if (z->left == nullptr) {
            x = z->right;
            transplant(z, z->right);
        } else if (z->right == nullptr) {
            x = z->left;
            transplant(z, z->left);
        } else {
            y = minimum(z->right);
            y_original_color = y->color;
            x = y->right;

            if (y->parent == z) {
                if (x != nullptr)
                    x->parent = y; // Check if x is not nullptr before assigning parent
            } else {
                if (x != nullptr)
                    x->parent = y->parent; // Check if x and y->parent are not nullptr before assigning parent
                transplant(y, y->right);
                if (y->right != nullptr)
                    y->right->parent = y; // Check if y->right is not nullptr before assigning parent
                y->right = z->right;
                if (y->right != nullptr)
                    y->right->parent = y; // Check if y->right is not nullptr before assigning parent
            }
            transplant(z, y);
            y->left = z->left;
            if (y->left != nullptr)
                y->left->parent = y; // Check if y->left is not nullptr before assigning parent
            y->color = z->color;
        }

        if (y_original_color == BLACK && x != nullptr) // Check if x is not nullptr
            fixDelete(x);

        delete z; // Free memory allocated for the deleted node
    }


    // Function to fix violations after deleting a node
    void fixDelete(Node* x) {
        while (x != root && x != nullptr && x->color == BLACK) {
            if (x == x->parent->left) {
                Node* w = x->parent->right;
                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    leftRotate(x->parent);
                    w = x->parent->right;
                }
                if ((w->left == nullptr || w->left->color == BLACK) &&
                    (w->right == nullptr || w->right->color == BLACK)) {
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (w->right == nullptr || w->right->color == BLACK) {
                        if (w->left != nullptr)
                            w->left->color = BLACK;
                        w->color = RED;
                        rightRotate(w);
                        w = x->parent->right;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    if (w->right != nullptr)
                        w->right->color = BLACK;
                    leftRotate(x->parent);
                    x = root;
                }
            } else {
                Node* w = x->parent->left;
                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    rightRotate(x->parent);
                    w = x->parent->left;
                }
                if ((w->right == nullptr || w->right->color == BLACK) &&
                    (w->left == nullptr || w->left->color == BLACK)) {
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (w->left == nullptr || w->left->color == BLACK) {
                        if (w->right != nullptr)
                            w->right->color = BLACK;
                        w->color = RED;
                        leftRotate(w);
                        w = x->parent->left;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    if (w->left != nullptr)
                        w->left->color = BLACK;
                    rightRotate(x->parent);
                    x = root;
                }
            }
        }
        if (x != nullptr)
            x->color = BLACK;
    }
        
    void deleteTree(Node* node) {
        if (node) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }

    // find the minimum node in a subtree
    Node* minimum(Node* node) {
        while (node->left != nullptr)
            node = node->left;
        return node;
    }

    // // print the tree structure (in-order traversal)
    // void printHelper(Node* root, int space) {
    //     constexpr int COUNT = 5;
    //     if (root == nullptr)
    //         return;
    //     space += COUNT;
    //     printHelper(root->right, space);
    //     std::cout << std::endl;
    //     for (int i = COUNT; i < space; i++)
    //         std::cout << " ";
    //     std::cout << root->data << "(" << ((root->color == RED) ? "RED" : "BLACK") << ")" << std::endl;
    //     printHelper(root->left, space);
    // }
   Node* search(Node* node, const T& key) {
        if (!node || node->data == key)
            return node;
        if (key < node->data)
            return search(node->left, key);
        else
            return search(node->right, key);
    }

    void evalHelper(Node* node, double x, double& acc) {
        if (!node) return;
        if (node->data.x <= x) {
            evalHelper(node->left, x, acc);
            acc += node->data.deltaY;
            evalHelper(node->right, x, acc);
        } else {
            evalHelper(node->left, x, acc);
        }
    }

     void printHelper(Node* root, std::string indent, bool last) {
        if (root) {
            std::cout << indent;
            if (last) {
                std::cout << "R----";
                indent += "   ";
            } else {
                std::cout << "L----";
                indent += "|  ";
            }
            std::cout << "(" << root->data.x << ", " << root->data.deltaY << ")"
                 << " [" << (root->color == RED ? "RED" : "BLACK") << "]" << std::endl;
            printHelper(root->left, indent, false);
            printHelper(root->right, indent, true);
        }
    }

public:
    RedBlackTree() : root(nullptr) {}
    ~RedBlackTree() { deleteTree(root); }

    // insert a node
    void insert(T val) {
        Node* newNode = new Node(val);
        Node* y = nullptr;
        Node* x = root;

        while (x != nullptr) {
            y = x;
            if (newNode->data < x->data)
                x = x->left;
            else
                x = x->right;
        }

        newNode->parent = y;
        if (y == nullptr)
            root = newNode;
        else if (newNode->data < y->data)
            y->left = newNode;
        else
            y->right = newNode;

        fixInsert(newNode);
    }

// delete a node by value
    void remove(T val) {
        Node* z = root;
        while (z != nullptr) {
            if (val < z->data)
                z = z->left;
            else if (val > z->data)
                z = z->right;
            else {
                deleteNode(z);
                return;
            }
        }
        std::cout << "Node with value " << val.x << " not found in the tree." << std::endl;
    }

    // // print the tree structure
    // void printTree() {
    //     printHelper(root, 0);
    // }

double eval_in(double x) {
    double result = 0.0;
    evalHelper(root, x, result);
    return result;
}







void accumulateUpTo(Node* node, double x, double& sum) {
    if (!node) return;
    if (node->data.x > x + EPSILON) {
        accumulateUpTo(node->left, x, sum);
    } else {
        accumulateUpTo(node->left, x, sum);
        sum += node->data.deltaY;
        accumulateUpTo(node->right, x, sum);
    }
}

void findBoundingNodes(Node* node, double x, Node*& left, Node*& right) {
    while (node) {
        if (fabs(x - node->data.x) < EPSILON) {
            left = node;
            right = nullptr;
            return;
        } else if (x < node->data.x) {
            right = node;
            node = node->left;
        } else {
            left = node;
            node = node->right;
        }
    }
}

double eval(double x) {
    if (!root) return 0.0;

    double sum = 0.0;
    accumulateUpTo(root, x, sum);

    Node* left = nullptr;
    Node* right = nullptr;
    findBoundingNodes(root, x, left, right);

    if (!right) {
        // x ≈ xi exact match, or x > max
        return sum;
    }

    if (fabs(x - right->data.x) < EPSILON) {
        // x ≈ x{i+1}, ajouter son deltaY complet
        sum += right->data.deltaY;
        return sum;
    }

    if (left) {
        double dx = right->data.x - left->data.x;
        if (dx != 0.0) {
            double fraction = (x - left->data.x) / dx;
            sum += fraction * right->data.deltaY;
        }
    }

    return sum;
}



void sum(const RedBlackTree<T>& g) {
    function<void(typename RedBlackTree<T>::Node*)> process = [&](typename RedBlackTree<T>::Node* node) {
        if (!node) return;
        process(node->left);

        double x = node->data.x;
        cout<< "node : " << x << endl;
        double deltaG = node->data.deltaY;

        Node* match = search(root, node->data);
        std::cout<< "looking for a match !!" << std::endl;
        if (match) {
            std::cout<< "match trouve  !!" << std::endl;
             // Si x existe déjà dans f, on additionne les deltas
            match->data.deltaY += deltaG;
        } else {
            // x n'existe pas dans f, calculer deltaF à x
            std::cout<< "node not matched !!" << std::endl;
            double y = this->eval(x);
            std::cout<< "node not matched a !!" << std::endl;
            
            // Trouver le dernier point de f avant x (i.e., x_prev)
            Node* left = nullptr;
            Node* right = nullptr;
            findBoundingNodes(root, x, left, right);
            std::cout<< "node not matched b !!" << std::endl;

            double yPrev = left ? this->eval(left->data.x) : 0.0;
            std::cout<< "node not matched c !!" << std::endl;
            double deltaF = y - yPrev;
            std::cout<< " eval f = " << y << std::endl;
            std::cout<< " new delta  = " << y -yPrev << std::endl;
            
            double delta_sum =  deltaF + deltaG ;      
            this->insert({x, delta_sum}); //insert new point
            if(right) right->data.deltaY -= delta_sum ; //update delta of next point
            std::cout<< "node not matched d !!" << std::endl;
        }

        process(node->right);
    };

    process(g.root);
}

void minus(const RedBlackTree<T>& g){
    function<void(typename RedBlackTree<T>::Node*)> process = [&](typename RedBlackTree<T>::Node* node) {
        if (!node) return;
        process(node->left);

        double x = node->data.x;
        double deltaG = -node->data.deltaY;

        Node* match = search(root, node->data);
        std::cout<< "looking for a match !!" << std::endl;
        if (match) {
            std::cout<< "match trouve  !!" << std::endl;
             // Si x existe déjà dans f, on additionne les deltas
            match->data.deltaY += deltaG;
        } else {
            // x n'existe pas dans f, calculer deltaF à x
            std::cout<< "node not matched !!" << std::endl;
            double y = this->eval(x);
            std::cout<< "node not matched a !!" << std::endl;
            
            // Trouver le dernier point de f avant x (i.e., x_prev)
            Node* left = nullptr;
            Node* right = nullptr;
            findBoundingNodes(root, x, left, right);
            std::cout<< "node not matched b !!" << std::endl;

            double yPrev = left ? this->eval(left->data.x) : 0.0;
            std::cout<< "node not matched c !!" << std::endl;
            double deltaF = y - yPrev;
            std::cout<< " eval f = " << y << std::endl;
            std::cout<< " new delta  = " << y -yPrev << std::endl;
            
            double delta_sum =  deltaF + deltaG ;

            this->insert({x, delta_sum});
            if(right) right->data.deltaY -= delta_sum ; //update delta of next point
            std::cout<< "node not matched d !!" << std::endl;
        }

        process(node->right);
    };

    process(g.root);
}

void negate(){
    function<void(Node*)> inorder = [&](Node* node) {
        if (!node) return;
        inorder(node->left);
        node->data.deltaY = -node->data.deltaY;
        inorder(node->right);
    };

    inorder(root);
}



void minfunction(double c){

    // DeltaPoint zeroPt;
    // zeroPt.deltaY = 0;
    // zeroPt.x = 0;
    // cout << " search coucouc" << endl;
    // Node* zeroNode = search(root, zeroPt);
    // cout << "post search coucouc" << endl;

    // if (zeroNode) {
    //     // Mise à jour de son delta en fonction de c
    //     if(zeroNode->data.deltaY > c)  zeroNode->data.deltaY = c;
    // } else {
    //     // Calculer deltaY de départ (par ex. c directement)
    //     double y = (c<0)? c:0;
        
    //     this->insert({0.0, y});
    // }


    
    // // === Étape 2 : mettre à jour le dernier point ===
    // Node* last = root;
    // if (last) {
    //     while (last->right) last = last->right;
    //     if (last->data.deltaY > c) last->data.deltaY = c ;  // mise à jour en fonction de c
    // } 

// === Étape 1 : gérer le noeud x = 0.0 ===
    DeltaPoint zeroPt;
    zeroPt.deltaY = 0;
    zeroPt.x = 0;
    Node* zeroNode = search(root, zeroPt);

if (zeroNode) {
    if (zeroNode->data.deltaY > c)  zeroNode->data.deltaY = c;
} else {
    double y0 = (c < 0) ? c : 0; // règle pour le delta initial
    insert({0.0, y0});
    zeroNode = search(root, zeroPt); // on le récupère après insertion
}

// === Mise à jour du nœud qui suit x = 0.0 ===
Node* succ = zeroNode;
if (succ) {
    // Trouver le successeur (le plus petit x > 0.0)
    if (succ->right) {
        succ = succ->right;
        while (succ->left) succ = succ->lef- ;
        // ajustement du delta du suivant
        cout << " successeur x :" <<  succ->data.x << endl;
        succ->data.deltaY -= zeroNode->data.deltaY;
    }
}

// // === Étape 2 : gérer le dernier nœud ===
// Node* last = root;
// Node* prev = nullptr;
// if (last) {
//     while (last->right) {
//         prev = last;
//         last = last->right;
//     }
//     // ajuster le delta du dernier en fonction de prev et c
//     if (prev) {
//         double yPrev = eval(prev->data.x); // valeur avant dernier
//         double newDelta = c - yPrev;
//         last->data.deltaY = newDelta;
//     }
// }


    vector<Node*> toDelete;
    vector<DeltaPoint> toInsert;
    cout << "minfunction execution starts here"<< endl;
    Node* prevNode = nullptr;
    double prevX = 0.0;
    double prevY = 0.0;
    double currentY = 0.0;

    function<void(Node*)> inorder = [&](Node* node) {
        if (!node) return;
        inorder(node->left);

        double x = node->data.x;
        currentY += node->data.deltaY;

        if (prevNode) {
            bool prevAbove = prevY > c + EPSILON;
            bool currAbove = currentY > c + EPSILON;
            cout << "Visiting(current) x=" << node->data.x << ", deltaY=" << node->data.deltaY << ", currentY=" << currentY << endl;
            cout << "Prev x=" << prevX << ", prevY=" << prevY<<  endl;

            // Traversée de c ?
            if ((prevAbove && !currAbove) || (!prevAbove && currAbove)) {
                cout << "coupure found" <<  endl;

                // Interpolation pour trouver l'intersection
                double t = (c - prevY) / (currentY - prevY);
                double xi = prevX + t * (x - prevX);
                double deltaAtXi = (prevY > c + EPSILON)? 0 : c - prevY;
              cout << "Point de coupure x=" << xi << ", deltaXi=" <<deltaAtXi << endl;
                toIns  ert.push_back({xi, deltaAtXi});
               // this->insert({xi, deltaAtXi});


                if ((prevAbove && !currAbove)) {
                    // Ajuster delta du nœud actuel
                    double remainingDelta = currentY - c;
                    node->data.deltaY = remainingDelta;
                    currentY = c + remainingDelta; }

            }
        }

        // Marquer les nœuds au-dessus de c pour suppression
        if (currentY > c + EPSILON) {
            cout << " deletion to be done for " << "node : ("<< node->data.x <<" , "<< node->data.deltaY<< ")"<< endl;
            toDelete.push_back(node);
        }
        cout << "Hello "  << endl;
        prevNode = node;
        prevX = x;
        prevY = currentY;

        inorder(node->right);
    };

    inorder(root);


    if (!toInsert.empty()){
       for (auto& pt : toInsert) {
        insert(pt);  // Fait après le parcours, évite corruption
    }
    }
    cout << "How are you "  << endl;
    // Supprimer les nœuds après le parcours 
    if (!toDelete.empty()) {
        for (Node* node : toDelete) {
            remove(node->data);
        }
    }
}




void exportFunction(const string& filename) {
    vector<pair<double, double>> points;
    double currentY = 0.0;

    function<void(Node*)> inorder = [&](Node* node) {
        if (!node) return;
        inorder(node->left);
        currentY += node->data.deltaY;
        points.push_back({node->data.x, currentY});
        inorder(node->right);
    };

    inorder(root);

    ofstream out(filename);
    if (!out) {
        cerr << "Erreur : impossible d ouvrir le fichier " << filename << endl;
        return;
    }

    for (auto& [x, y] : points) {
        out << x << " " << y << "\n";
    }

    out.close();
    cout << "Fonction exportee vers " << filename << endl;
}



    void printTree() {
        if (!root)
            std::cout << "Tree is empty." << std::endl;
        else
            printHelper(root, "", true);
    }
};
