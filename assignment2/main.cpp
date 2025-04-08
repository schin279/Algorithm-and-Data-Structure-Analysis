#include <iostream>
#include <sstream>
using namespace std;

// AVL tree structure
struct Node {
    int key;
    struct Node* left, * right;
    int height; // height of the node
};

int max(int a, int b);

// function to get height of the tree
int height(Node *N) {
    if (N == NULL)
        return 0;
    return N->height;
}

// function to get maximum of two integers
int max(int a, int b) {
    return (a > b)? a : b;
}

/* helper function that allocates a new node with the given data
and NULL left and right pointers. */
Node* newNode(int key) {
    Node* node = new Node();
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->height = 1; // new node is initially added at leaf

    return(node);
}

// function to right rotate subtree rooted with y
Node *rightRotate(Node *y) {
    Node *x = y->left;
    Node *T2 = x->right;

    // perform rotation
    x->right = y;
    y->left = T2;

    // update heights
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    // return new root
    return x;
}

// function to left rotate subtree rooted with x
Node *leftRotate(Node *x) {
    Node *y = x->right;
    Node *T2 = y->left;

    // perform rotation
    y->left = x;
    x->right = T2;

    // update heights
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    // return new root
    return y;
}

// get balance factor of node N
int getBalance(Node *N) {
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

/* recursive function to insert a key in the subtree rooted with node
and returns the new root of the subtree*/
Node* insert(Node* node, int key) {
    // 1. perform the normal BST insertion
    if (node == NULL)
        return newNode(key);

    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    else // equal keys not allowed in BST
        return node;
    
    // 2. update height of this ancestor node
    node->height = 1 + max(height(node->left), height(node->right));

    /* 3. get the balance factor of this ancestor node 
    to check whether this node became unbalanced */
    int balance = getBalance(node);

    // if this node becomes unbalanced, then there are 4 cases

    // left left case
    if (balance > 1 && key < node->left->key)
        return rightRotate(node);
    
    // right right case
    if (balance < -1 && key > node->right->key)
        return leftRotate(node);
    
    // left right case
    if (balance > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // right left case
    if (balance < -1 && key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    // return the unchanged node pointer
    return node;
}

/* given a non-empty BST,
return node with maximum key value found in that tree.*/
Node * maxValueNode(Node* node) {
    Node* current = node;
    // loop down to find the leftmost left
    while (current->right != NULL)
        current = current->right;
    return current;
}

/* recursive function to delete a node with given key from subtree with given root.
returns root of the modified subtree. */
Node* deleteNode(Node* root, int key) {
    // 1. perform standard BST delete
    if (root == NULL)
        return root;
    
    /* if key to be deleted is smaller than root's key,
    then it lies in left subtree */
    if (key < root->key)
        root->left = deleteNode(root->left, key);

    /* if key to be deleted is greater than root's key,
    then it lies in right subtree */
    else if (key > root->key)
        root->right = deleteNode(root->right, key);

    /* if key is same as root's key,
    then this is the node to be deleted*/
    else {
        // node with only one child or no child
        if ((root->left == NULL) || (root->right == NULL)) {
            Node *temp = (root->left != NULL) ? root->left : root->right;
            // no child case
            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else // one child case
                *root = *temp; // copy the contents of the non-empty child
            delete temp; // delete the copied node
        } else {
            /* node with two children:
            get the inorder successor */
            Node* temp =  maxValueNode(root->left);

            // copy the inorder successor's data to this node
            root->key = temp->key;

            // delete the inorder successor
            root->left = deleteNode(root->left, temp->key);
        }
    } 

    // if the tree had only one node, then return
    if (root == NULL)
        return root;

    // 2. update height of current node
    root->height = 1 + max(height(root->left), height(root->right));

    // 3. get the balance factor of this node
    // to check whether this node became unbalanced
    int balance = getBalance(root);

    // if this node becomes unbalanced, there are 4 cases
    // left left case
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);
    
    // left right case
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // right right case
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);
    
    // right left case
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
    return root;
}

// function to print preorder traversal of the tree
void preOrder(Node *root) {
    if (root == NULL)
        return;
    
    // first print data of root
    cout << root->key << " ";

    // then recur on left subtree
    preOrder(root->left);

    // now recur on right subtree
    preOrder(root->right);
}

// function to print inorder traversal of the tree
void inOrder(Node *root) {
    if (root == NULL)
        return;

    // first recur on left subtree
    inOrder(root->left);

    // then print data of root
    cout << root->key << " ";

    // now recur on right subtree
    inOrder(root->right);
}

// function to print postorder traversal of the tree
void postOrder(Node *root) {
    if (root == NULL)
        return;

    // first recur on left subtree
    postOrder(root->left);

    // then recur on right subtree
    postOrder(root->right);

    // now print data of root
    cout << root->key << " ";
}

int main() {
    Node* root = nullptr; // initialise an empty AVL tree

    string input;
    getline(cin, input); // read input line

    istringstream iss(input);
    string move;

    while (iss >> move) {
        if (move == "PRE") {
            // check if the tree is empty before traversal
            if (root == nullptr) {
                cout << "EMPTY" << endl;
            } else {
                // finish with pre-order traversal
                preOrder(root);
                cout << endl;
            }
        } else if (move == "POST") {
            // check if the tree is empty before traversal
            if (root == nullptr) {
                cout << "EMPTY" << endl;
            } else {
                // finish with post-order traversal
                postOrder(root);
                cout << endl;
            }
        } else if (move == "IN") {
            // check if the tree is empty before traversal
            if (root == nullptr) {
                cout << "EMPTY" <<endl;
            } else {
                // finish with in-order traversal
                inOrder(root);
                cout << endl;
            }
        } else {
            char modification = move[0];
            int n = stoi(move.substr(1));

            if (modification == 'A') {
                // insert the value into the AVL tree
                root = insert(root, n);
            } else if (modification == 'D') {
                // delete the value from the AVL tree
                root = deleteNode(root, n);
            }
        }
    }

    return 0;
}