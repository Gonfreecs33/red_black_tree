#include <iostream>
#include <map>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <functional>
#include <algorithm>
#include <fstream>
#include <utility>

const double EPSILON = 1e-6; // Utiliser une tolérance plus petite pour les comparaisons de double

class PiecewiseLinearFunction {
private:
    // map où la clé est l'abscisse (x) et la valeur est le deltaY
    std::map<double, double> breakpoints;

    double eval(double x) const {
        if (breakpoints.empty()) {
            return 0.0;
        }
    
        double y = 0.0;
        auto prev_it = breakpoints.begin();
        double y_prev = prev_it->second;   // valeur au premier breakpoint
        double x_prev = prev_it->first;
    
        // Cas particulier : si x < premier point
        if (x < x_prev) {
            return 0.0; // ou bien la valeur initiale si tu veux f(0) = y0
        }
    
        // Accumuler et trouver l’intervalle où se situe x
        for (auto it = std::next(breakpoints.begin()); it != breakpoints.end(); ++it) {
            double x_curr = it->first;
            double delta = it->second;
            double y_curr = y_prev + delta;
    
            if (x <= x_curr + EPSILON) {
                // interpolation entre (x_prev, y_prev) et (x_curr, y_curr)
                double slope = (y_curr - y_prev) / (x_curr - x_prev);
                return y_prev + slope * (x - x_prev);
            }
    
            // avancer
            x_prev = x_curr;
            y_prev = y_curr;
        }
    
        // Si x est au-delà du dernier breakpoint
        return y_prev;
    }
    
public:
    PiecewiseLinearFunction(double y0 = 0.0) {
        breakpoints[0.0] = y0;
    }
    
    // Constructeur de copie
    PiecewiseLinearFunction(const PiecewiseLinearFunction& other) = default;
    
    // Opérateur d'affectation
    PiecewiseLinearFunction& operator=(const PiecewiseLinearFunction& other) = default;

    void addBreakpoint(double x, double deltaY) {
        // Ajouter à la valeur existante si le point de rupture existe
        breakpoints[x] = deltaY;
    }

    void removeBreakpoint(double x) {
        auto it = breakpoints.find(x);
        if (it != breakpoints.end()) {
            breakpoints.erase(it);
        }
    }

