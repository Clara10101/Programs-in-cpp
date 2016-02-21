#include <iostream>

using namespace std;

/*
 * BST typy zagniezdzone
 * Oddzielenie interfejsu od implementacji
 */
struct BST {
private:

    struct BST_node {
        //jako typ skladowy struktury BST
        int key;
        BST_node *left, *right;
    };

    BST_node *root;

    BST_node * BST_search(BST_node *, int);
    BST_node * BST_insert(BST_node *, int);
    void BST_print(BST_node *);
    void BST_destroy(BST_node *);

public:
    bool search(int);
    void insert(int);
    void init();
    void destroy();
    void print();
};

bool BST::search(int key) {
    return BST_search(root, key) != NULL;
}

void BST::insert(int key) {
    root = BST_insert(root, key);
}

void BST::print(){
    BST_print(root);
}

void BST::init(){
    //ustawienie root na NULL
    root = NULL;
}

void BST::destroy(){
    // ma usunąć całe drzewo (zwalniając pamięć po każdym zaalokowanym węźle – żadnych wycieków!), 
    //a następnie ustawia root na NULL (np. przez wywołanie init)
    //usuwa lewe i prawe poddrzewo a nastepnie ustawia root na null
    if (root->left != NULL)
        BST_destroy(root->left);
    if (root->right != NULL)
        BST_destroy(root->right);
    init();
}

void BST::BST_destroy(BST_node * node){
    if (node != NULL){
        if (node->left != NULL)
            BST_destroy(node->left);
        if (node->right != NULL)
            BST_destroy(node->right);
        delete node;
    }
}

void BST::BST_print(BST_node * node) {
    if (node != NULL) {
        if (node->left != NULL)
            BST_print(node->left);
        cout << node->key << endl;
        if (node->right != NULL)
            BST_print(node->right);
    }
}

BST::BST_node * BST::BST_search(BST_node *node, int key) {
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

BST::BST_node * BST::BST_insert(BST_node *node, int key) {

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


/*
 * BST tree;
 * W tak określonej strukturze BST możemy jedynie wołać z zewnątrz metody insert i search. Pozostałe metody
 * oraz zmienna root są dostępne jedynie z innych metod struktury. Zauważmy, że powoduje to pewien problem przy
 * inicjowaniu drzewa – wcześniej mogliśmy dobrać się do pola root i ustawić je na NULL. Teraz nie jest to możliwe,
 * więc deklarujemy zmienną typu BST globalnie, bo wtedy wszystkie jej pola są wyzerowane. Dlatego musiala powstac 
 * metoda ktorej zadaniem jest ustawienie root na null
 */

int main(int argc, char** argv) {
    BST tree;
    tree.init();
    tree.insert(10);
    tree.insert(8);
    tree.print();
    tree.destroy();
    tree.print();
    return 0;
}

