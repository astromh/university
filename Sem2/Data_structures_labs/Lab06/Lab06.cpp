#include <iostream>
#include <limits>
#include <vector>
#include <cstdlib>  
#include <iomanip> 
#include <random>
#include <string>

using namespace std;

// ========================== Stats + Table Row ==========================
struct Stats {
    long long cmp = 0;   
    long long swp = 0;   
};

struct ResultRow {
    string name;
    long long cmp;
    long long swp;
};

using SortFn = void(*)(int* arr, int n, bool ascending, Stats& s);

static inline void SwapCount(int& a, int& b, Stats& s) {
    ++s.swp; // CHANGED: mov removed
    int t = a;
    a = b;
    b = t;
}

int ReadPositiveInt(const string& prompt, int MaxValue);

// Sorting algorithms
void BubbleSort(int* arr, int n, bool ascending, Stats& s);
void InsertionSort(int* arr, int n, bool ascending, Stats& s);
void SelectionSort(int* arr, int n, bool ascending, Stats& s);
void ShellSort(int* arr, int n, bool ascending, Stats& s);
void QuickSort(int* arr, int n, bool ascending, Stats& s);

// QuickSort helper
static void QuickSortRec(int* arr, int left, int right, bool ascending, Stats& s);

// Utilities
void PrintMatrix(int** A, int N, int M);
int** CopyMatrix(int** A, int N, int M);
void FreeMatrix(int** A, int N);

// Pipeline
int DigitSortByMethod(int element, SortFn sorter, Stats& s);
Stats RunAndPrint(int** original, int N, int M, const string& name, SortFn sorter);

// Matrix input methods
void ManualInput(int** Arr, int N, int M);
void RandomInput(int** Arr, int N, int M);

// ========================== main ==========================
int main() {
    const int MAXN = 100;
    const int MAXM = 100;

    int N = ReadPositiveInt("Enter number of rows N: ", MAXN);
    int M = ReadPositiveInt("Enter number of columns M: ", MAXM);

    int** Arr = new int*[N];
    for (int i = 0; i < N; ++i) Arr[i] = new int[M];

    char input_value;
    do {
        cout << "\nDo you want to Randomly generate the Matrix or enter it manually? (r/m): ";
        cin >> input_value;
        input_value = (char)tolower((unsigned char)input_value); // CHANGED
    } while (input_value != 'r' && input_value != 'm');

    if (input_value == 'm') ManualInput(Arr, N, M);
    else RandomInput(Arr, N, M);

    cout << "\n\nOriginal Matrix:\n";
    PrintMatrix(Arr, N, M);

    vector<ResultRow> table;

    // Bubble
    {
        Stats st = RunAndPrint(Arr, N, M, "Bubble", BubbleSort);
        table.push_back({"Bubble", st.cmp, st.swp}); // CHANGED
    }

    // Insertion
    {
        Stats st = RunAndPrint(Arr, N, M, "Insertion", InsertionSort);
        table.push_back({"Insertion", st.cmp, st.swp}); // CHANGED
    }

    // Selection
    {
        Stats st = RunAndPrint(Arr, N, M, "Selection", SelectionSort);
        table.push_back({"Selection", st.cmp, st.swp}); // CHANGED
    }

    // Shell
    {
        Stats st = RunAndPrint(Arr, N, M, "Shell", ShellSort);
        table.push_back({"Shell", st.cmp, st.swp}); // CHANGED
    }

    // Quick
    {
        Stats st = RunAndPrint(Arr, N, M, "Quick", QuickSort);
        table.push_back({"Quick", st.cmp, st.swp}); // CHANGED
    }

    cout << "\n\n===== Efficiency Table (comparisons / swaps) =====\n"; // CHANGED
    cout << left << setw(12) << "Method"
         << right << setw(14) << "Comparisons"
         << right << setw(10) << "Swaps" << "\n"; // CHANGED

    for (const auto& r : table) {
        cout << left  << setw(12) << r.name
             << right << setw(14) << r.cmp
             << right << setw(10) << r.swp << "\n"; // CHANGED
    }

    FreeMatrix(Arr, N);
    return 0;
}

// ========================== Sorting Algorithms ==========================

void BubbleSort(int* arr, int n, bool ascending, Stats& s) {
    for (int pass = 0; pass < n - 1; ++pass) {
        for (int j = 0; j < n - 1 - pass; ++j) {
            ++s.cmp;
            bool needSwap = ascending ? (arr[j] > arr[j + 1]) : (arr[j] < arr[j + 1]);
            if (needSwap) SwapCount(arr[j], arr[j + 1], s);
        }
    }
}

void InsertionSort(int* arr, int n, bool ascending, Stats& s) {
    for (int i = 1; i < n; ++i) {
        int key = arr[i];
        int j = i - 1;

        while (j >= 0) {
            ++s.cmp;
            bool needShift = ascending ? (arr[j] > key) : (arr[j] < key);
            if (!needShift) break;

            arr[j + 1] = arr[j];
            --j;
        }

        arr[j + 1] = key;
    }
}

