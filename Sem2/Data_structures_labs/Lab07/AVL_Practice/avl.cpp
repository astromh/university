#include <iostream>
#include <vector>

using namespace std;

struct AVLNode {
    int value;
    int height;
    int count;
    AVLNode* left;
    AVLNode* right;

    AVLNode(int v): value(v), height(1), count(1), left(nullptr), right(nullptr) {}
};

int GetHeight(AVLNode* node);
int GetBalance(AVLNode* node);
void UpdateHeight(AVLNode* node); 

AVLNode* InsertAVL(AVLNode* node, int value);
bool SearchAVL(AVLNode* root, int value);

AVLNode* RotateRight(AVLNode* y);
AVLNode* RotateLeft(AVLNode* x);

int main() {
    return 0;
}

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

    // left-left
    if (balance > 1 && value < node->left->value) {
        return RotateRight(node);
    }

    // right-right
    if (balance < -1 && value > node->right->value) {
        return RotateLeft(node);
    }

    // left-right
    if (balance > 1 && value > node->left->value) {
        node->left = RotateLeft(node->left);
        return RotateRight(node);
    }

    // right-left
    if (balance < -1 && value < node->right->value) {
        node->right = RotateRight(node->right);
        return RotateLeft(node);
    }

    return node;
}