    // Évalue la fonction en un point x
    double evaluate(double x) const {
        return eval(x);
    }
//======================================================================================================
//==========================              sum/minus f+g /f-g          ==================================
//======================================================================================================

// Addition de deux fonctions
    void sum(const PiecewiseLinearFunction& g) {
        if (g.breakpoints.empty()) return;
    
        double xg_min = g.breakpoints.begin()->first;
        double xg_max = g.breakpoints.rbegin()->first;
        double yi_prec = 0.0;
    
        // bornes utiles de f
        auto it_f = breakpoints.upper_bound(xg_min);
        auto end_f = breakpoints.upper_bound(xg_max);
    
        auto it_g = g.breakpoints.begin();

        if (it_f != breakpoints.begin()) {
            auto prev = it_f;   // copie
            --prev;           // recule d'un cran
            yi_prec = this->eval(prev->first);
            std::cout << "un noeud de f à gauche de g trouvé\n";
            std::cout << " first yi = " << yi_prec << "\n";
            std::cout << " first xi = " << prev->first << "\n";
        }
    
        struct Operation {
            enum Type { Insert, Update } type;
            double x;
            double delta;
        };
        std::vector<Operation> ops;
    
        std::cout << "=== Starting sum ===\n";
    
        while (it_f != end_f || it_g != g.breakpoints.end()) {
            double x;
            bool take_f = false, take_g = false;
    
            if (it_g != g.breakpoints.end() &&
                (it_f == end_f || it_g->first < it_f->first)) {
                x = it_g->first;
                take_g = true;
            } else if (it_f != end_f &&
                       (it_g == g.breakpoints.end() || it_f->first < it_g->first)) {
                x = it_f->first;
                take_f = true;
            } else { // même abscisse
                x = it_f->first;
                take_f = take_g = true;
            }
    
            double F = this->eval(x);
            double G = g.eval(x);
            double sum = F + G;
            double delta_sum = sum - yi_prec;
            yi_prec = sum;
    
            std::cout << "x=" << x 
                      << " | F=" << F << " G=" << G 
                      << " -> sum=" << sum 
                      << " delta=" << delta_sum << "\n";
    
            // déjà un point à x ?
            auto it_match = breakpoints.find(x);
            if (it_match != breakpoints.end()) {
                ops.push_back({Operation::Update, x, delta_sum});
            } else {
                ops.push_back({Operation::Insert, x, delta_sum});
            }
    
            if (take_f) ++it_f;
            if (take_g) ++it_g;
        }
    
        // Appliquer les opérations
        for (auto& op : ops) {
            breakpoints[op.x] = op.delta;
        }
    
        std::cout << "=== Sum completed ===\n";
    }
    


    
    // Soustraction de deux fonctions
// Soustraction de deux fonctions (this - g)
void minus(const PiecewiseLinearFunction& g) {
    if (g.breakpoints.empty()) return;

    double xg_min = g.breakpoints.begin()->first;
    double xg_max = g.breakpoints.rbegin()->first;
    double yi_prec = 0.0;

    // bornes utiles de f
    auto it_f = breakpoints.upper_bound(xg_min);
    auto end_f = breakpoints.upper_bound(xg_max);

    auto it_g = g.breakpoints.begin();

    // initialiser yi_prec avec le point de f avant xg_min
    if (it_f != breakpoints.begin()) {
        auto prev = it_f;
        --prev;
        yi_prec = this->eval(prev->first);
        std::cout << "un noeud de f à gauche de g trouvé\n";
        std::cout << " first yi = " << yi_prec << "\n";
        std::cout << " first xi = " << prev->first << "\n";
    }

    struct Operation {
        enum Type { Insert, Update } type;
        double x;
        double delta;
    };
    std::vector<Operation> ops;

    std::cout << "=== Starting minus ===\n";

    while (it_f != end_f || it_g != g.breakpoints.end()) {
        double x;
        bool take_f = false, take_g = false;

        if (it_g != g.breakpoints.end() &&
            (it_f == end_f || it_g->first < it_f->first)) {
            x = it_g->first;
            take_g = true;
        } else if (it_f != end_f &&
                   (it_g == g.breakpoints.end() || it_f->first < it_g->first)) {
            x = it_f->first;
            take_f = true;
        } else { // même abscisse
            x = it_f->first;
            take_f = take_g = true;
        }

        double F = this->eval(x);
        double G = g.eval(x);
        double diff = F - G;
        double delta_diff = diff - yi_prec;
        yi_prec = diff;

        std::cout << "x=" << x 
                  << " | F=" << F << " G=" << G 
                  << " -> diff=" << diff 
                  << " delta=" << delta_diff << "\n";

        // déjà un point à x ?
        auto it_match = breakpoints.find(x);
        if (it_match != breakpoints.end()) {
            ops.push_back({Operation::Update, x, delta_diff});
        } else {
            ops.push_back({Operation::Insert, x, delta_diff});
        }

        if (take_f) ++it_f;
        if (take_g) ++it_g;
    }

    // Appliquer les opérations
    for (auto& op : ops) {
        breakpoints[op.x] = op.delta;
    }

    std::cout << "=== Minus completed ===\n";
}

