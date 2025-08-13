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

    bool operator<=(const DeltaPoint& other) const {
        return x <= other.x;
    }

    bool operator==(const DeltaPoint& other) const {
        return fabs(x - other.x) < EPSILON;
    }
};



// Class template for Red-Black Tree
template <typename T> class RedBlackTree {
private:
    // Structure for a node in Red-Black Tree
    struct Node {
        T data;
        Color color;
        Node* parent;
        Node* left;
        Node* right;

        // Constructor to initialize node with data and
        // color
        Node(T value)
            : data(value)
            , color(RED)
            , parent(nullptr)
            , left(nullptr)
            , right(nullptr)
        {
        }
    };

    Node* root; // Root of the Red-Black Tree

    // Utility function: Left Rotation
    void rotateLeft(Node*& node)
    {
        Node* child = node->right;
        node->right = child->left;
        if (node->right != nullptr)
            node->right->parent = node;
        child->parent = node->parent;
        if (node->parent == nullptr)
            root = child;
        else if (node == node->parent->left)
            node->parent->left = child;
        else
            node->parent->right = child;
        child->left = node;
        node->parent = child;
    }

    // Utility function: Right Rotation
    void rotateRight(Node*& node)
    {
        Node* child = node->left;
        node->left = child->right;
        if (node->left != nullptr)
            node->left->parent = node;
        child->parent = node->parent;
        if (node->parent == nullptr)
            root = child;
        else if (node == node->parent->left)
            node->parent->left = child;
        else
            node->parent->right = child;
        child->right = node;
        node->parent = child;
    }

    // Utility function: Fixing Insertion Violation
    void fixInsert(Node*& node)
    {
        Node* parent = nullptr;
        Node* grandparent = nullptr;
        while (node != root && node->color == RED
               && node->parent->color == RED) {
            parent = node->parent;
            grandparent = parent->parent;
            if (parent == grandparent->left) {
                Node* uncle = grandparent->right;
                if (uncle != nullptr
                    && uncle->color == RED) {
                    grandparent->color = RED;
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    node = grandparent;
                }
                else {
                    if (node == parent->right) {
                        rotateLeft(parent);
                        node = parent;
                        parent = node->parent;
                    }
                    rotateRight(grandparent);
                    swap(parent->color, grandparent->color);
                    node = parent;
                }
            }
            else {
                Node* uncle = grandparent->left;
                if (uncle != nullptr
                    && uncle->color == RED) {
                    grandparent->color = RED;
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    node = grandparent;
                }
                else {
                    if (node == parent->left) {
                        rotateRight(parent);
                        node = parent;
                        parent = node->parent;
                    }
                    rotateLeft(grandparent);
                    swap(parent->color, grandparent->color);
                    node = parent;
                }
            }
        }
        root->color = BLACK;
    }

    // Utility function: Fixing Deletion Violation
    void fixDelete(Node*& node)
    {
        while (node != root && node->color == BLACK) {
            if (node == node->parent->left) {
                Node* sibling = node->parent->right;
                if (sibling->color == RED) {
                    sibling->color = BLACK;
                    node->parent->color = RED;
                    rotateLeft(node->parent);
                    sibling = node->parent->right;
                }
                if ((sibling->left == nullptr
                     || sibling->left->color == BLACK)
                    && (sibling->right == nullptr
                        || sibling->right->color
                               == BLACK)) {
                    sibling->color = RED;
                    node = node->parent;
                }
                else {
                    if (sibling->right == nullptr
                        || sibling->right->color == BLACK) {
                        if (sibling->left != nullptr)
                            sibling->left->color = BLACK;
                        sibling->color = RED;
                        rotateRight(sibling);
                        sibling = node->parent->right;
                    }
                    sibling->color = node->parent->color;
                    node->parent->color = BLACK;
                    if (sibling->right != nullptr)
                        sibling->right->color = BLACK;
                    rotateLeft(node->parent);
                    node = root;
                }
            }
            else {
                Node* sibling = node->parent->left;
                if (sibling->color == RED) {
                    sibling->color = BLACK;
                    node->parent->color = RED;
                    rotateRight(node->parent);
                    sibling = node->parent->left;
                }
                if ((sibling->left == nullptr
                     || sibling->left->color == BLACK)
                    && (sibling->right == nullptr
                        || sibling->right->color
                               == BLACK)) {
                    sibling->color = RED;
                    node = node->parent;
                }
                else {
                    if (sibling->left == nullptr
                        || sibling->left->color == BLACK) {
                        if (sibling->right != nullptr)
                            sibling->right->color = BLACK;
                        sibling->color = RED;
                        rotateLeft(sibling);
                        sibling = node->parent->left;
                    }
                    sibling->color = node->parent->color;
                    node->parent->color = BLACK;
                    if (sibling->left != nullptr)
                        sibling->left->color = BLACK;
                    rotateRight(node->parent);
                    node = root;
                }
            }
        }
        node->color = BLACK;
    }

    // Utility function: Find Node with Minimum Value
    Node* minValueNode(Node*& node)
    {
        Node* current = node;
        while (current->left != nullptr)
            current = current->left;
        return current;
    }

    // Utility function: Transplant nodes in Red-Black Tree
    void transplant(Node*& root, Node*& u, Node*& v)
    {
        if (u->parent == nullptr)
            root = v;
        else if (u == u->parent->left)
            u->parent->left = v;
        else
            u->parent->right = v;
        if (v != nullptr)
            v->parent = u->parent;
    }

    // Utility function: Helper to print Red-Black Tree
    void printHelper(Node* root, string indent, bool last) {
        if (root) {
            cout << indent;
            if (last) {
                cout << "R----";
                indent += "   ";
            } else {
                cout << "L----";
                indent += "|  ";
            }
            cout << "(" << root->data.x << ", " << root->data.deltaY << ")"
                 << " [" << (root->color == RED ? "RED" : "BLACK") << "]" << endl;
            printHelper(root->left, indent, false);
            printHelper(root->right, indent, true);
        }
    }

    // Utility function: Delete all nodes in the Red-Black
    // Tree
    void deleteTree(Node* node)
    {
        if (node != nullptr) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }

    Node* search(Node* node, const T& key) {
        if (!node || fabs(node->data.x - key.x)< EPSILON)
            return node;
        if (key < node->data)
            return search(node->left, key);
        else
            return search(node->right, key);
    }

public:
    // Constructor: Initialize Red-Black Tree
    RedBlackTree()
        : root(nullptr)
    {
    }

    // Destructor: Delete Red-Black Tree
    ~RedBlackTree() { deleteTree(root); }

    // Public function: Insert a value into Red-Black Tree
    void insert(T key)
    {
        Node* node = new Node(key);
        Node* parent = nullptr;
        Node* current = root;
        while (current != nullptr) {
            parent = current;
            if (node->data < current->data)
                current = current->left;
            else
                current = current->right;
        }
        node->parent = parent;
        if (parent == nullptr)
            root = node;
        else if (node->data < parent->data)
            parent->left = node;
        else
            parent->right = node;
        fixInsert(node);
    }


    void remove(T key)
    {
        Node* node = root;
        Node* z = nullptr;
        Node* x = nullptr;
        Node* y = nullptr;
        while (node != nullptr) {
            if (node->data == key) {
                z = node;
            }

            if (node->data <= key) {
                node = node->right;
            }
            else {
                node = node->left;
            }
        }

        if (z == nullptr) {
            cout << "Key not found in the tree" << endl;
            return;
        }

        y = z;
        Color yOriginalColor = y->color;
        if (z->left == nullptr) {
            x = z->right;
            transplant(root, z, z->right);
        }
        else if (z->right == nullptr) {
            x = z->left;
            transplant(root, z, z->left);
        }
        else {
            y = minValueNode(z->right);
            yOriginalColor = y->color;
            x = y->right;
            if (y->parent == z) {
                if (x != nullptr)
                    x->parent = y;
            }
            else {
                transplant(root, y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            transplant(root, z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }
        delete z;
        if (yOriginalColor == BLACK) {
            fixDelete(x);
        }
    }


    // Public function: Print the Red-Black Tree
    void printTree()
    {
        if (root == nullptr)
            cout << "Tree is empty." << endl;
        else {
            cout << "Red-Black Tree:" << endl;
            printHelper(root, "", true);
        }
    }



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

            this->insert({x, delta_sum});
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



void minfunction(double c) {
    vector<Node*> toDelete;

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
            cout << "Visiting x=" << node->data.x << ", deltaY=" << node->data.deltaY << ", currentY=" << currentY << endl;

            // Traversée de c ?
            if ((prevAbove && !currAbove) || (!prevAbove && currAbove)) {
                cout << "coucou" <<  endl;

                // Interpolation pour trouver l'intersection
                double t = (c - prevY) / (currentY - prevY);
                double xi = prevX + t * (x - prevX);
                double deltaAtXi = (prevY > c + EPSILON)? 0 : c - prevY;

                insert({xi, deltaAtXi});


                if ((prevAbove && !currAbove)) {
                    // Ajuster delta du nœud actuel
                    double remainingDelta = currentY - c;
                    node->data.deltaY = remainingDelta;
                    currentY = c + remainingDelta; }

            }
        }

        // Marquer les nœuds au-dessus de c pour suppression
        if (currentY > c + EPSILON) {
            cout << " salut mon gars "<< endl;
            toDelete.push_back(node);
        }
        cout << "Hello "  << endl;
        prevNode = node;
        prevX = x;
        prevY = currentY;

        inorder(node->right);
    };

    inorder(root);
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



//End RBT class
};


