#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <functional>
#include <cmath>  // pour fabs()
using namespace std;

enum Color { RED, BLACK };

struct DeltaPoint {
    double x;         // abscisse du point
    double deltaY;    // y_i - y_{i-1}

    bool operator<(const DeltaPoint& other) const {
        return x < other.x;
    }

    bool operator==(const DeltaPoint& other) const {
        return fabs(x - other.x) < 1e-9;
    }
};

template <typename T>
class RedBlackTree {
private:
    struct Node {
        T data;
        Color color;
        Node* parent;
        Node* left;
        Node* right;

        Node(T value)
            : data(value), color(RED), parent(nullptr), left(nullptr), right(nullptr) {}
    };

    Node* root;
    vector<pair<string, T>> history;

    void rotateLeft(Node*& node) {
        Node* child = node->right;
        node->right = child->left;
        if (child->left)
            child->left->parent = node;
        child->parent = node->parent;
        if (!node->parent)
            root = child;
        else if (node == node->parent->left)
            node->parent->left = child;
        else
            node->parent->right = child;
        child->left = node;
        node->parent = child;
    }

    void rotateRight(Node*& node) {
        Node* child = node->left;
        node->left = child->right;
        if (child->right)
            child->right->parent = node;
        child->parent = node->parent;
        if (!node->parent)
            root = child;
        else if (node == node->parent->left)
            node->parent->left = child;
        else
            node->parent->right = child;
        child->right = node;
        node->parent = child;
    }

    void fixInsert(Node*& node) {
        while (node != root && node->parent->color == RED) {
            Node* parent = node->parent;
            Node* grandparent = parent->parent;

            if (parent == grandparent->left) {
                Node* uncle = grandparent->right;
                if (uncle && uncle->color == RED) {
                    grandparent->color = RED;
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    node = grandparent;
                } else {
                    if (node == parent->right) {
                        rotateLeft(parent);
                        node = parent;
                    }
                    rotateRight(grandparent);
                    swap(parent->color, grandparent->color);
                    node = parent;
                }
            } else {
                Node* uncle = grandparent->left;
                if (uncle && uncle->color == RED) {
                    grandparent->color = RED;
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    node = grandparent;
                } else {
                    if (node == parent->left) {
                        rotateRight(parent);
                        node = parent;
                    }
                    rotateLeft(grandparent);
                    swap(parent->color, grandparent->color);
                    node = parent;
                }
            }
        }
        root->color = BLACK;
    }

    void fixDelete(Node*& node) {
        while (node != root && (!node || node->color == BLACK)) {
            Node* sibling;
            if (node == node->parent->left) {
                sibling = node->parent->right;
                if (sibling && sibling->color == RED) {
                    sibling->color = BLACK;
                    node->parent->color = RED;
                    rotateLeft(node->parent);
                    sibling = node->parent->right;
                }
                if ((!sibling->left || sibling->left->color == BLACK) &&
                    (!sibling->right || sibling->right->color == BLACK)) {
                    sibling->color = RED;
                    node = node->parent;
                } else {
                    if (!sibling->right || sibling->right->color == BLACK) {
                        if (sibling->left)
                            sibling->left->color = BLACK;
                        sibling->color = RED;
                        rotateRight(sibling);
                        sibling = node->parent->right;
                    }
                    sibling->color = node->parent->color;
                    node->parent->color = BLACK;
                    if (sibling->right)
                        sibling->right->color = BLACK;
                    rotateLeft(node->parent);
                    node = root;
                }
            } else {
                sibling = node->parent->left;
                if (sibling && sibling->color == RED) {
                    sibling->color = BLACK;
                    node->parent->color = RED;
                    rotateRight(node->parent);
                    sibling = node->parent->left;
                }
                if ((!sibling->left || sibling->left->color == BLACK) &&
                    (!sibling->right || sibling->right->color == BLACK)) {
                    sibling->color = RED;
                    node = node->parent;
                } else {
                    if (!sibling->left || sibling->left->color == BLACK) {
                        if (sibling->right)
                            sibling->right->color = BLACK;
                        sibling->color = RED;
                        rotateLeft(sibling);
                        sibling = node->parent->left;
                    }
                    sibling->color = node->parent->color;
                    node->parent->color = BLACK;
                    if (sibling->left)
                        sibling->left->color = BLACK;
                    rotateRight(node->parent);
                    node = root;
                }
            }
        }
        if (node) node->color = BLACK;
    }

    Node* minValueNode(Node* node) {
        while (node->left)
            node = node->left;
        return node;
    }

    void transplant(Node*& u, Node*& v) {
        if (!u->parent)
            root = v;
        else if (u == u->parent->left)
            u->parent->left = v;
        else
            u->parent->right = v;
        if (v)
            v->parent = u->parent;
    }

    void deleteTree(Node* node) {
        if (node) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }

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

public:
    RedBlackTree() : root(nullptr) {}
    ~RedBlackTree() { deleteTree(root); }

    void insert(T key) {
        Node* node = new Node(key);
        Node* parent = nullptr;
        Node* current = root;
        while (current) {
            parent = current;
            if (node->data < current->data)
                current = current->left;
            else
                current = current->right;
        }
        node->parent = parent;
        if (!parent)
            root = node;
        else if (node->data < parent->data)
            parent->left = node;
        else
            parent->right = node;
        fixInsert(node);

        history.push_back({"insert", key});
    }

    void remove(T key) {
        Node* z = search(root, key);
        if (!z) {
            cout << "Key (" << key.x << ") not found." << endl;
            return;
        }

        history.push_back({"remove", z->data});

        Node* y = z;
        Node* x = nullptr;
        Color yOriginalColor = y->color;
        if (!z->left) {
            x = z->right;
            transplant(z, z->right);
        } else if (!z->right) {
            x = z->left;
            transplant(z, z->left);
        } else {
            y = minValueNode(z->right);
            yOriginalColor = y->color;
            x = y->right;
            if (y->parent == z) {
                if (x)
                    x->parent = y;
            } else {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }
        delete z;
        if (yOriginalColor == BLACK && x)
            fixDelete(x);
    }

    void undoLastOperation() {
        if (history.empty()) {
            cout << "Nothing to undo." << endl;
            return;
        }
        auto [action, dp] = history.back();
        history.pop_back();

        if (action == "insert") {
            remove(dp);
        } else if (action == "remove") {
            insert(dp);
        }
    }

    double eval_in(double x) {
        double result = 0.0;
        evalHelper(root, x, result);
        return result;
    }





    const double EPSILON = 1e-8;
    

    
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
            cerr << "Erreur : impossible d’ouvrir le fichier " << filename << endl;
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
            cout << "Tree is empty." << endl;
        else
            printHelper(root, "", true);
    }
};