    // Négation de la fonction
    void negate() {
        for (auto& pair : breakpoints) {
            pair.second = -pair.second;
        }
    }
//======================================================================================================
//====================================== min(f, constante c) and max  ==================================
//======================================================================================================
    void minfunction(double c) {
        std::cout << "--- minfunction start ---" << std::endl;
    
        if (breakpoints.empty()) return;

        //  Premier élément
        auto it_prev = breakpoints.begin();
        double x_prev = it_prev->first;
        double y_prev_old = it_prev->second;
        double y_prev = std::min(y_prev_old, c); // tronquer au besoin
        it_prev->second = y_prev;
        
        std::cout << "First node: x=" << x_prev << ", y=" << y_prev_old << std::endl;
        
        //  Élément suivant
        auto it_current = std::next(it_prev);
        if (it_current != breakpoints.end()) {
            double x_current = it_current->first;
            double y_current = y_prev_old + it_current->second;
        
            bool prevAbove = y_prev_old > c;
            bool currUnder = y_current < c;
        
            std::cout << "Visiting first segment: x_current=" << x_current
                      << ", y_current=" << y_current << std::endl;
        
            // Traversée de c ?
            if (prevAbove && currUnder) {
                std::cout << "Crossing found in first segment" << std::endl;
                // Interpolation pour trouver xi
                double t = (c - y_prev_old) / (y_current - y_prev_old);
                double xi = x_prev + t * (x_current - x_prev);
        
                // Ajouter breakpoint à xi avec delta 0
                double deltaAtXi = 0;
                breakpoints[xi] = deltaAtXi;
                std::cout << "Insert breakpoint at xi=" << xi << ", delta=" << deltaAtXi << std::endl;
        
                // Ajuster delta du point actuel
                double remainingDelta = y_current - c;
                it_current->second = remainingDelta;
                std::cout << "Adjust current delta: x=" << x_current
                          << ", remainingDelta=" << remainingDelta << std::endl;
        
            } else {

                if(c < y_prev_old){
                                    // Pas de coupure, ajuster delta
                double newDelta = y_current - y_prev ;
                it_current->second = newDelta;
                std::cout << "No crossing zero above c, adjust delta: x=" << x_current
                          << ", newDelta=" << newDelta << std::endl;
                    
                }else{
                // Pas de coupure, ajuster delta
                double newDelta = y_current - y_prev_old;
                it_current->second = newDelta;
                std::cout << "No crossing, adjust delta: x=" << x_current
                          << ", newDelta=" << newDelta << std::endl;}
            }
        } else {
            std::cout << "No element after first node" << std::endl;
        }
        



        std::vector<std::pair<double, double>> points;
        double y = 0.0;
        for (auto &kv : breakpoints) {
            y += kv.second;
            points.push_back({kv.first, y});
            std::cout << "Accumulated point: x=" << kv.first << ", y=" << y << std::endl;
        }
    
        // Appliquer min avec c
        for (size_t i = 0; i < points.size() - 1; ++i) {
            double x = points[i + 1].first;
            double y_val = points[i + 1].second;
    
            double y_prev = points[i].second;
            double x_prev = points[i].first;
    
            bool prevAbove = y_prev > c;
            bool currAbove = y_val > c ;
            bool prevUnder = y_prev < c;
            bool currUnder = y_val < c ;
    
            std::cout << "\nVisiting segment: [" << x_prev << ", " << x << "] "
                      << "y_prev=" << y_prev << ", y_curr=" << y_val << std::endl;
    
            // Cas où on traverse la ligne y=c
            if ((prevAbove && currUnder) || (prevUnder && currAbove)) {
                double t = (c - y_prev) / (y_val - y_prev);
                double xi = x_prev + t * (x - x_prev);
                double deltaAtXi = (y_prev >= c) ? 0 : c - y_prev;
    
                std::cout << "Crossing found at x=" << xi << ", deltaAtXi=" << deltaAtXi << std::endl;
                this->addBreakpoint(xi, deltaAtXi);
    
                if (prevAbove && currUnder) {
                    double remainingDelta = y_val - c;
                    breakpoints[x] = remainingDelta;
                    points[i + 1].second = c + remainingDelta;
                    std::cout << "Adjusting current delta: x=" << x << ", remainingDelta=" << remainingDelta << std::endl;
                }
            }
    
            // Cas où le point tombe exactement sur c
            if (prevAbove && y_val == c) {
                std::cout << "Breakpoint exactly at c at x =" << x << std::endl;
                breakpoints[x] = 0;
                points[i + 1].second = c;
            }
    
            // Supprimer les nœuds au-dessus de c
            if (y_val > c) {
                std::cout << "Deleting node above c: x=" << x << ", y=" << y_val << std::endl;
                breakpoints.erase(x);
            }
        }
    
        std::cout << "--- minfunction end ---" << std::endl;
    }
    

