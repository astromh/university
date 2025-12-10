#include <iostream>
#include <limits>
#include <windows.h>

using namespace std;

int ReadPositiveInt(const string& prompt, int MaxValue);
void TransformDifferences(int** A, int N, int M);
WORD MakeBackgroundAttrs(int r, int g, int b);
WORD MakeForegroundAttrs(int r, int g, int b);
int ReadIntElement(const string& prompt);
long long NewSum(int** A, int N, int M);
int OriginalSum(int** A, int N, int M);

int main()
{
    const int MAXN = 100;
    const int MAXM = 100;

    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO startInfo;
    GetConsoleScreenBufferInfo(hStdOut, &startInfo);

    //RGB Changing zone!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    const int BG_R = 200;
    const int BG_G = 33;
    const int BG_B = 20;

    WORD bgAttr = MakeBackgroundAttrs(BG_R, BG_G, BG_B);
    WORD fgAttr = MakeForegroundAttrs(BG_R, BG_G, BG_B);

    WORD CombinedAttr = bgAttr | fgAttr;

    SetConsoleTextAttribute(hStdOut, CombinedAttr);

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

    cout << "\nOriginal Matrix:\n";
    for (int i=0; i<N; ++i){
        for (int j=0; j<M; ++j){
            cout << Arr[i][j] << "\t";
        }
        cout << "\n";
    }

    int Sum = OriginalSum(Arr, N, M);
    cout << "\nThe sum of the original matrix is: " << Sum << "\n";

    TransformDifferences(Arr, N, M);
    cout << "\nTransformed Matrix:\n";
    for (int i=0; i<N; ++i){
        for (int j=0; j<M; ++j){
            cout << Arr[i][j] << "\t";
        }
        cout << "\n";
    }

    long long TransformedSum = NewSum(Arr, N, M);
    cout << "\nThe sum of the new translformed matrix is: " << TransformedSum << "\n";

    for (int i=0; i<N; ++i){
        delete[] Arr[i];
    }
    delete[] Arr;

    SetConsoleTextAttribute(hStdOut, startInfo.wAttributes);
    cout << "\n[Colors restored to original settings]\n";
    return 0;
}

WORD MakeBackgroundAttrs(int r, int g, int b){
    WORD attr = 0;

    if (r > 64) attr |= BACKGROUND_RED;
    if (g > 64) attr |= BACKGROUND_GREEN;
    if (b > 64) attr |= BACKGROUND_BLUE;

    if (r > 200 || g > 200 || b > 200){
        attr |= BACKGROUND_INTENSITY;
    }

    return attr;
 }

 WORD MakeForegroundAttrs(int r, int g, int b){
    double luminance = 0.2126*r + 0.7152*g + 0.0722*b;

    WORD fg = 0;

    if (luminance > 140.0){
        fg = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
    } else {
        fg = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
    }
    return fg;
 }

void TransformDifferences(int** A, int N, int M){
    int total = N*M;

    for (int k=0; k<total-1; ++k){
        int i = k/M; //Row of element k
        int j = k%M; //Column of element k
        int kp = k+1; //Index of the next element
        int ni = kp/M; //Row of the next element
        int nj = kp%M; //Column of the next element

        A[i][j] = A[i][j] - A[ni][nj];

    }
}

long long NewSum(int** A, int N, int M){
    long long new_sum = 0;
    for (int i=0; i<N; ++i){
        for (int j=0; j<M; ++j){
            new_sum += A[i][j];
        }
    }
    return new_sum;
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