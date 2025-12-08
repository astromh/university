#include <iostream>
#include <limits>

using namespace std;

int ReadPositiveInt(const string& prompt, int MaxValue);
int ReadIntElement(const string& prompt);
int OriginalSum(int** A, int N, int M);

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

    cout << "Now Enter " << N*M << " elements (row by row):\n";
    for (int i=0; i<N; ++i){
        for (int j=0; j<M; ++j){
            //Arr[i][j] = ReadIntElement("Arr["+to_string(i)+"]["+to_string(j)+"] = ");
            cout << "Arr["<<i<<"]["<<j<<"] = ";
            while (!(cin >> Arr[i][j])){
                cout << "[-] Invalid input! Please enter and integer number!\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Arr["<<i<<"]["<<j<<"] = ";
                
            }
        }
    }

    int Sum = OriginalSum(Arr, N, M);
    cout << "\nThe sum of the original matrix is: " << Sum << "\n";

    cout << "\nMatrix:\n";
    for (int i=0; i<N; ++i){
        for (int j=0; j<M; ++j){
            cout << Arr[i][j] << "\t";
        }
        cout << "\n";
    }

    for (int i=0; i<N; ++i){
        delete[] Arr[i];
    }
    delete[] Arr;

    return 0;
}

int OriginalSum(int** A, int N, int M){
    int sum = 0;

    for (int i=0; i<N; ++i){
        for (int j=0; j<M; ++j){
            sum += A[i][j];
        }
    }
    return sum;
}

int ReadPositiveInt(const string& prompt, int MaxValue){
    int x;
    while (true){
        cout << prompt;
        if (!(cin >> x)){
            cout << "[-] Invalid input! Please enter and integer number!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        if (x <= 0 || x > MaxValue){
            cout << "Value x must be between 1 and " << MaxValue << ". Try gain!\n";
            continue;
        }
        return x;
    }

}

int ReadIntElement(const string& prompt){
    int x;
    while (true){
        cout << prompt;
        if (!(cin >> x)){
            cout << "[-] Invalid inoput! Please enter and integer number! \n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        return x;
    }
}