    void maxfunction(double c) {
        std::cout << "--- maxfunction start ---" << std::endl;
    
        if (breakpoints.empty()) return;
    
        //  Premier élément
        auto it_prev = breakpoints.begin();
        double x_prev = it_prev->first;
        double y_prev_old = it_prev->second;
        double y_prev = std::max(y_prev_old, c); // relever au besoin
        it_prev->second = y_prev;
    
        std::cout << "First node: x=" << x_prev << ", y=" << y_prev_old << std::endl;
    
        //  Élément suivant
        auto it_current = std::next(it_prev);
        if (it_current != breakpoints.end()) {
            double x_current = it_current->first;
            double y_current = y_prev_old + it_current->second;
    
            bool prevUnder = y_prev_old < c;
            bool currAbove = y_current > c;
    
            std::cout << "Visiting first segment: x_current=" << x_current
                      << ", y_current=" << y_current << std::endl;
    
            // Traversée de c ?
            if (prevUnder && currAbove) {
                std::cout << "Crossing found in first segment" << std::endl;
                double t = (c - y_prev_old) / (y_current - y_prev_old);
                double xi = x_prev + t * (x_current - x_prev);
    
                // Ajouter breakpoint à xi avec delta 0
                double deltaAtXi = 0;
                breakpoints[xi] = deltaAtXi;
                std::cout << "Insert breakpoint at xi=" << xi << ", delta=" << deltaAtXi << std::endl;
    
                // Ajuster delta du point actuel
                double remainingDelta = y_current - c;
                it_current->second = remainingDelta;
                std::cout << "Adjust current delta: x=" << x_current
                          << ", remainingDelta=" << remainingDelta << std::endl;
    
            } else {
                if (c > y_prev_old) {
                    // Pas de coupure, ajuster delta
                    double newDelta = y_current - y_prev;
                    it_current->second = newDelta;
                    std::cout << "No crossing zero under c, adjust delta: x=" << x_current
                              << ", newDelta=" << newDelta << std::endl;
                } else {
                    double newDelta = y_current - y_prev_old;
                    it_current->second = newDelta;
                    std::cout << "No crossing, adjust delta: x=" << x_current
                              << ", newDelta=" << newDelta << std::endl;
                }
            }
        } else {
            std::cout << "No element after first node" << std::endl;
        }
    
        // Reconstruire points cumulés
        std::vector<std::pair<double, double>> points;
        double y = 0.0;
        for (auto &kv : breakpoints) {
            y += kv.second;
            points.push_back({kv.first, y});
            std::cout << "Accumulated point: x=" << kv.first << ", y=" << y << std::endl;
        }
    
        // Appliquer max avec c
        for (size_t i = 0; i < points.size() - 1; ++i) {
            double x = points[i + 1].first;
            double y_val = points[i + 1].second;
    
            double y_prev = points[i].second;
            double x_prev = points[i].first;
    
            bool prevUnder = y_prev < c;
            bool currUnder = y_val < c;
    
            std::cout << "\nVisiting segment: [" << x_prev << ", " << x << "] "
                      << "y_prev=" << y_prev << ", y_curr=" << y_val << std::endl;
    
            // Cas où on traverse la ligne y=c
            if ((prevUnder && y_val > c) || (y_prev > c && currUnder)) {
                double t = (c - y_prev) / (y_val - y_prev);
                double xi = x_prev + t * (x - x_prev);
                double deltaAtXi = (y_prev <= c) ? 0 : c - y_prev;
    
                std::cout << "Crossing found at x=" << xi << ", deltaAtXi=" << deltaAtXi << std::endl;
                this->addBreakpoint(xi, deltaAtXi);
    
                if (prevUnder && y_val > c) {
                    double remainingDelta = y_val - c;
                    breakpoints[x] = remainingDelta;
                    points[i + 1].second = c + remainingDelta;
                    std::cout << "Adjusting current delta: x=" << x << ", remainingDelta=" << remainingDelta << std::endl;
                }
            }
    
            // Cas où le point tombe exactement sur c
            if (prevUnder && y_val == c) {
                std::cout << "Breakpoint exactly at c at x =" << x << std::endl;
                breakpoints[x] = 0;
                points[i + 1].second = c;
            }
    
            // Supprimer les nœuds en dessous de c
            if (y_val < c) {
                std::cout << "Deleting node under c: x=" << x << ", y=" << y_val << std::endl;
                breakpoints.erase(x);
            }
        }
    
        std::cout << "--- maxfunction end ---" << std::endl;
    }
    
    
//======================================================================================================
//====================================== verify if f<= g  ==============================================
//=======================================================================================================
    // Vérifie si la fonction est toujours inférieure ou égale à une autre
    bool isLessOrEqual(const PiecewiseLinearFunction& g) const {
        PiecewiseLinearFunction diff = g;
        diff.minus(*this);
        for (const auto& pair : diff.breakpoints) {
            if (diff.evaluate(pair.first) < 0) {
                return false;
            }
        }
        return true;
    }
//======================================================================================================
//======================================  find min/max f in [tinf, tsup]   ==============================
//=======================================================================================================
    // Évaluation du maximum sur un intervalle
    double evaluate_max(double t_inf, double t_sup) const {
        if (t_inf > t_sup) return evaluate(t_inf);
        
        double maxVal = evaluate(t_inf);
        maxVal = std::max(maxVal, evaluate(t_sup));

        auto it_begin = breakpoints.lower_bound(t_inf);
        auto it_end = breakpoints.upper_bound(t_sup);
        
        for (auto it = it_begin; it != it_end; ++it) {
            maxVal = std::max(maxVal, evaluate(it->first));
        }
        return maxVal;
    }
    
