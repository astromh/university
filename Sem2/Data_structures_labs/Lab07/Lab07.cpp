#include <iostream>
#include <limits>
#include <vector>
#include <cstdlib>  
#include <iomanip> 
#include <algorithm>
#include <random>
#include <string>

using namespace std;

struct AVLNode {
    int value;
    int height;
    int count;
    AVLNode* left;
    AVLNode* right;

    AVLNode(int v) : value(v), height(1), count(1), left(nullptr), right(nullptr) {}
};

int ReadPositiveInt(const string& prompt, int MaxValue);
vector<int> SpiralOrder(int** Arr, int N, int M);
vector<int> CopySort(const vector<int>&Arr);
vector<int> ReadTargetList(int size);
void BinarySearch(const vector<int>&Arr, int n);
void GenerateMatrix(int** Arr, int N, int M);
void PrintMatrix(int** A, int N, int M);
void LinearSearch(const vector<int>&Arr, int n);

int ReadAnyInt(const string& prompt);
vector<int> ReadTargetList(int size);

int GetHeight(AVLNode* node);
int GetBalance(AVLNode* node);
void UpdateHeight(AVLNode* node);

AVLNode* RotateRight(AVLNode* y);
AVLNode* RotateLeft(AVLNode* x);

AVLNode* InsertAVL(AVLNode* node, int value);
bool SearchAVL(AVLNode* root, int value, int& comparisons);

void PrintInOrder(AVLNode* root);
void PrintPreOrder(AVLNode* root);
void PrintPostOrder(AVLNode* root);

void DeleteTree(AVLNode* root);

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
    cout << "\n[+] The sorted form of the spiral array: \n";
    vector<int> SortedMatrix = CopySort(SpiralMatrix);
    for (int i = 0; i < SortedMatrix.size(); ++i) {
        cout << SortedMatrix[i] << "\t";
    }
    cout << "\n";

    AVLNode* root = nullptr;
    for (int i = 0; i < SpiralMatrix.size(); ++i) {
        root = InsertAVL(root, SpiralMatrix[i]);
    }

    vector<int> targets = ReadTargetList(SpiralMatrix.size());

    for (int i = 0; i < targets.size(); ++i) {
        int target = targets[i];

        cout << "\n\n==============================\n";
        cout << "[+] Searching for " << target << "\n\n";

        LinearSearch(SpiralMatrix, target);
        BinarySearch(SortedMatrix, target);

        int avlComparisons = 0;
        bool avlFound = SearchAVL(root, target, avlComparisons);

        cout << "\nAVL Tree Search: ";
        if (avlFound) {
            cout << "\n[+] The number was found!";
            cout << "\n[+] Number of comparisons: " << avlComparisons << "\n";
        } else {
            cout << "\n[-] The number was not found!";
            cout << "\n[+] Number of comparisons: " << avlComparisons << "\n";
        }
    }

    cout << "\n[+] AVL Tree Inorder Traversal:\n";
    PrintInOrder(root);
    cout << "\n";

    cout << "\n[+] AVL Tree Preorder Traversal:\n";
    PrintPreOrder(root);
    cout << "\n";

    cout << "\n[+] AVL Tree Postorder Traversal:\n";
    PrintPostOrder(root);
    cout << "\n";

    DeleteTree(root);
    for (int i = 0; i < N; ++i) {
        delete[] Arr[i];
    }
    delete[] Arr;

    return 0;
}

void FoundNumber(bool found, int comparisons, int position, const string& positionLabel) {
    if (found) {
        cout << "\n[+] The number was found!";
        if (position != -1) {
            cout << "\n[+] " << positionLabel << ": " << position;
        }
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

vector<int> ReadTargetList(int size) {
    int k = ReadPositiveInt("[?] How many numbers do you want to search for: ", size);

    vector<int> targets;
    targets.reserve(k);

    for (int i = 0; i < k; ++i) {
        string prompt = "[?] Enter target #" + to_string(i + 1) + ": ";
        targets.push_back(ReadAnyInt(prompt));
    }

    return targets;
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

    FoundNumber(found, comparisons, position, "Position in sorted array");
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

    FoundNumber(found, comparisons, position, "Position in Spiral array");
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

int ReadAnyInt(const string& prompt) {
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

            if (val < numeric_limits<int>::min() || val > numeric_limits<int>::max()) {
                cout << "[-] Number is out of int range, please try again!\n";
                continue;
            }

            x = static_cast<int>(val);
            return x;
        } catch (...) {
            cout << "[-] Invalid input, please try again!\n";
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


// ==================================================== TREE ===========================================

int GetHeight(AVLNode* node) {
    if (node == nullptr) return 0;
    return node->height;
}

void UpdateHeight(AVLNode* node) {
    if (node != nullptr) {
        int LeftHeight = GetHeight(node->left);
        int RightHeight = GetHeight(node->right);
        node->height = 1 + max(LeftHeight, RightHeight);
    }
}

int GetBalance(AVLNode* node) {
    if (node == nullptr) return 0;
    return GetHeight(node->left) - GetHeight(node->right);
}

AVLNode* RotateRight(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    UpdateHeight(y);
    UpdateHeight(x);

    return x;
}

AVLNode* RotateLeft(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    UpdateHeight(x);
    UpdateHeight(y);

    return y;
}

AVLNode* InsertAVL(AVLNode* node, int value) {
    if (node == nullptr) {
        return new AVLNode(value);
    }

    if (value < node->value) {
        node->left = InsertAVL(node->left, value);
    }
    else if (value > node->value) {
        node->right = InsertAVL(node->right, value);
    }
    else {
        node->count++;
        return node;
    }

    UpdateHeight(node);

    int balance = GetBalance(node);

    // Left-Left
    if (balance > 1 && value < node->left->value) {
        return RotateRight(node);
    }

    // Right-Right
    if (balance < -1 && value > node->right->value) {
        return RotateLeft(node);
    }

    // Left-Right
    if (balance > 1 && value > node->left->value) {
        node->left = RotateLeft(node->left);
        return RotateRight(node);
    }

    // Right-Left
    if (balance < -1 && value < node->right->value) {
        node->right = RotateRight(node->right);
        return RotateLeft(node);
    }

    return node;
}

bool SearchAVL(AVLNode* root, int value, int& comparisons) {
    AVLNode* current = root;

    while (current != nullptr) {
        comparisons++;

        if (value == current->value) {
            return true;
        }
        else if (value < current->value) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }

    return false;
}

void PrintInOrder(AVLNode* root) {
    if (root == nullptr) return;

    PrintInOrder(root->left);
    for (int i = 0; i < root->count; ++i) {
        cout << root->value << "\t";
    }
    PrintInOrder(root->right);
}

void PrintPreOrder(AVLNode* root) {
    if (root == nullptr) return;

    for (int i = 0; i < root->count; ++i) {
        cout << root->value << "\t";
    }
    PrintPreOrder(root->left);
    PrintPreOrder(root->right);
}

void PrintPostOrder(AVLNode* root) {
    if (root == nullptr) return;

    PrintPostOrder(root->left);
    PrintPostOrder(root->right);
    for (int i = 0; i < root->count; ++i) {
        cout << root->value << "\t";
    }
}

void DeleteTree(AVLNode* root) {
    if (root == nullptr) return;

    DeleteTree(root->left);
    DeleteTree(root->right);
    delete root;
}