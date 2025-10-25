#include <windows.h>
#include <string.h>
#include <iostream>
#include <limits>
#include <cctype>
#include <cmath>

using namespace std;
int* get_input(void); 
double* calculate(int R, int G, int B);

const double EPS = 1e-9;

int main(void)
{

    int* RGB = get_input();
    // R=RGB[0], G=RGB[1], B=RGB[2]

    double* HLS = calculate(RGB[0], RGB[1], RGB[2]);
    cout << "RGB ---> HLS: " << HLS[0] << " " 
    << HLS[1] << " " << HLS[2];

    delete[] HLS;
    delete[] RGB;
    return 0;
}

int* get_input(void)
{
    int* arr = new int[3];
    const char* names[3] = {"Red", "Green", "Blue"};
    string input;

    for (int i = 0; i < 3; i++) {
        while (true) {
            cout << "Enter value for " << names[i] << " (0 - 255): ";
            getline(cin, input); 

            if (input.empty()) {
                cout << "[!] Empty input. Try again.\n";
                continue;
            }

            bool only_digits = true;
            for (char c : input) {
                if (!isdigit(static_cast<unsigned char>(c))) {
                    only_digits = false;
                    break;
                }
            }

            if (!only_digits) {
                cout << "[!] Invalid input. Only digits (0 - 255) allowed.\n";
                continue;
            }

            int value = stoi(input);

            if (value < 0 || value > 255) {
                cout << "[!] Value out of range (0 - 255).\n";
                continue;
            }

            arr[i] = value;
            break;
        }
    }

    return arr;
}


double* calculate(int R, int G, int B)
{
    double* arr = new double[3];

    double inner = (double)R * R + (double)G * G + (double)B * B
                   - (double)B * G - (double)B * R - (double)G * R;

    if (inner < 0.0 && inner > -EPS) inner = 0.0;

    if (inner < 0.0) {
        arr[2] = 0.0;
    } else {
        arr[2] = sqrt(inner); 
    }

    arr[1] = ((double)R + (double)G + (double)B) / 3.0;

    if (arr[2] < EPS) {
        arr[0] = 0.0;
    } else {
        double arg = sqrt(3.0 / 2.0) * ((double)G - (double)R) / arr[2];

        if (!isfinite(arg)) {
            arr[0] = 0.0;
        } else {
            if (arg > 1.0) arg = 1.0;
            if (arg < -1.0) arg = -1.0;
            arr[0] = asin(arg);
        }
    }

    return arr;
}

