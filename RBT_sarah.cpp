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


template <typename T> class RedBlackTree;

RedBlackTree<DeltaPoint> delta_profile(double gap, double a, double b, double c);
RedBlackTree<DeltaPoint> cba_profile(double cap, double a, double b);



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

    //Fonction de clonage (copie profonde)
    Node* cloneTree(Node* node, Node* parent = nullptr) {
        if (!node) return nullptr;
        Node* n = new Node(node->data);
        n->color = node->color;
        n->parent = parent;
        n->left = cloneTree(node->left, n);
        n->right = cloneTree(node->right, n);
        return n;
    }

    Node* successor(Node* node) {
        if (!node) return nullptr;
        if (node->right) {
            Node* curr = node->right;
            while (curr->left) curr = curr->left;
            return curr;
        }
        Node* parent = node->parent;
        while (parent && node == parent->right) {
            node = parent;
            parent = parent->parent;
        }
        return parent;
    }

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
    ~RedBlackTree() { deleteTree(root); root = nullptr; }


    // Constructeur de copie (utilise cloneTree)
    RedBlackTree(const RedBlackTree& other) : root(nullptr) {   
        cout << " clone tree is called" << endl;
        root = cloneTree(other.root, nullptr);
    }

    //swap helper
    void swap(RedBlackTree& other) noexcept {
        std::swap(root, other.root);
    }

    // Opérateur d’affectation (copy-and-swap)
    RedBlackTree& operator=(RedBlackTree other) { // copie locale
        swap(other);
        return *this;
    }


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
    // void remove(T val) {
    //     Node* z = root;
    //     while (z != nullptr) {
    //         if (val < z->data)
    //             z = z->left;
    //         else if (val > z->data)
    //             z = z->right;
    //         else {
    //             deleteNode(z);
    //             std::cout << "Node with value " << val.x << " remove from the tree." << std::endl;
    //             return;
    //         }
    //     }
    //     std::cout << "Node with value " << val.x << " not found in the tree." << std::endl;
    // }

    void remove(const T& val) {
        Node* z = search(root, val); // utilise operator== (EPSILON)
        if (z) {
            deleteNode(z);
            std::cout << "Node with value " << val.x << " remove from the tree." << std::endl;
        } else {
            std::cout << "Node with value " << val.x << " not found in the tree." << std::endl;
        }
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







void accumulateUpTo(Node* node, double x, double& sum) const{
    if (!node) return;
    if (node->data.x > x + EPSILON) {
        accumulateUpTo(node->left, x, sum);
    } else {
        accumulateUpTo(node->left, x, sum);
        sum += node->data.deltaY;
        accumulateUpTo(node->right, x, sum);
    }
}

void findBoundingNodes(Node* node, double x, Node*& left, Node*& right) const{
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

double eval(double x) const {
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

double eval_delta(double x) const {
    DeltaPoint probe {x, 0.0};

    // enlever le const pour pouvoir utiliser search
    auto* self = const_cast<RedBlackTree<T>*>(this);

    Node* match = self->search(self->root, probe);
    if (match) {
        return match->data.deltaY;
    } else {
        Node* left = nullptr;
        Node* right = nullptr;
        self->findBoundingNodes(self->root, x, left, right);

        double y = this->eval(x);   // eval peut rester const
        double yPrev = left ? this->eval(left->data.x) : 0.0;
        return y - yPrev;
    }
}


// auto g_points = g.to_points();

// for (size_t i = 0; i < g_points.size(); ++i) {
//     auto& g_point = g_points[i];
//     std::cout << "Point courant : (" << g_point.first << ", " << g_point.second << ")\n";

//     // Précédent
//     if (i > 0) {
//         auto& prev = g_points[i - 1];
//         std::cout << "   Précédent : (" << prev.first << ", " << prev.second << ")\n";
//     } else {
//         std::cout << "   Aucun précédent (c'est le premier)\n";
//     }

//     // Suivant
//     if (i + 1 < g_points.size()) {
//         auto& next = g_points[i + 1];
//         std::cout << "   Suivant : (" << next.first << ", " << next.second << ")\n";
//     } else {
//         std::cout << "   Aucun suivant (c'est le dernier)\n";
//     }
// }

void sum(const RedBlackTree& g) {
    auto g_points = g.to_points_delta();
    if (g_points.empty()) return;

    double xg_min = g_points.front().first;
    double xg_max = g_points.back().first;

    auto f_points = this->to_points_compact_bis(xg_min, xg_max);

    size_t i = 0, j = 0;

    std::cout << "=== Starting sum ===\n";
    std::cout << "g_points size: " << g_points.size() << ", f_points size: " << f_points.size() << "\n";

    // fusion parallèle
    while (i < g_points.size() || j < f_points.size()) {
        double x;
        bool take_g = false, take_f = false;

        if (i < g_points.size() && (j >= f_points.size() || g_points[i].first < f_points[j].first)) {
            x = g_points[i].first;
            take_g = true;
            std::cout << "Taking point from g_points: x = " << x << " (i = " << i << ")\n";
        } else if (j < f_points.size() && (i >= g_points.size() || f_points[j].first < g_points[i].first)) {
            x = f_points[j].first;
            take_f = true;
            std::cout << "Taking point from f_points: x = " << x << " (j = " << j << ")\n";
        } else { // même abscisse
            x = g_points[i].first;
            take_g = take_f = true;
            std::cout << "Points coincide: x = " << x << " (i = " << i << ", j = " << j << ")\n";
        }

        // calcul des deltas
        double deltaF = this->eval_delta(x);
        double deltaG = g.eval_delta(x);
        double delta_sum = deltaF + deltaG;

        std::cout << "deltaF = " << deltaF << ", deltaG = " << deltaG 
                  << ", delta_sum = " << delta_sum << "\n";

        // --- mise à jour du noeud suivant (right) ---
        Node* left = nullptr;
        Node* right = nullptr;
        findBoundingNodes(root, x+0.01, left, right);

        // insérer ou mettre à jour dans f
        DeltaPoint probe {x, 0.0};
        Node* match = search(root, probe);

        if (match) {
            match->data.deltaY = delta_sum;
            std::cout << "Updated existing node in f: x = " << x << ", new deltaY = " << match->data.deltaY << "\n";
        } else {
            this->insert({x, delta_sum});
            std::cout << "Inserted new node in f: x = " << x << ", deltaY = " << delta_sum << "\n";
        }



        if (right) {
            right->data.deltaY -= deltaG;
            std::cout << "Updated right node: x = " << right->data.x 
                      << ", new deltaY = " << right->data.deltaY << "\n";
        }

        // avancer les pointeurs
        if (take_g) i++;
        if (take_f) j++;
    }


     
       std::cout << "===on s'occupe du dernier noeud ===\n";

        Node* left = nullptr;
        Node* right = nullptr;
        findBoundingNodes(root, xg_max+0.01, left, right);

        if (right) {
            double xright = right->data.x;
            double F = this->eval(xright);
            double G = g.eval(xright);
            double sum = F + G;
            double yprevf = (left)? this->eval(left->data.x):0;
            double delta_sum = sum - yprevf;
            right->data.deltaY = delta_sum ;
            std::cout << "Updated right node: x = " << right->data.x 
                      << ", new deltaY = " << right->data.deltaY << "\n";
        }


    std::cout << "=== Sum completed ===\n";
}


// // méthode sum : f = f + g
// void sum(const RedBlackTree& g) {
//      // ... insérer des points dans f ...
//     auto g_points = g.to_points_delta();
//     double xg_max = g_points.back().first;
//     double xg_min = g_points[0].first; 
//     auto f_points  = this->to_points_compact_bis(xg_min,xg_max);
//     double delta_sum = 0;
//     int j = 0;

//     for (size_t i = 0; i < g_points.size(); ++i) {

//         auto& g_point = g_points[i];
//         double x = g_point.first;

//         auto& f_point = f_points[j]; 

//         if (f_points.empty()) {
//             std::cout << "WARNING: f_points is vide" << std::endl;
//             return;
//         }
//         if (j < 0 || j >= f_points.size()) {
//             std::cout << "WARNING: j hors bornes (j=" << j 
//                       << ", taille=" << f_points.size() << ")" << std::endl;
//             return;
//         }
        
//        // auto& f_point = f_points[j];
//         std::cout << "f_point.first = " << f_point.first << std::endl;
//         cout << " f_point first = " <<  f_point.first<< endl;
//         double xf = f_point.first;//f_point.first;
//         auto& nextg_point = g_points[i+1];


//         cout<< "node : " << x << endl;
//         double deltaG = g_point.second;
//         DeltaPoint probe {x, 0.0};

//         // Trouver le dernier point de f avant x (i.e., x_prev)
//         Node* left = nullptr;
//         Node* right = nullptr;
//         findBoundingNodes(root, x, left, right);

//         Node* match = search(root, probe);
//         std::cout<< "looking for a match !!" << std::endl;
//         if (match) {
//             std::cout<< "match trouve  !!" << std::endl;
//              // Si x existe déjà dans f, on additionne les deltas
//             match->data.deltaY += deltaG;
//             delta_sum = match->data.deltaY;
//         } else {

//             // x n'existe pas dans f, calculer deltaF à x
//             std::cout<< "node not matched !!" << std::endl;
//             double y = this->eval(x);
//             std::cout<< "evaluation de f en x = "<< x<< " , f(x) = "<< y << std::endl;

//             double yPrev = left ? this->eval(left->data.x) : 0.0;
//             std::cout<< "yprev de f ="<< yPrev << std::endl;
//             double deltaF = y - yPrev;

//             std::cout<< " new deltaF  = " << y -yPrev << std::endl;

//             delta_sum =  deltaF + deltaG ;      
//             this->insert({x, delta_sum}); //insert new point

//          }

//         // // Trouver le dernier point de f avant x (i.e., x_prev)
//         // Node* left = nullptr;
//         // Node* right = nullptr;
//         // findBoundingNodes(root, x, left, right);
//         if(right){ 
//             // Suivant
//             cout << "segmentation ? a" << endl;
//             std::cout<< " right x = " << right->data.x << " , deltay  =  "<< right->data.deltaY  << std::endl;
//             right->data.deltaY -= delta_sum ; //update delta of next point
//             std::cout<< "delta right update = "<<  right->data.deltaY << std::endl;
            
//         }

//         bool inter_point  =  true ;
//         while(inter_point){
//             std::cout<< "coucou 0 " << std::endl;


//             std::cout<< "coucou 1.0 " << std::endl;
//             if(xf < nextg_point.first){ 
//                 double yg = g.eval(xf);
//                 std::cout<< "coucou 1 " << std::endl;

//                 // Trouver le dernier point de f avant x (i.e., x_prev)
//                 Node* left1 = nullptr;
//                 Node* right1 = nullptr;
//                 findBoundingNodes(g.root, xf, left1, right1);
//                 std::cout<< "coucou 1 " << std::endl;


//                 double ygPrev = left1 ? g.eval(left1->data.x) : 0.0;
//                 std::cout<< "coucou 2 " << std::endl;

//                 std::cout<< "ygprev de g ="<< ygPrev << std::endl;
//                 double deltaG = yg - ygPrev;

//                 std::cout<< " new deltaF  = " << yg -ygPrev << std::endl;
//                 double deltaf = f_point.second;
//                 delta_sum =  deltaf+ deltaG ;  
//                 DeltaPoint probe1 {xf, 0.0};
//                 j+=1;
//                 Node* match1 = search(root, probe1);
//                 if(match1){
//                     match1->data.deltaY = delta_sum;
                   

//                     if(right){ 
//                         // Suivant
//                         cout << "segmentation ? a" << endl;
//                         std::cout<< " right x = " << right->data.x << " , deltay  =  "<< right->data.deltaY  << std::endl;
//                         right->data.deltaY -= delta_sum ; //update delta of next point
//                         std::cout<< "delta right update = "<<  right->data.deltaY << std::endl;
                        
//                     }


//                 }
           

//             }else{inter_point = false;}
              




//         //std::cout << "(" << p.first << ", " << p.second << ")" << std::endl;
//     }

// }

// }

// void sum(const RedBlackTree<T>& g) {
//     function<void(typename RedBlackTree<T>::Node*)> process = [&](typename RedBlackTree<T>::Node* node) {
//         if (!node) return;
//         process(node->left);

//         double x = node->data.x;
//         cout<< "node : " << x << endl;
//         double deltaG = node->data.deltaY;

//         Node* match = search(root, node->data);
//         std::cout<< "looking for a match !!" << std::endl;
//         if (match) {
//             std::cout<< "match trouve  !!" << std::endl;
//              // Si x existe déjà dans f, on additionne les deltas
//             match->data.deltaY += deltaG;
//         } else {
//             // x n'existe pas dans f, calculer deltaF à x
//             std::cout<< "node not matched !!" << std::endl;
//             double y = this->eval(x);
//             std::cout<< "evaluation de f en x = "<< x<< " , f(x) = "<< y << std::endl;
            
//             // Trouver le dernier point de f avant x (i.e., x_prev)
//             Node* left = nullptr;
//             Node* right = nullptr;
//             findBoundingNodes(root, x, left, right);

//             double yPrev = left ? this->eval(left->data.x) : 0.0;
//             std::cout<< "yprev de f ="<< yPrev << std::endl;
//             double deltaF = y - yPrev;

//             std::cout<< " new deltaF  = " << y -yPrev << std::endl;
            
//             double delta_sum =  deltaF + deltaG ;      
//             this->insert({x, delta_sum}); //insert new point
//             if(right){ 

//                 Node* leftg = nullptr;
//                 Node* rightg = nullptr;
//                 findBoundingNodes(g.root, x, leftg, rightg);
//                 cout << "segmentation ? " << endl;
//                 if(rightg){
//                     cout << "segmentation ? a" << endl;
//                         std::cout<< " right x = " << right->data.x << " , deltay  =  "<< right->data.deltaY  << std::endl;
//                         right->data.deltaY -= delta_sum ; //update delta of next point
//                         std::cout<< "delta right update = "<<  right->data.deltaY << std::endl;
//                     }else{
//                         cout << "segmentation ? b " << endl;
//                         double gx = g.eval(right->data.x);
//                         cout << "segmentation ? c " << endl;
//                         double fx = this->eval(right->data.x);

//                         right->data.deltaY = gx + fx - g.eval(x)  ;

//                     }
                 
//                             }
//             std::cout<< " insertion x = " << x << " , deltay  =  "<< delta_sum  << std::endl;
//         }

//         process(node->right);
//     };

//     process(g.root);
// }

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

    // === Étape 1 : gérer le noeud x = 0.0 ===
    DeltaPoint zeroPt;
    zeroPt.deltaY = 0;
    zeroPt.x = 0;
    double y_prev = 0;
    Node* zeroNode = search(root, zeroPt);

    if (zeroNode) {
        y_prev  =  zeroNode->data.deltaY;
        if (zeroNode->data.deltaY > c)  zeroNode->data.deltaY = c;
    } else {

        y_prev  = 0;
        double y0 = (c < 0) ? c : 0; // règle pour le delta initial
        this->insert({0.0, y0});
        zeroNode = search(root, zeroPt); // on le récupère après insertion
    }


    double xprev = zeroNode->data.x;
    cout<< xprev << endl;
    // Trouver le dernier point de f avant x (i.e., x_prev)

    // === Trouver le nœud suivant (qui est apres x = 0.0) et le mettre a jour ===
    Node* right = successor(zeroNode);
    //cout << "fist case zero node Visiting(current) x=" << right->data.x << ", deltaY=" << right->data.deltaY << ", currentY=" << this->eval_in(right->data.x)<< endl;

    if(right){                        
                bool prevabove = y_prev  > c ;
                double x_current = right->data.x;
                double y_current = y_prev + right->data.deltaY;
                bool currunder =  y_current < c ;
                cout << "fist case zero node Visiting(current) x=" << right->data.x << ", deltaY=" << right->data.deltaY << ", currentY=" << y_current<< endl;
            //   cout << "Prev x=" << prevX << ", prevY=" << prevY<<  endl;

                // Traversée de c ?
                if ((prevabove && currunder)) {
                    cout << "coupure found first node case" <<  endl;


                    // Interpolation pour trouver l'intersection
                    double t = (c - y_prev) / (y_current - y_prev);
                    double xi = xprev + t * (x_current - xprev);
                    double deltaAtXi =  0;
                // cout << "Point de coupure x=" << xi << ", deltaXi=" <<deltaAtXi << endl;
                    this->insert({xi, deltaAtXi});

                    // Ajuster delta du nœud actuel
                    double remainingDelta = y_current - c;
                    right->data.deltaY = remainingDelta; 

                }else{
                    // Ajuster delta du nœud actuel
                    cout << "pas de coupure" << endl;
                    double newDelta = y_current - zeroNode->data.deltaY ;
                    right->data.deltaY = newDelta ; 
                }




                } else { cout << " right n'a pas ete trouve , on est dans la merde gros" << endl;}


   // === Commencer l'algo de la fonction min ===
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
            bool prevAbove = prevY > c ;
            bool currAbove = currentY > c ;
            bool prevUnder = prevY < c ;
            bool currUnder  = currentY < c ;

            cout << "Visiting(current) x=" << node->data.x << ", deltaY=" << node->data.deltaY << ", currentY=" << currentY << endl;
            cout << "Prev x=" << prevX << ", prevY=" << prevY<<  endl;

            // Traversée de c ?
            if ((prevAbove && currUnder) || (prevUnder && currAbove)) {
                cout << "coupure found" <<  endl;

                // Interpolation pour trouver l'intersection
                double t = (c - prevY) / (currentY - prevY);
                double xi = prevX + t * (x - prevX);
                double deltaAtXi = (prevY >= c)? 0 : c - prevY;
                cout << "Point de coupure x=" << xi << ", deltaXi=" <<deltaAtXi << endl;
                toInsert.push_back({xi, deltaAtXi});
               // this->insert({xi, deltaAtXi});


                if ((prevAbove && currUnder)) {
                    // Ajuster delta du nœud actuel
                    double remainingDelta = currentY - c;
                    node->data.deltaY = remainingDelta;
                    currentY = c + remainingDelta; }

            }


        // Traversée de c ?
        if ((prevAbove && currentY == c)) {
            cout << "coupure found : breakpoint on c " <<  endl;
                // Ajuster delta du nœud actuel
                node->data.deltaY = 0;
                currentY = c ;
   
                     }
        }

        // Marquer les nœuds au-dessus de c pour suppression
        if (currentY > c ) {
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


// void maxfunction(double c) {
//     cout << "=== Début maxfunction avec c=" << c << " ===" << endl;

//     // === Étape 1 : gérer le noeud x = 0.0 ===
//     DeltaPoint zeroPt;
//     zeroPt.deltaY = 0;
//     zeroPt.x = 0;
//     double y_prev = 0;
//     Node* zeroNode = search(root, zeroPt);

//     if (zeroNode) {
//         cout << "Noeud x=0 trouvé: deltaY=" << zeroNode->data.deltaY << endl;
//         y_prev = zeroNode->data.deltaY;
//         if (zeroNode->data.deltaY < c) {
//             cout << "Mise à jour deltaY du noeud x=0 de " << zeroNode->data.deltaY << " vers " << c << endl;
//             zeroNode->data.deltaY = c;
//         }
//     } else {
//         cout << "Noeud x=0 introuvable, insertion..." << endl;
//         y_prev = 0;
//         double y0 = (c > 0) ? c : 0; // règle pour le delta initial
//         cout << "Insertion de (0, " << y0 << ")" << endl;
//         this->insert({0.0, y0});
//         zeroNode = search(root, zeroPt);
//     }

//     double xprev = zeroNode->data.x;
//     Node* right = successor(zeroNode);

//     if (right) {
//         cout << "Successeur de x=0 : x=" << right->data.x << ", deltaY=" << right->data.deltaY << endl;
//         bool prevBelow = y_prev < c;
//         double x_current = right->data.x;
//         double y_current = y_prev + right->data.deltaY;
//         bool currAbove = y_current > c;

//         cout << "prevBelow=" << prevBelow << ", currAbove=" << currAbove
//              << ", y_prev=" << y_prev << ", y_current=" << y_current << endl;

//         if ((prevBelow && currAbove)) {
//             cout << "Coupure détectée entre x=" << xprev << " et x=" << x_current << endl;
//             double t = (c - y_prev) / (y_current - y_prev);
//             double xi = xprev + t * (x_current - xprev);
//             cout << "Interpolation: t=" << t << ", xi=" << xi << endl;
//             double deltaAtXi = 0;
//             this->insert({xi, deltaAtXi});
//             cout << "Insertion du point d'intersection (" << xi << ", " << deltaAtXi << ")" << endl;
//             double remainingDelta = y_current - c;
//             right->data.deltaY = remainingDelta;
//             cout << "Mise à jour deltaY du successeur vers " << remainingDelta << endl;
//         } else {
//             double newDelta = y_current - zeroNode->data.deltaY;
//             cout << "Pas de coupure, mise à jour deltaY du successeur vers " << newDelta << endl;
//             right->data.deltaY = newDelta;
//         }
//     } else {
//         cout << "Pas de successeur trouvé pour x=0" << endl;
//     }
    
//     this->printTree();
//     // === Algo principal ===
//     cout << "=== Parcours infixe de l'arbre ===" << endl;
//     vector<DeltaPoint> toDeleteValues;
//     vector<DeltaPoint> toInsert;
//     Node* prevNode = nullptr;
//     double prevX = 0.0;
//     double prevY = 0.0;
//     double currentY = 0.0;

//     function<void(Node*)> inorder = [&](Node* node) {
//         if (!node) return;
//         inorder(node->left);
          
//         double x = node->data.x;
//         currentY += node->data.deltaY;
//         this->printTree();
//         cout << "Visite: x=" << x << ", deltaY=" << node->data.deltaY
//              << ", currentY=" << currentY << endl;

//         if (prevNode) {
//             bool prevBelow = prevY < c;
//             bool currBelow = currentY < c;
//             bool prevAbove = prevY > c;
//             bool currAbove = currentY > c;

//             cout << "   Comparaison: prevBelow=" << prevBelow
//                  << ", currAbove=" << currAbove
//                  << ", prevAbove=" << prevAbove
//                  << ", currBelow=" << currBelow << endl;

//             if ((prevBelow && currAbove) || (prevAbove && currBelow)) {
//                 cout << "   ↳ Coupure détectée entre x=" << prevX << " et x=" << x << endl;
//                 double t = (c - prevY) / (currentY - prevY);
//                 double xi = roundToMilli(prevX + t * (x - prevX));
//                 double deltaAtXi = (prevY <= c) ? 0 : c - prevY;
//                 cout << "     Interpolation: t=" << t << ", xi=" << xi
//                      << ", deltaAtXi=" << deltaAtXi << endl;
//                 toInsert.push_back({xi, deltaAtXi});

//                 if (prevBelow && currAbove) {
//                     double remainingDelta = currentY - c;
//                     node->data.deltaY = remainingDelta;
//                     cout << "     Mise à jour deltaY du noeud courant vers " << remainingDelta << endl;
//                 }
//             }

//             if (prevBelow && currentY == c) {
//                 cout << "   ↳ Plateau à c détecté, mise à zéro deltaY du noeud" << endl;
//                 node->data.deltaY = 0;
//                 currentY = c;
//             }
//         }

//         if (currentY < c) {
//             cout << "   ↳ Marquage pour suppression: (" << node->data.x << ", " << node->data.deltaY << ")" << endl;
//             toDeleteValues.push_back(node->data);
//         }

//         prevNode = node;
//         prevX = x;
//         prevY = currentY;

//         inorder(node->right);
//     };

//     inorder(root);

//     cout << "=== Suppression des noeuds marqués ===" << endl;
//     for (auto& val : toDeleteValues) {
//         cout << "Suppression: (" << val.x << ", " << val.deltaY << ")" << endl;
//         remove(val);
//     }


//     cout << "=== Insertion des points détectés ===" << endl;
//     for (auto& pt : toInsert) {
//         cout << "Insertion: (" << pt.x << ", " << pt.deltaY << ")" << endl;
//         insert(pt);
//     }


//     cout << "=== Fin maxfunction ===" << endl;
// }

void maxfunction(double c) {
    // === Étape 1 : gérer le noeud x = 0.0 ===
    DeltaPoint zeroPt;
    zeroPt.deltaY = 0;
    zeroPt.x = 0;
    double y_prev = 0;
    Node* zeroNode = search(root, zeroPt);

    if (zeroNode) {
        y_prev = zeroNode->data.deltaY;
        if (zeroNode->data.deltaY < c) zeroNode->data.deltaY = c;
    } else {
        y_prev = 0;
        double y0 = (c > 0) ? c : 0; // règle pour le delta initial
        this->insert({0.0, y0});
        zeroNode = search(root, zeroPt);
    }

    double xprev = zeroNode->data.x;
    Node* right = successor(zeroNode);

    if (right) {
        bool prevBelow = y_prev < c;
        double x_current = right->data.x;
        double y_current = y_prev + right->data.deltaY;
        bool currAbove = y_current > c;

        if ((prevBelow && currAbove)) {
            double t = (c - y_prev) / (y_current - y_prev);
            double xi = xprev + t * (x_current - xprev);
            double deltaAtXi = 0;
            this->insert({xi, deltaAtXi});
            double remainingDelta = y_current - c;
            right->data.deltaY = remainingDelta;
        } else {
            double newDelta = y_current - zeroNode->data.deltaY;
            right->data.deltaY = newDelta;
        }
    }

    // === Algo principal ===
    vector<Node*> toDelete;
    vector<DeltaPoint> toInsert;
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
            bool prevBelow = prevY < c;
            bool currBelow = currentY < c;
            bool prevAbove = prevY > c;
            bool currAbove = currentY > c;

            if ((prevBelow && currAbove) || (prevAbove && currBelow)) {
                double t = (c - prevY) / (currentY - prevY);
                double xi = prevX + t * (x - prevX);
                double deltaAtXi = (prevY <= c) ? 0 : c - prevY;
                toInsert.push_back({xi, deltaAtXi});

                if (prevBelow && currAbove) {
                    double remainingDelta = currentY - c;
                    node->data.deltaY = remainingDelta;
                    currentY = c + remainingDelta;
                }
            }

            if (prevBelow && currentY == c) {
                node->data.deltaY = 0;
                currentY = c;
            }
        }

        if (currentY < c) {
            toDelete.push_back(node);
        }

        prevNode = node;
        prevX = x;
        prevY = currentY;

        inorder(node->right);
    };

    inorder(root);


    for (Node* node : toDelete) {
        remove(node->data);
    }
    for (auto& pt : toInsert) {
        insert(pt);
    }

}

RedBlackTree<DeltaPoint> maxWithC(double c) const {
    RedBlackTree<DeltaPoint> copy;
    copy = *this;
    copy.maxfunction(c);
    return copy;
}


RedBlackTree<DeltaPoint> minWithC(double c) const {
    RedBlackTree<DeltaPoint> copy;
    copy = *this;
    copy.minfunction(c);
    return copy;
}


bool isLessOrEqual(const RedBlackTree<T>& g) {
    // On calcule diff = g - f
    RedBlackTree<DeltaPoint> diff = g;
    diff.minus(*this);

    bool result = true;

    // Parcours récursif en ordre croissant
    function<void(typename RedBlackTree<T>::Node*)> checkNode = [&](typename RedBlackTree<T>::Node* node) {
        if (!node || !result) return; // arrêter si déjà faux

        checkNode(node->left);

        double x = node->data.x;         // on prend la position x du noeud courant
        double diff_eval = diff.eval(x); // valeur de g(x) - f(x)

        if (diff_eval < 0) {      // si jamais g(x) < f(x)
            result = false;
            return; // on arrête
        }

        checkNode(node->right);
    };

    checkNode(diff.root);

    return result;
}

//================================================================================================================
//====================================== Eval min/max sur un interval ============================================
//================================================================================================================

double evaluate_max(double t_inf, double t_sup) const {
    if (!root || t_inf > t_sup) return 0.0;
    
    double a = this->eval(t_inf); double b = this->eval(t_sup);
    double maxVal = std::max(a, b); // bornes

    // parcours infixe de l'arbre
    function<void(Node*)> inorder = [&](Node* node) {
        if (!node) return;
        inorder(node->left);

        double x = node->data.x;
        if (x >= t_inf  && x <= t_sup ) {
            double val = this->eval(x);
            if (val > maxVal) maxVal = val;
        }

        inorder(node->right);
    };

    inorder(root);

    return maxVal;
}


double evaluate_min(double t_inf, double t_sup) const {
    if (!root || t_inf > t_sup) return 0.0;
    
    double a = this->eval(t_inf); double b = this->eval(t_sup);
    double minVal = std::min(a, b); // bornes

    // parcours infixe de l'arbre
    function<void(Node*)> inorder = [&](Node* node) {
        if (!node) return;
        inorder(node->left);

        double x = node->data.x;
        if (x >= t_inf  && x <= t_sup ) {
            double val = this->eval(x);
            if (val < minVal) minVal = val;
        }

        inorder(node->right);
    };

    inorder(root);

    return minVal;
}



//============================================================================
//==================== Update total contribution (min & max) =================
//============================================================================

void update_cbr_stmin(double stmin_old, double stmin, 
                                double ctmin, double cap_min, 
                                double cap_max) 
{
    double cap = (cap_min > 0) ? cap_max : cap_min;

    if (std::abs(stmin - stmin_old) < 1e-6)
        return;

    RedBlackTree<DeltaPoint> delta;

    // Case (1)
    if (stmin_old < stmin && stmin < ctmin) {
        double slope = -cap / (ctmin - stmin_old);
        double gap = slope * (stmin - stmin_old);
        delta = delta_profile(gap, stmin_old, stmin, ctmin);
    }
    // Case (2)
    else if (stmin_old <= ctmin && ctmin < stmin) {
        double gap = -cap;
        delta = delta_profile(gap, stmin_old, ctmin, stmin);
    }
    // Case (3)
    else if (ctmin < stmin_old && stmin_old < stmin) {
        double slope = -cap / (ctmin - stmin);
        double gap = slope * (stmin_old - stmin);
        delta = delta_profile(gap, ctmin, stmin_old, stmin);
    }

    this->sum(delta);
}


void update_cbr_ctmin(double ctmin_old, double ctmin,
                                double stmin, double cap_min,
                                double cap_max) 
{
    double cap = (cap_min > 0) ? cap_max : cap_min;

    if (std::abs(ctmin - ctmin_old) < 1e-6)
        return;

    RedBlackTree<DeltaPoint> delta;

    // Case (4)
    if (stmin < ctmin_old && ctmin_old < ctmin) {
        double slope = -cap / (ctmin - stmin);
        double gap = slope * (ctmin - ctmin_old);
        delta = delta_profile(gap, stmin, ctmin_old, ctmin);
    }
    // Case (5)
    else if (ctmin_old <= stmin && stmin < ctmin) {
        double gap = -cap;
        delta = delta_profile(gap, ctmin_old, stmin, ctmin);
    }
    // Case (6)
    else if (ctmin_old < ctmin && ctmin < stmin) {
        double slope = -cap / (ctmin_old - stmin);
        double gap = slope * (ctmin_old - ctmin);
        delta = delta_profile(gap, ctmin_old, ctmin, stmin);
    }

    this->sum(delta);
}


void update_cbr_stmax(double stmax_old, double stmax,
                            double ctmax, double cap_min,
                            double cap_max) 
{
    double cap = (cap_min > 0) ? cap_min : cap_max;

    if (std::abs(stmax - stmax_old) < 1e-6)
        return;

    RedBlackTree<DeltaPoint> delta;

    // Case (7)
    if (stmax < stmax_old && stmax_old < ctmax) {
        double slope = cap / (ctmax - stmax);
        double gap = slope * (stmax_old - stmax);
        delta = delta_profile(gap, stmax, stmax_old, ctmax);
    }
    // Case (8)
    else if (stmax <= ctmax && ctmax < stmax_old) {
        double gap = cap;
        delta = delta_profile(gap, stmax, ctmax, stmax_old);
    }
    // Case (9)
    else if (ctmax < stmax && stmax < stmax_old) {
        double slope = cap / (ctmax - stmax_old);
        double gap = slope * (stmax - stmax_old);
        delta = delta_profile(gap, ctmax, stmax, stmax_old);
    }

    this->sum(delta);
}


void update_cbr_ctmax(double ctmax_old, double ctmax,
                                double stmax, double cap_min,
                                double cap_max) 
{
    double cap = (cap_min > 0) ? cap_min : cap_max;

    if (std::abs(ctmax - ctmax_old) < 1e-6)
        return;

    RedBlackTree<DeltaPoint> delta;

    // Case (10)
    if (stmax < ctmax && ctmax < ctmax_old) {
        double slope = cap / (ctmax_old - stmax);
        double gap = slope * (ctmax_old - ctmax);
        delta = delta_profile(gap, stmax, ctmax, ctmax_old);
    }
    // Case (11)
    else if (ctmax <= stmax && stmax < ctmax_old) {
        double gap = cap;
        delta = delta_profile(gap, ctmax, stmax, ctmax_old);
    }
    // Case (12)
    else if (ctmax < ctmax_old && ctmax_old < stmax) {
        double slope = cap / (ctmax - stmax);
        double gap = slope * (ctmax - ctmax_old);
        delta = delta_profile(gap, ctmax, ctmax_old, stmax);
    }

    this->sum(delta);
}


void update_cbr_cap(double cap_old, double cap, double start, double end) 
{
    if (std::abs(cap - cap_old) < 1e-6)
        return;

    double gap = cap - cap_old;
    RedBlackTree<DeltaPoint> delta = cba_profile(gap, start, end);

    this->sum(delta);
}

//==========================================================================================================
//================ methodes pour extraire les point (x,f(x)) ou (x, deltay) in-order traversal==============
//==========================================================================================================
// extraction de tous les noeuds (x,f(x)) 
std::vector<std::pair<double, double>> to_points() const {
    std::vector<std::pair<double, double>> result;
    double cumulative = 0.0;

    std::function<void(Node*)> inorder = [&](Node* node) {
        if (!node) return;
        inorder(node->left);
        cumulative += node->data.deltaY;
        result.push_back({node->data.x, cumulative});
        inorder(node->right);
    };

    inorder(root);
    return result;
}

// extraction de tous les noeuds (x,deltay) 
std::vector<std::pair<double, double>> to_points_delta() const {
    std::vector<std::pair<double, double>> result;


    std::function<void(Node*)> inorder = [&](Node* node) {
        if (!node) return;
        inorder(node->left);
        result.push_back({node->data.x, node->data.deltaY});
        inorder(node->right);
    };

    inorder(root);
    return result;
}

// extraction de noeud (x,f(x)) sur le compact [a,b] (Mais visite de tous les noeuds !)
std::vector<std::pair<double, double>> to_points_compact(double a, double b) const {
    std::vector<std::pair<double, double>> result;
    double cumulative = 0.0;

    std::function<void(Node*)> inorder = [&](Node* node) {
        if (!node) return;
        inorder(node->left);

        cumulative += node->data.deltaY;

        if (node->data.x>= a && node->data.x <= b) {
            result.push_back({node->data.x, cumulative});
        }

        inorder(node->right);
    };

    inorder(root);
    return result;
}

// extraction de noeud (x,deltay) sur le compact [a,b] (Evite de vister tous les noeuds !)
std::vector<std::pair<double, double>> to_points_compact_bis(double xmin, double xmax) const {
    std::vector<std::pair<double, double>> result;
    std::function<void(Node*)> helper = [&](Node* node) {
        if (!node) return;

        // Si ce nœud peut avoir des descendants dans l'intervalle, on va à gauche
        if (node->data.x > xmin) {
            helper(node->left);
        }

        // On stocke seulement si x est dans [xmin, xmax]
        if (node->data.x >= xmin && node->data.x <= xmax) {
            result.push_back({node->data.x, node->data.deltaY});
        }

        // Si ce nœud peut avoir des descendants dans l'intervalle, on va à droite
        if (node->data.x < xmax) {
            helper(node->right);
        }
    };

    helper(root);
    return result;
}

//==============================================================================
//==================== methode annexe pour print/draw function =================
//==============================================================================

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



//=================================================================================================================
//======================================  Construction profile delta function =====================================
//=================================================================================================================

RedBlackTree<DeltaPoint> delta_profile(double gap, double a, double b , double c) {
    RedBlackTree<DeltaPoint> delta;
//    delta.insert({0, 0});      // départ
    delta.insert({a, 0});      // reste à 0
    delta.insert({b, gap});    // monte à gap
    delta.insert({c, -gap});      // redescend à 0
//    delta.insert({horizon, 0}); // plateau final à 0
    return delta;
}

RedBlackTree<DeltaPoint> cba_profile(double cap, double a, double b) {
    RedBlackTree<DeltaPoint> cba;

  //  cba.insert({0, 0});        // départ
    cba.insert({a, 0});        // plateau à 0
    cba.insert({b, cap});      // monte à cap
   // cba.insert({horizon, cap}); // reste à cap
    return cba;
}

