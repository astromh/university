#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;
double series_value(double x, double eps, int& N_used);
double library_value(double x);

int main(void) {
    double eps, a, b, h;

    cout << "Enter eps, a, b, h: ";
    if (!(cin>>eps>>a>>b>>h)){
        cout << "[!] Invalid input!";
        return 1;
    }
    if (h == 0.0){
        cout << "[!] h must be non-zero!";
        return 1;
    }

    //print header
    cout << "\nTable 1 (fixed eps over [a, b])\n";
    std::cout << std::left
              << std::setw(12) << "x"
              << std::setw(20) << "S(x) series"
              << std::setw(20) << "F(x) library"
              << std::setw(20) << "Delta |S-F|"
              << std::setw(8)  << "N"
              << "\n";
    std::cout << std::string(12+20+20+20+8, '-') << "\n";

    cout.setf(ios::fixed);
    cout << setprecision(12);

    auto within = [&](double x) {
    const double tiny = 1e-12;
    return (h > 0) ? (x <= b + tiny) : (x >= b - tiny);
    };

    for (double x; within(x); x += h){
        int N= 0;
        double S = series_value(x, eps, N);
        double F = library_value(x);
        double D = fabs(S-F);

        cout << std::setw(12) << x
            << std::setw(20) << S
            << std::setw(20) << F
            << std::setw(20) << D
            << std::setw(8)  << N
            << "\n";

        if (x+h == x) break;
    }

    //Table 2
    double x0;
    cout << "Enter x0: ";
    if (!(cin >> x0)) return 0;

    const double eps_list[] = {1e-1, 1e-2, 1e-3, 1e-4, 1e-6, 1e-8, 1e-9};

        std::cout << "\nTABLE 2 (fixed x0, varying eps)\n";
    cout << std::left
            << std::setw(12) << "eps"
            << std::setw(20) << "S(x0) series"
            << std::setw(20) << "F(x0) library"
            << std::setw(20) << "Delta |S-F|"
            << std::setw(8)  << "N"
            << "\n";
    cout << std::string(12+20+20+20+8, '-') << "\n";

    double F0 = library_value(x0);

    for (double e: eps_list) {
        int N= 0;
        double S = series_value(x0, e, N);
        double D = fabs(S - F0);
        
        cout << std::setw(12) << e
                << std::setw(20) << S
                << std::setw(20) << F0
                << std::setw(20) << D
                << std::setw(8)  << N
                << "\n";
    }
    return 0;
}

double series_value(double x, double eps, int& N_used)
{
    double t = 1.0;
    double S = t;
    int k = 0;
    N_used = 1;

    while(true) {
        double next_t = t*x*((k+2)/((k+1.0)*(k+1.0)));

        S += next_t;
        N_used++;

        // |t_{k+1} - t_k| < eps
        if ((next_t - t) < eps) break;
        
        t = next_t;
        k++;

        if (N_used > 2'000'000) break;
    }

    return S;
}

double library_value(double x) {
    return exp(x)*(1.0+x);
}
