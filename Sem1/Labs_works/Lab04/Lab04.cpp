#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>

using namespace std;

// Computes S(x) = sum_{k=0}^{∞} x^k (k+1) / k!
// using the recurrence relation and stopping rule |t_{k+1} - t_k| < eps.
double series_value(double x, double eps, int& N_used);

// Library value: F(x) = e^x (1 + x)
double library_value(double x);

int main() {
    double eps, a, b, h;

    cout << "Enter eps, a, b, h: ";
    if (!(cin >> eps >> a >> b >> h)) {
        cout << "[!] Invalid input!\n";
        return 1;
    }

    if (h == 0.0) {
        cout << "[!] Step h must be non-zero.\n";
        return 1;
    }

    // Direction check: step sign must match interval direction
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

    // Helper to check if current x is still within [a, b] (with small tolerance)
    auto within = [&](double x) {
        const double tiny = 1e-12;
        if (h > 0)
            return x <= b + tiny;
        else
            return x >= b - tiny;
    };

    // Loop over x from a to b with step h
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
    }

    // Second experiment: fixed x0, different eps
    double x0;
    cout << "\nEnter x0 for detailed calculations: ";
    if (!(cin >> x0)) {
        cout << "[!] Invalid input for x0.\n";
        return 1;
    }

    // You can change this list if your lecturer wants specific eps values.
    vector<double> eps_list = {1e-1, 1e-2, 1e-3, 1e-4, 1e-5};

    double F0 = library_value(x0);

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
    // First term: a0 = x^0 * (0+1) / 0! = 1
    double t = 1.0;    // current term a_k
    double S = t;      // partial sum
    int k = 0;         // current k
    N_used = 1;

    while (true) {
        // Recurrence:
        // a_{k+1} = a_k * x * (k+2) / (k+1)^2
        double next_t = t * x * (k + 2) / ((k + 1.0) * (k + 1.0));

        S += next_t;
        N_used++;

        // Stopping condition: |a_{k+1} - a_k| < eps
        if (fabs(next_t - t) < eps)
            break;

        t = next_t;
        k++;

        // Safety guard to avoid infinite loop on bad eps, etc.
        if (N_used > 2000000)
            break;
    }

    return S;
}

double library_value(double x) {
    return exp(x) * (1.0 + x);
}