void SelectionSort(int* arr, int n, bool ascending, Stats& s) {
    for (int i = 0; i < n - 1; ++i) {
        int best = i;
        for (int j = i + 1; j < n; ++j) {
            ++s.cmp;
            bool better = ascending ? (arr[j] < arr[best]) : (arr[j] > arr[best]);
            if (better) best = j;
        }
        if (best != i) SwapCount(arr[i], arr[best], s);
    }
}

void ShellSort(int* arr, int n, bool ascending, Stats& s) {
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; ++i) {
            int temp = arr[i];
            int j = i;

            while (j >= gap) {
                ++s.cmp;
                bool needShift = ascending ? (arr[j - gap] > temp) : (arr[j - gap] < temp);
                if (!needShift) break;

                arr[j] = arr[j - gap];
                j -= gap;
            }

            arr[j] = temp;
        }
    }
}

static void QuickSortRec(int* arr, int left, int right, bool ascending, Stats& s) {
    int i = left;
    int j = right;
    int pivot = arr[(left + right) / 2];

    while (true) {
        if (ascending) {
            while (true) { ++s.cmp; if (arr[i] < pivot) ++i; else break; }
            while (true) { ++s.cmp; if (arr[j] > pivot) --j; else break; }
        } else {
            while (true) { ++s.cmp; if (arr[i] > pivot) ++i; else break; }
            while (true) { ++s.cmp; if (arr[j] < pivot) --j; else break; }
        }

        if (i >= j) break;

        SwapCount(arr[i], arr[j], s);
        ++i;
        --j;
    }

    if (left < j) QuickSortRec(arr, left, j, ascending, s);
    if (j + 1 < right) QuickSortRec(arr, j + 1, right, ascending, s);
}

void QuickSort(int* arr, int n, bool ascending, Stats& s) {
    if (n <= 1) return;
    QuickSortRec(arr, 0, n - 1, ascending, s);
}

// ========================== Pipeline ==========================

int DigitSortByMethod(int element, SortFn sorter, Stats& s) {
    int sign = (element < 0 ? -1 : 1);
    element = std::abs(element);

    if (element == 0) return 0;

    int temp = element, count = 0;
    while (temp > 0) { ++count; temp /= 10; }

    vector<int> digits(count);

    for (int i = count - 1; i >= 0; --i) {
        digits[i] = element % 10;
        element /= 10;
    }

    sorter(digits.data(), (int)digits.size(), /*ascending=*/false, s);

    int result = 0;
    for (int d : digits) result = result * 10 + d;

    return sign * result;
}

Stats RunAndPrint(int** original, int N, int M, const string& name, SortFn sorter) {
    Stats s;
    int** B = CopyMatrix(original, N, M);

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            B[i][j] = DigitSortByMethod(B[i][j], sorter, s);
        }
    }

    for (int i = 0; i < N; ++i) {
        sorter(B[i], M, /*ascending=*/true, s);
    }

    cout << "\n\n========== " << name << " sort method ==========\n";
    PrintMatrix(B, N, M);

    FreeMatrix(B, N);
    return s;
}

// ========================== Utilities ==========================

void PrintMatrix(int** A, int N, int M) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            cout << A[i][j] << "\t";
        }
        cout << "\n";
    }
}

int** CopyMatrix(int** A, int N, int M) {
    int** B = new int*[N];
    for (int i = 0; i < N; ++i) {
        B[i] = new int[M];
        for (int j = 0; j < M; ++j) B[i][j] = A[i][j];
    }
    return B;
}

void FreeMatrix(int** A, int N) {
    for (int i = 0; i < N; ++i) delete[] A[i];
    delete[] A;
}

// ========================== Matrix input methods ==========================

void RandomInput(int** Arr, int N, int M) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(0, 9999);

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            Arr[i][j] = dist(gen);
        }
    }
}

void ManualInput(int** Arr, int N, int M) {
    cout << "\nNow enter " << (N * M) << " elements (row by row):\n";
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            cout << "Arr[" << i << "][" << j << "] = ";
            while (!(cin >> Arr[i][j])) {
                cout << "[-] Invalid input, please try again.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Arr[" << i << "][" << j << "] = ";
            }
        }
    }
}

// ========================== Input control ==========================

int ReadPositiveInt(const string& prompt, int MaxValue) {
    int x;
    while (true) {
        cout << prompt;
        string token;
        if (!(cin >> token)) {
            cout << "[-] Invalid input, please try again!\n"; 
            cin.clear();
            continue;
        }

        size_t pos = 0;
        try {
            long long val = stoll(token, &pos);
            if (pos != token.size()) {
                cout << "[-] Invalid input, please try again!\n";
                continue;
            }
            if (val <= 0 || val > MaxValue) {
                cout << "[-] The input must be between 1 and " << MaxValue << ". Try again.\n";
                continue;
            }
            x = (int)val;
            return x;
        } catch (...) {
            cout << "[-] Invalid input, please try again!\n";
            continue;
        }
    }
}