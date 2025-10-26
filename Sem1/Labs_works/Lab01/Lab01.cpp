#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;
double F(double x, double a);

int main()
{
    double a;
    bool valid;
    cout << fixed << setprecision(4); 
    do {
        cout << "Enter a: ";
        cin >> a;

        if (cin.fail()) // copied
        {
            cout << "Error: please enter a number!" << endl;
            cin.clear();             // copied
            cin.ignore(10000, '\n'); // copied
            valid = false;        
        }
        else if (a < -1e6 || a > 1e6){
            cout << "[!] Error: value out of range!\n";
            valid = false;
        } 
        else 
        {
            valid = true;
        }


    } while (!valid);
    


    double x_start = -1.7;
    double x_end = 3.3;
    double dx = 0.5;

    cout << setw(5)  << "N"
        << setw(12) << "x"
        << setw(12) << "a"
        << setw(15) << "F(x,a)" << endl;

    cout << string(44, '-') << endl;


    int row = 1;
    for (double x = x_start; x <= x_end + 1e-9; x += dx)
    {
        double y = F(x, a);

        cout << setw(5)  << row << ")"
        << setw(12) << x 
        << setw(12) << a;

        if (isnan(y) || isinf(y)) {
            cout << setw(15) << "ERROR!";
        } else {
            cout << setw(15) << y;
        }
        cout << endl;
        row++;
    }

    return 0;

}

double F(double x, double a)
{
    double inner = a + tan(a + x/3);
    if (inner < 0) 
        return NAN;

    double denom = 1 - sqrt(inner);
    if (fabs(denom) < 1e-9) 
        return NAN;

    return 1 / denom;

}