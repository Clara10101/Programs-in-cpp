#include <iostream>
#include <cstddef>

using namespace std;

/*
 * BST Trees
 */

struct BST_node {
    int key;
    BST_node *left, *right;
};

BST_node * BST_search(BST_node *node, int key) {
    //search node with given key
    if (node == NULL)
        return NULL;
    if (node->key == key)
        return node;
    if (node->key > key)
        return BST_search(node->left, key);
    else
        return BST_search(node->right, key);
}

bool key_in_tree(BST_node *root, int key) {
    return BST_search(root, key) != NULL;
}

BST_node * BST_insert(BST_node *node, int key) {
    if (!key_in_tree(node, key)) {
        if (node == NULL) {
            BST_node *tmp = new BST_node;
            tmp->key = key;
            tmp->left = tmp->right = NULL;
            return tmp;
        }

        if (key < node->key)
            node->left = BST_insert(node->left, key);
        else
            node->right = BST_insert(node->right, key);
        return node;
    }
    else
        return NULL;
}

void BST_print(BST_node * node) {
    if (node != NULL) {
        if (node->left != NULL)
            BST_print(node->left);
        cout << node->key << endl;
        if (node->right != NULL)
            BST_print(node->right);
    }
}

BST_node * BST_find_parent(BST_node * node, BST_node * root) {
    //find node's parent
    BST_node * parent = NULL;
    if (root->left == node || root->right == node) {
        parent = root;
    } else {
        if (root->left != NULL)
            parent = BST_find_parent(node, root->left);
        if (parent == NULL){
            if (root->right != NULL)
                parent = BST_find_parent(node, root->right);
        }
    }
    return parent;

}

BST_node * BST_find_max(BST_node * node) {
    if (node->right != NULL)
        return BST_find_max(node->right);
    else
        return node;
}

void BST_delete(BST_node * node, int key) {
    //delete node with given key, node - root of the tree
    BST_node * node_to_remove = BST_search(node, key);
    BST_node * parent = BST_find_parent(node_to_remove, node);

    if (node_to_remove != NULL) {

        //if node doesn't have children - delete
        if (node_to_remove->left == NULL && node_to_remove->right == NULL) {
            //set null in parent
            if (parent->left == node_to_remove)
                parent->left = NULL;
            else
                parent->right = NULL;
        }

        //node to remove has one child 
        if (node_to_remove->left == NULL && node_to_remove->right != NULL) {
            if (parent->left == node_to_remove)
                parent->left = node_to_remove->right;
            else
                parent->right = node_to_remove->right;
        }
        
        if (node_to_remove->left != NULL && node_to_remove->right == NULL) {
            if (parent->left == node_to_remove){
                parent->left = node_to_remove->left;
            }
            else{
                parent->right = node_to_remove->left;
            }
        }

        //replace with biggest node in left subtree
        if (node_to_remove->left != NULL && node_to_remove->right != NULL) {
            BST_node * node_to_change = BST_find_max(node_to_remove->left);
            int tmp = node_to_change->key;
            BST_delete(node, tmp);
            node_to_remove->key = tmp;
        }
    }
}

int main(int argc, char** argv) {
    BST_node *root = NULL;
    root = BST_insert(root, 10);
    root = BST_insert(root, 8);
    root = BST_insert(root, 9);
    root = BST_insert(root, 5);
    root = BST_insert(root, 15);
    root = BST_insert(root, 13);
    root = BST_insert(root, 1);
    root = BST_insert(root, 2);
    root = BST_insert(root, 11);
    root = BST_insert(root, 14);
    root = BST_insert(root, 12);
    root = BST_insert(root, 20);
    root = BST_insert(root, 7);
    root = BST_insert(root, 100);
    root = BST_insert(root, 87);
    BST_print(root);
    BST_delete(root, 100);
    BST_print(root);
    return 0;
}

