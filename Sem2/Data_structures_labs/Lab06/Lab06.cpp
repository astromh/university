#include <iostream>
#include <windows.h>
#include <limits>
#include <cmath>
#include <vector>

using namespace std;

int ReadPositiveInt(const string& prompt, int MaxValue);
int SortDescending(int element);

int main()
{
    const int MAXN = 100;
    const int MAXM = 100;

    int N = ReadPositiveInt("Enter number of rows N: ", MAXN);
    int M = ReadPositiveInt("Enter number of columns M: ", MAXM);

    int** Arr = new int*[N];
    for (int i=0; i<N; ++i){
        Arr[i] = new int[M];
    }

    cout << "\n Now enter " << (N*M) << " elements (row by row): ";
    for (int i=0; i<N; ++i){
        for (int j=0; j<M ; ++j){
            cout << "Arr[" << i << "][" << j << "] = ";
            while (!(cin >> Arr[i][j])) {
                cout << "[-] Invalid input please try again. \n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Arr["<<i<<"]["<<j<<"] = ";
            }
        }
    }

    cout << "\n\n Original Matrix: \n";
    for (int i=0; i<N; ++i){
        for (int j=0; j<M; ++j) {
            cout << Arr[i][j] << "\t";
        }
        cout << "\n";
    }

    for (int i=0; i<N; ++i){
        for (int j=0; j<M; ++j){
            Arr[i][j] = SortDescending(Arr[i][j]);
        }
    }

    cout << "\n\n New Matrix: \n";
    for (int i=0; i<N; ++i){
        for (int j=0; j<M; ++j) {
            cout << Arr[i][j] << "\t";
        }
        cout << "\n";
    }

    for (int i=0; i<N; ++i){
        delete[] Arr[i];
    }
    delete[] Arr;

}

int SortDescending(int element){

    element = abs(element);
    if (element == 0) return 0;

    int temp = element, count = 0;
    while (temp > 0) {
        ++count;
        temp /= 10;
    }

    vector<int> digits(count);

    for (int i = count -1; i >= 0; i--){
        digits[i] = element % 10;
        element /= 10;
    }

    int n = (int)digits.size();

    //Bubble sort
    for (int i=0; i<n-1; ++i){
        for (int j=0; j<n-1-i; ++j) {
            if (digits[j] < digits[j+1]) {
                int t = digits[j];
                digits[j] = digits[j+1];
                digits[j+1] = t;
            } 
        }
    }

    int result = 0;

    for (int i=0; i<n; ++i) {
        result = result * 10 + digits[i];
    }
    return result;
}

int ReadPositiveInt(const string& prompt, int MaxValue) {
    int x;
    while (true) {
        cout << prompt;
        if (!(cin >> x)) {
            cout << "[-] Invalid input please try again!";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        if (x <= 0 || x > MaxValue){
            cout << "[-] The input must be between 1 and " << MaxValue << ". Try again. \n";
            continue;
        }
        return x;
    }
}

