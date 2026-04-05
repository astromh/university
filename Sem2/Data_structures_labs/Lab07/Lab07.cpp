#include <iostream>
#include <limits>
#include <vector>
#include <cstdlib>  
#include <iomanip> 
#include <random>
#include <string>

using namespace std;

int ReadPositiveInt(const string& prompt, int MaxValue);
vector<int> SpiralOrder(int** Arr, int N, int M);
vector<int> CopySort(const vector<int>&Arr);
void BinarySearch(const vector<int>&Arr, int n);
void GenerateMatrix(int** Arr, int N, int M);
void PrintMatrix(int** A, int N, int M);
void LinearSearch(const vector<int>&Arr, int n);

int main() {
    const int MAXN = 100;
    const int MAXM = 100;

    int N = ReadPositiveInt("Enter number of rows N: ", MAXN);
    int M = ReadPositiveInt("Enter number of columns M: ", MAXM);

    int** Arr = new int*[N];
    for (int i=0; i<N; ++i) Arr[i] = new int[M];

    GenerateMatrix(Arr, N, M);
    cout << "\nThe Original Matrix: \n";
    PrintMatrix(Arr, N, M);

    vector<int> SpiralMatrix = SpiralOrder(Arr, N, M);

    cout << "[+] The spiral traversal of the matrix: \n";
    for (int i=0; i<SpiralMatrix.size(); ++i) {
        cout << SpiralMatrix[i] << "\t";
    }
    cout << "\n";

    int n;
    // Needs to be filtered
    cout << "[?] Enter the number you want to search for: ";
    cin >> n;

    LinearSearch(SpiralMatrix, n);

    cout << "\n[+] The sorted form of the matrix: \n";
    vector<int> SortedMatrix = CopySort(SpiralMatrix);
    for (int i=0; i<SortedMatrix.size(); ++i) {
        cout << SortedMatrix[i] << "\t";
    }
    cout << "\n";

    BinarySearch(SortedMatrix, n);

    for (int i = 0; i < N; ++i) delete[] Arr[i];
    delete[] Arr;

    return 0;
}

void FoundNumber(bool found, int comparisons, int position) {
    if (found) {
        cout << "\n[+] The number was found!";
        cout << "\n[+] Position in spiral array: " << position;
        cout << "\n[+] Number of comparisons: " << comparisons << "\n";
    } else {
        cout << "\n[-] The number was not found!(";
        cout << "\n[+] Number of comparisons: " << comparisons << "\n";
    }
}

vector<int> SpiralOrder(int** Arr, int N, int M) {
    int left= 0;
    int right = M-1;
    int top = 0;
    int bottom = N-1;

    vector<int> spiral;

    while (top <= bottom && left<=right) {

        for (int i=left; i<=right;++i) {
            spiral.push_back(Arr[top][i]);
        }
        top++;

        for(int i=top; i<=bottom; ++i) {
            spiral.push_back(Arr[i][right]);
        }
        right--;

        if (top <= bottom) {
            for (int i=right; i>=left; --i) {
                spiral.push_back(Arr[bottom][i]);
            }
            bottom--;
        }

        if (left <= right) {
            for (int i=bottom; i>=top; --i) {
                spiral.push_back(Arr[i][left]);
            }
            left++;
        }
    }

    return spiral;

}

void BinarySearch(const vector<int>&Arr, int n) {
    cout << "\nBinary Search: ";
    int left = 0;
    int right = Arr.size() - 1;
    int comparisons = 0;
    int position = -1;
    bool found = false;


    while (left<=right) {
        int mid = left + (right - left) / 2;
        comparisons++;

        if (Arr[mid] == n) {
            found = true;
            position = mid;
            break;
        }
        else if (Arr[mid] > n) {
            right = mid-1;
        }
        else if (Arr[mid] < n) {
            left = mid+1;
        }
    }

    FoundNumber(found, comparisons, position);
}

void LinearSearch(const vector<int>&Arr, int n) {
    cout << "\nLinear Search: ";
    int comparisons = 0;

    bool found = false;
    int position = -1;
    for (int i=0; i<Arr.size(); ++i) {
        comparisons++;
        if (Arr[i] == n) {
            found = true;
            position = i;
            break;
        }
    }

    FoundNumber(found, comparisons, position);
}

int ReadPositiveInt(const string& prompt, int MaxValue) {
    int x;
    while (true) {
        cout << prompt;
        string token;
        if (!(cin >> token)) {
            cout << "[!] Invalid input, Please try again!\n";
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

void GenerateMatrix(int** Arr, int N, int M) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(0, 9999);

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            Arr[i][j] = dist(gen);
        }
    }
}

vector<int> CopySort(const vector<int>& Arr) {
    vector<int> NewArray = Arr;

    for (int i = 1; i < NewArray.size(); ++i) {
        int key = NewArray[i];
        int j = i - 1;

        while (j >= 0 && NewArray[j] > key) {
            NewArray[j + 1] = NewArray[j];
            j = j - 1;
        }
        NewArray[j + 1] = key;
    }

    return NewArray;
}

void PrintMatrix(int** A, int N, int M) {
    for (int i=0; i<N; ++i) {
        for (int j=0; j<M; ++j) {
            cout << A[i][j] << "\t";
        }
        cout << "\n";
    }
    cout << "\n";
}

