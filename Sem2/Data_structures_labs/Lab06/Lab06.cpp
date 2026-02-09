#include <iostream>
#include <limits>
#include <cmath>
#include <vector>
#include <random>

using namespace std;

int ReadPositiveInt(const string& prompt, int MaxValue);
int BubbleSortDescending(int element);
void BubbleSortAscending(int* arr, int size);
void ManualInput(int** Arr, int N, int M);
void RandomInput(int** Arr, int N, int M);


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

    char input_value;
    do {
        cout << "\nDo you want to Randomly generate the Matrix or enter it manulay? (R/M): ";
        cin >> input_value;
    } while (input_value != 'R' && input_value != 'M');


    // Check the condtion for random or munal function call
    // do-While loop while the input is not M or R
    //if (M) {
    // Manual function
    //} elif (R) {
    // Random function
    //}

    if (input_value == 'M') {
        ManualInput(Arr, N, M);
    } 
    else if (input_value == 'R') {
        RandomInput(Arr, N, M);
    }


    // Printing out the oroginal input matrix
    cout << "\n\n Original Matrix: \n";
    for (int i=0; i<N; ++i){
        for (int j=0; j<M; ++j) {
            cout << Arr[i][j] << "\t";
        }
        cout << "\n";
    }

    // Sorting the numbers (task 1)
    for (int i=0; i<N; ++i){
        for (int j=0; j<M; ++j){
            Arr[i][j] = BubbleSortDescending(Arr[i][j]);
        }
    }

    // Sorting the rows (task 2)
    for (int i=0; i<N; ++i) {
        BubbleSortAscending(Arr[i], M);
    }

    // Printing out the final matrix
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

    return 0;
}

void RandomInput(int** Arr, int N, int M) {
    random_device rd;
    mt19937 gen(rd());

    uniform_int_distribution<int> dist(0, 9999);
    
    for (int i=0; i<N; ++i){
        for (int j=0; j<M; ++j){
            Arr[i][j] = dist(gen);
        }
    }
}

void ManualInput(int** Arr, int N, int M) {
    cout << "\n Now enter " << (N*M) << " elements (row by row): \n";
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
}

// Bubble sort for sorting elements in each row so the row is in an ascending order
void BubbleSortAscending(int* arr, int size) {
    for (int i=0; i<size-1; ++i){
        for (int j=0; j<size-1-i; ++j){
            if (arr[j] > arr[j+1]){
                int t = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = t;
        }
        }
    }
}

// Function for extracting the digits and sorting them in a descending order
int BubbleSortDescending(int element){

    int sign = (element < 0 ? -1 : 1);
    element = abs(element);
    if (element == 0) return 0;

    // Getting the size of the integer given
    int temp = element, count = 0;
    while (temp > 0) {
        ++count;
        temp /= 10;
    }

    vector<int> digits(count);

    // Breaking the integer into digits for sorting
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

    // Building the integer from the "digits" array
    for (int i=0; i<n; ++i) {
        result = result * 10 + digits[i];
    }
    return sign*result;
}

// Function for controlling the input strictly
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

