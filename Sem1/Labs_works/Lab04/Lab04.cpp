#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>

using namespace std;

double series_value(double x, double eps, int& N_used);
double library_value(double x);

int main() {
    double eps, a, b, h;

    cout << "Enter eps, a, b, h: ";
    if (!(cin >> eps >> a >> b >> h)) {
        cout << "[!] Invalid input!\n";
        return 1;
    }

    if (!(eps > 0.0) || !isfinite(eps)) {
        cout << "[!] eps must be a positive, finite number.\n";
        return 1;
    }
    if (!isfinite(a) || !isfinite(b) || !isfinite(h)) {
        cout << "[!] a, b, h must be finite numbers.\n";
        return 1;
    }

    if (h == 0.0) {
        cout << "[!] Step h must be non-zero.\n";
        return 1;
    }

    if ((b > a && h < 0) || (b < a && h > 0)) {
        cout << "[!] Step h must lead from a to b.\n";
        return 1;
    }

    cout << "\nTABLE 1: x, S(x) (series), F(x) (library), |S - F|, N\n\n";
    cout << setw(10) << "x"
         << setw(20) << "S(x)"
         << setw(20) << "F(x)"
         << setw(20) << "|S - F|"
         << setw(8)  << "N" << '\n';

    cout << string(78, '-') << '\n';

    auto within = [&](double x) {
        const double tiny = 1e-12;
        if (h > 0)
            return x <= b + tiny;
        else
            return x >= b - tiny;
    };

    double span = fabs(b - a);
    if (span > 0 && fabs(h) > 0 && span / fabs(h) > 1e7) {
        cout << "[!] Too many steps (" << span / fabs(h)
        << "). Choose a larger |h|.\n";
        return 1;
    }

    for (double x = a; within(x); x += h) {
        int N = 0;
        double S = series_value(x, eps, N);
        double F = library_value(x);
        double D = fabs(S - F);

        cout << fixed << setprecision(4);
        cout << setw(10) << x
             << setw(20) << setprecision(10) << S
             << setw(20) << setprecision(10) << F
             << setw(20) << setprecision(10) << D
             << setw(8)  << N << '\n';

        if (x + h == x) {
            cout << "[!] Step h is too small to advance x; aborting the sweep.\n";
            break;
        }
    }

    double x0;
    cout << "\nEnter x0 for detailed calculations: ";
    if (!(cin >> x0)) {
        cout << "[!] Invalid input for x0.\n";
        return 1;
    }

    if (!isfinite(x0)) {
        cout << "[!] x0 must be a finite number.\n";
        return 1;
    }

    vector<double> eps_list = {1e-1, 1e-2, 1e-3, 1e-4, 1e-5};

    double F0 = library_value(x0);

    if (!isfinite(F0)) {
        cout << "[!] Warning: F(x0) overflow/underflow; results may be inf or NaN.\n";
    }

    cout << "\nTABLE 2: x0 = " << x0
         << "\n      eps, S(x0), F(x0), |S - F|, N\n\n";

    cout << setw(12) << "eps"
         << setw(20) << "S(x0)"
         << setw(20) << "F(x0)"
         << setw(20) << "|S - F|"
         << setw(8)  << "N" << '\n';

    cout << string(80, '-') << '\n';

    for (double e : eps_list) {
        int N = 0;
        double S0 = series_value(x0, e, N);
        double D0 = fabs(S0 - F0);

        cout << scientific << setprecision(1);
        cout << setw(12) << e;

        cout << fixed << setprecision(10);
        cout << setw(20) << S0
             << setw(20) << F0
             << setw(20) << D0
             << setw(8)  << N << '\n';
    }

    return 0;
}

double series_value(double x, double eps, int& N_used) {

    if (!isfinite(x)) {
        N_used = 0;
        return std::numeric_limits<double>::quiet_NaN();
    }

    double t = 1.0;    
    double S = t;     
    int k = 0;         
    N_used = 1;

    while (true) {
        double next_t = t * x * (k + 2) / ((k + 1.0) * (k + 1.0));

        if (!isfinite(next_t)) {
        S += next_t;
        ++N_used;
        break;
        }

        S += next_t;
        N_used++;

        if (fabs(next_t - t) < eps)
            break;

        t = next_t;
        k++;

        if (N_used > 2000000)
            break;
    }

    return S;
}

double library_value(double x) {
    return exp(x) * (1.0 + x);
}
