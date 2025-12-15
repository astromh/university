#include <iostream>
#include <limits>
#include <windows.h>

using namespace std;

WORD MakeLastElemForegroundAttrs(int r, int g, int b, WORD baseFg, WORD accentFg);
WORD MakeAccentForegroundAttrs(int r, int g, int b, WORD baseFg);
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
    const int BG_R = 255;
    const int BG_G = 34;
    const int BG_B = 24;

    WORD bgAttr = MakeBackgroundAttrs(BG_R, BG_G, BG_B);

    WORD baseFg   = MakeForegroundAttrs(BG_R, BG_G, BG_B);
    WORD accentFg = MakeAccentForegroundAttrs(BG_R, BG_G, BG_B, baseFg);

    WORD lastFg = MakeLastElemForegroundAttrs(BG_R, BG_G, BG_B, baseFg, accentFg);
    WORD lastElemAttr = bgAttr | lastFg;


    WORD baseAttr   = bgAttr | baseFg;
    WORD accentAttr = bgAttr | accentFg;

    SetConsoleTextAttribute(hStdOut, baseAttr);

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
    SetConsoleTextAttribute(hStdOut, accentAttr);

    cout << "\nTransformed Matrix:\n";
    for (int i=0; i<N; ++i){
        for (int j=0; j<M; ++j){
            if (i == N-1 && j == M-1) {
                SetConsoleTextAttribute(hStdOut, lastElemAttr);
                cout << Arr[i][j] << "\t";
                SetConsoleTextAttribute(hStdOut, accentAttr); // back to accent
            } else {
                cout << Arr[i][j] << "\t";
            }
        }
        cout << "\n";
    }
    SetConsoleTextAttribute(hStdOut, baseAttr);

    SetConsoleTextAttribute(hStdOut, accentAttr);
    long long TransformedSum = NewSum(Arr, N, M);
    cout << "\nThe sum of the new translformed matrix is: " << TransformedSum << "\n";
    SetConsoleTextAttribute(hStdOut, baseAttr);

    for (int i=0; i<N; ++i){
        delete[] Arr[i];
    }
    delete[] Arr;

    SetConsoleTextAttribute(hStdOut, startInfo.wAttributes);
    cout << "\n[+] Colors restored to original settings!\n";
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

 WORD MakeAccentForegroundAttrs(int r, int g, int b, WORD baseFg)
{
    const double luminance = 0.2126*r + 0.7152*g + 0.0722*b;
    const bool lightBg = (luminance > 140.0);

    WORD candidates[4];

    if (lightBg) {
        candidates[0] = FOREGROUND_BLUE;                          // dark blue
        candidates[1] = FOREGROUND_GREEN;                         // dark green
        candidates[2] = FOREGROUND_RED | FOREGROUND_GREEN;        // dark yellow / olive-ish
        candidates[3] = FOREGROUND_RED | FOREGROUND_BLUE;         // dark magenta
    } else {
        candidates[0] = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY; // cyan
        candidates[1] = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;  // yellow
        candidates[2] = FOREGROUND_GREEN | FOREGROUND_INTENSITY;                   // green
        candidates[3] = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;   // magenta
    }

    for (WORD fg : candidates) {
        if (fg != baseFg) return fg;
    }

    return lightBg ? (WORD)(FOREGROUND_BLUE) : (WORD)(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
}

WORD MakeLastElemForegroundAttrs(int r, int g, int b, WORD baseFg, WORD accentFg)
{
    const double luminance = 0.2126*r + 0.7152*g + 0.0722*b;
    const bool lightBg = (luminance > 140.0);

    WORD candidates[4];

    if (lightBg) {
        candidates[0] = 0;                                      // black
        candidates[1] = FOREGROUND_BLUE;                        // dark blue
        candidates[2] = FOREGROUND_RED;                         // dark red
        candidates[3] = FOREGROUND_RED | FOREGROUND_BLUE;       // dark magenta
    } else {
        candidates[0] = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY; // yellow
        candidates[1] = FOREGROUND_RED | FOREGROUND_INTENSITY;                   // bright red
        candidates[2] = FOREGROUND_BLUE | FOREGROUND_INTENSITY;                  // bright blue
        candidates[3] = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY; // magenta
    }

    for (WORD fg : candidates) {
        if (fg != baseFg && fg != accentFg) return fg;
    }
    return candidates[0];
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