    // Évaluation du minimum sur un intervalle
    double evaluate_min(double t_inf, double t_sup) const {
        if (t_inf > t_sup) return evaluate(t_inf);
        
        double minVal = evaluate(t_inf);
        minVal = std::min(minVal, evaluate(t_sup));

        auto it_begin = breakpoints.lower_bound(t_inf);
        auto it_end = breakpoints.upper_bound(t_sup);
        
        for (auto it = it_begin; it != it_end; ++it) {
            minVal = std::min(minVal, evaluate(it->first));
        }
        return minVal;
    }


//=================================================================================================================
//======================================  Construction profile delta function =====================================
//=================================================================================================================

    
    // Fonctions de profil
    static PiecewiseLinearFunction delta_profile(double gap, double a, double b, double c) {
        PiecewiseLinearFunction delta;
        delta.addBreakpoint(a, 0);
        delta.addBreakpoint(b, gap);
        delta.addBreakpoint(c, -gap);
        return delta;
    }
    
    static PiecewiseLinearFunction cba_profile(double cap, double a, double b) {
        PiecewiseLinearFunction cba;
        cba.addBreakpoint(a, 0);
        cba.addBreakpoint(b, cap);
        return cba;
    }
//=================================================================================================================
//======================================  utile pour print/draw python        =====================================
//=================================================================================================================
    // Exportation des points vers un fichier
    void exportFunction(const std::string& filename) const {
        std::ofstream out(filename);
        if (!out) {
            std::cerr << "Erreur: impossible d'ouvrir le fichier " << filename << std::endl;
            return;
        }

        std::vector<std::pair<double, double>> points;
        double currentY = 0.0;
        for (const auto& pair : breakpoints) {
            currentY += pair.second;
            points.push_back({pair.first, currentY});
        }
        
        for (const auto& pair : points) {
            out << pair.first << " " << pair.second << "\n";
        }

        out.close();
        std::cout << "Fonction exportee vers " << filename << std::endl;
    }
//======================================================================================================
//======================================  update global profile    =====================================
//=======================================================================================================
    // Mise à jour de la fonction selon la méthode CBR
    void update_cbr_stmin(double stmin_old, double stmin, double ctmin, double cap_min, double cap_max) {
        double cap = (cap_min > 0) ? cap_max : cap_min;
        if (std::abs(stmin - stmin_old) < EPSILON) return;
        PiecewiseLinearFunction delta;
        if (stmin_old < stmin && stmin < ctmin) {
            double slope = -cap / (ctmin - stmin_old);
            double gap = slope * (stmin - stmin_old);
            delta = delta_profile(gap, stmin_old, stmin, ctmin);
        } else if (stmin_old <= ctmin && ctmin < stmin) {
            double gap = -cap;
            delta = delta_profile(gap, stmin_old, ctmin, stmin);
        } else if (ctmin < stmin_old && stmin_old < stmin) {
            double slope = -cap / (ctmin - stmin);
            double gap = slope * (stmin_old - stmin);
            delta = delta_profile(gap, ctmin, stmin_old, stmin);
        }
        this->sum(delta);
    }

