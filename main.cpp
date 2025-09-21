#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;


struct Node {
    int key;          // value stored in this node
    int height;       // height of this node
    Node *left;       // pointer to left child
    Node *right;      // pointer to right child
    Node(int k) : key(k), height(1), left(nullptr), right(nullptr) {}
};

//get height of a node (nullptr means height 0)
int getHeight(Node* n) {
    return n ? n->height : 0;
}

// balance factor = height(left) - height(right)
int getBalance(Node* n) {
    return n ? getHeight(n->left) - getHeight(n->right) : 0;
}

// update node height from children
void updateHeight(Node* n) {
    if (n) n->height = 1 + max(getHeight(n->left), getHeight(n->right));
}

//right rotation (used to balance left-heavy case)
Node* rotateRight(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;
    // rotation
    x->right = y;
    y->left = T2;
    //update heights
    updateHeight(y);
    updateHeight(x);
    return x; // new root after rotation
}

//left rotation (used to balance right-heavy case)
Node* rotateLeft(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;
    //rotation
    y->left = x;
    x->right = T2;
    // update heights
    updateHeight(x);
    updateHeight(y);
    return y; // new root after rotation
}

//balance node after insertion or deletion
Node* balance(Node* node) {
    updateHeight(node);
    int balanceFactor = getBalance(node);

    // Case 1: left-heavy
    if (balanceFactor > 1) {
        // Left-Right case -> rotate left child left first
        if (getBalance(node->left) < 0)
            node->left = rotateLeft(node->left);
        // Left-Left case -> rotate right
        return rotateRight(node);
    }
    // Case 2: right-heavy
    if (balanceFactor < -1) {
        // Right-Left case -> rotate right child right first
        if (getBalance(node->right) > 0)
            node->right = rotateRight(node->right);
        // Right-Right case -> rotate left
        return rotateLeft(node);
    }
    // Already balanced
    return node;
}

// Insert a value into the AVL tree
Node* insert(Node* node, int key) {
    if (!node) return new Node(key); // empty spot found

    if (key < node->key)
        node->left = insert(node->left, key);   // go left
    else if (key > node->key)
        node->right = insert(node->right, key); // go right
    else
        return node; // duplicate value -> do nothing

    return balance(node); // restore AVL balance
}

// find largest node in a subtree (rightmost node)
Node* findMax(Node* node) {
    while (node->right) node = node->right;
    return node;
}

//delete a value from the AVL tree
Node* deleteNode(Node* root, int key) {
    if (!root) return root; // value not found, do nothing

    if (key < root->key)
        root->left = deleteNode(root->left, key);   // search in left subtree
    else if (key > root->key)
        root->right = deleteNode(root->right, key); // search in right subtree
    else {
        // Found the node to delete
        if (!root->left && !root->right) {
            // Case 1: no children
            delete root;
            return nullptr;
        } else if (!root->left) {
            // Case 2: one child (right only)
            Node* temp = root->right;
            delete root;
            return temp;
        } else if (!root->right) {
            // Case 2: one child (left only)
            Node* temp = root->left;
            delete root;
            return temp;
        } else {
            // Case 3: two children
            // Replace with largest from left subtree (predecessor)
            Node* pred = findMax(root->left);
            root->key = pred->key;
            root->left = deleteNode(root->left, pred->key);
        }
    }
    return balance(root); // restore AVL balance
}

// Pre-order traversal: 

//root -> left -> right
void preorder(Node* root, vector<int>& res) {
    if (!root) return;
    res.push_back(root->key);
    preorder(root->left, res);
    preorder(root->right, res);
}

// In-order traversal: 

//left -> root -> right
void inorder(Node* root, vector<int>& res) {
    if (!root) return;
    inorder(root->left, res);
    res.push_back(root->key);
    inorder(root->right, res);
}

// Post-order traversal:

//left -> right -> root
void postorder(Node* root, vector<int>& res) {
    if (!root) return;
    postorder(root->left, res);
    postorder(root->right, res);
    res.push_back(root->key);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    Node* root = nullptr; // start with an empty AVL tree
    string line;
    if (!getline(cin, line)) return 0; // read input line

    stringstream ss(line);
    string token;
    vector<string> commands;
    while (ss >> token) commands.push_back(token);

    // Process all commands except the last one (which is traversal type)
    for (size_t i = 0; i + 1 < commands.size(); i++) {
        string cmd = commands[i];
        char type = cmd[0];           // 'A' or 'D'
        int val = stoi(cmd.substr(1)); // extract number
        if (type == 'A') root = insert(root, val);
        else if (type == 'D') root = deleteNode(root, val);
    }

    // The last command is the traversal request
    string finish = commands.back();
    vector<int> result;
    if (finish == "PRE") preorder(root, result);
    else if (finish == "IN") inorder(root, result);
    else if (finish == "POST") postorder(root, result);

    // Print result
    if (result.empty()) {
        cout << "EMPTY\n";
    } else {
        for (size_t i = 0; i < result.size(); i++) {
            if (i) cout << " ";
            cout << result[i];
        }
        cout << "\n";
    }
    return 0;
}