    void update_cbr_ctmin(double ctmin_old, double ctmin, double stmin, double cap_min, double cap_max) {
        double cap = (cap_min > 0) ? cap_max : cap_min;
        if (std::abs(ctmin - ctmin_old) < EPSILON) return;
        PiecewiseLinearFunction delta;
        if (stmin < ctmin_old && ctmin_old < ctmin) {
            double slope = -cap / (ctmin - stmin);
            double gap = slope * (ctmin - ctmin_old);
            delta = delta_profile(gap, stmin, ctmin_old, ctmin);
        } else if (ctmin_old <= stmin && stmin < ctmin) {
            double gap = -cap;
            delta = delta_profile(gap, ctmin_old, stmin, ctmin);
        } else if (ctmin_old < ctmin && ctmin < stmin) {
            double slope = -cap / (ctmin_old - stmin);
            double gap = slope * (ctmin_old - ctmin);
            delta = delta_profile(gap, ctmin_old, ctmin, stmin);
        }
        this->sum(delta);
    }

    void update_cbr_stmax(double stmax_old, double stmax, double ctmax, double cap_min, double cap_max) {
        double cap = (cap_min > 0) ? cap_min : cap_max;
        if (std::abs(stmax - stmax_old) < EPSILON) return;
        PiecewiseLinearFunction delta;
        if (stmax < stmax_old && stmax_old < ctmax) {
            double slope = cap / (ctmax - stmax);
            double gap = slope * (stmax_old - stmax);
            delta = delta_profile(gap, stmax, stmax_old, ctmax);
        } else if (stmax <= ctmax && ctmax < stmax_old) {
            double gap = cap;
            delta = delta_profile(gap, stmax, ctmax, stmax_old);
        } else if (ctmax < stmax && stmax < stmax_old) {
            double slope = cap / (ctmax - stmax_old);
            double gap = slope * (stmax - stmax_old);
            delta = delta_profile(gap, ctmax, stmax, stmax_old);
        }
        this->sum(delta);
    }

    void update_cbr_ctmax(double ctmax_old, double ctmax, double stmax, double cap_min, double cap_max) {
        double cap = (cap_min > 0) ? cap_min : cap_max;
        if (std::abs(ctmax - ctmax_old) < EPSILON) return;
        PiecewiseLinearFunction delta;
        if (stmax < ctmax && ctmax < ctmax_old) {
            double slope = cap / (ctmax_old - stmax);
            double gap = slope * (ctmax_old - ctmax);
            delta = delta_profile(gap, stmax, ctmax, ctmax_old);
        } else if (ctmax <= stmax && stmax < ctmax_old) {
            double gap = cap;
            delta = delta_profile(gap, ctmax, stmax, ctmax_old);
        } else if (ctmax < ctmax_old && ctmax_old < stmax) {
            double slope = cap / (ctmax - stmax);
            double gap = slope * (ctmax - ctmax_old);
            delta = delta_profile(gap, ctmax, ctmax_old, stmax);
        }
        this->sum(delta);
    }

    void update_cbr_cap(double cap_old, double cap, double start, double end) {
        if (std::abs(cap - cap_old) < EPSILON) return;
        double gap = cap - cap_old;
        PiecewiseLinearFunction delta = cba_profile(gap, start, end);
        this->sum(delta);
    }
//======================================================================================================
//======================================  Extract points (x,f(x))   =====================================
//=======================================================================================================
    std::vector<std::pair<double, double>> to_points_cumulative() const {
        std::vector<std::pair<double, double>> points;
        double y = 0.0;
    
        for (const auto& kv : breakpoints) {
            y += kv.second;                // cumul des deltas
            points.emplace_back(kv.first, y); // (x, valeur réelle de f(x))
        }
    
        return points;
    }
    
};