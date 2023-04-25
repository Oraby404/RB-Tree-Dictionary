#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define  RED    1
#define  BLACK  0


struct Node {
    char *key;
    struct Node *left;
    struct Node *right;
    struct Node *parent;
    bool color;
};

typedef struct Node Node;
typedef Node *Tree;

Tree constructNode(char *str) {
    Tree n = malloc(sizeof(Node));
    n->key = (char *) malloc(strlen(str) + 1);
    strcpy(n->key, str);
    n->left = NULL;
    n->right = NULL;
    n->parent = NULL;
    n->color = RED;
    return n;
}

Tree bst_insert(Tree root, Tree new_node) {
    if (root == NULL)
        return new_node;

    if (strcasecmp(new_node->key, root->key) < 0) {
        root->left = bst_insert(root->left, new_node);
        root->left->parent = root;
    } else if (strcasecmp(new_node->key, root->key) > 0) {
        root->right = bst_insert(root->right, new_node);
        root->right->parent = root;
    }
    return root;
}

Tree left_rotate(Tree root, Tree node) {

    Tree new_root = NULL;

    Tree y = node->right;
    node->right = y->left;

    if (y->left != NULL)
        y->left->parent = node;

    y->parent = node->parent;

    if (node->parent == NULL)
        new_root = y;
    else if (node == node->parent->left) {
        node->parent->left = y;
        new_root = root;
    } else {
        node->parent->right = y;
        new_root = root;
    }

    y->left = node;
    node->parent = y;

    return new_root;
}

Tree right_rotate(Tree root, Tree node) {
    Tree new_root = NULL;

    Tree y = node->left;
    node->left = y->right;

    if (y->right != NULL)
        y->right->parent = node;

    y->parent = node->parent;

    if (node->parent == NULL)
        new_root = y;
    else if (node == node->parent->right) {
        node->parent->right = y;
        new_root = root;
    } else {
        node->parent->left = y;
        new_root = root;
    }

    y->right = node;
    node->parent = y;

    return new_root;
}

Tree RB_Insert_Fixup(Tree root, Tree z) {
    Tree z_parent = NULL;
    Tree z_uncle = NULL;
    Tree z_grandparent = NULL;

    while ((z != root) && (z->color != BLACK) && (z->parent->color == RED)) {

        z_parent = z->parent;
        z_grandparent = z->parent->parent;

        if (z_parent == z_grandparent->left) {

            z_uncle = z_grandparent->right;

            if ((z_uncle != NULL) && (z_uncle->color == RED)) {
                z_parent->color = BLACK;
                z_uncle->color = BLACK;
                z_grandparent->color = RED;
                z = z_grandparent;
            } else {

                if (z == z_parent->right) {
                    root = left_rotate(root, z_parent);
                    z = z_parent;
                    z_parent = z->parent;
                }
                root = right_rotate(root, z_grandparent);
                bool color = z_parent->color;
                z_parent->color = z_grandparent->color;
                z_grandparent->color = color;
                z = z_parent;

            }
        } else {
            z_uncle = z_grandparent->left;

            if ((z_uncle != NULL) && (z_uncle->color == RED)) {
                z_parent->color = BLACK;
                z_uncle->color = BLACK;
                z_grandparent->color = RED;
                z = z_grandparent;
            } else {

                if (z == z_parent->left) {
                    root = right_rotate(root, z_parent);
                    z = z_parent;
                    z_parent = z->parent;
                }
                root = left_rotate(root, z_grandparent);
                bool color = z_parent->color;
                z_parent->color = z_grandparent->color;
                z_grandparent->color = color;
                z = z_parent;

            }
        }
    }
    root->color = BLACK;
    return root;
}


int count(Tree root) {
    if (root) return 1 + count(root->left) + count(root->right);
    return 0;
}


int max(int x, int y) {
    return x > y ? x : y;
}

int height(Tree root) {
    if (root == NULL)
        return -1;
    else
        return 1 + max(height(root->left), height(root->right));

}

Tree RB_Insert(Tree root, char *str) {
    Tree new_node = constructNode(str);
    Tree new_root = bst_insert(root, new_node);
    new_root = RB_Insert_Fixup(new_root, new_node);
    return new_root;
}

Tree search(Tree root, char *str) {
    if (root == NULL)
        return NULL;

    else if (strcasecmp(str, root->key) == 0)
        return root;

    else if (strcasecmp(str, root->key) < 0)
        return search(root->left, str);

    else
        return search(root->right, str);
}


void check(Tree root, char *str) {
    Node *temp = search(root, str);
    if (temp)
        printf("\n%s - Found\n", temp->key);
    else {
        printf("\n%s - Not Found\n\nDo You Wish to Insert \"%s\" in the Dictionary? (y/n) :", str, str);
        char answer = 'y';

        scanf("%c", &answer);
        if (answer == 'y') {
            root = RB_Insert(root, str);
            printf("\n\"%s\" Inserted Successfully\n",str);
            printf("..................................\n");
            printf("New Size : %d\n", count(root));
            printf("..................................\n");
            printf("New Height : %d\n", height(root));
            printf("..................................\n");
        }
    }
}

int main() {

    char buff[30], str[100], *token;
    FILE *fp = fopen("EN-US-Dictionary.txt", "a+");

    Tree root = NULL;

    while (fgets(buff, sizeof(buff), fp)) {
        buff[strlen(buff) - 1] = '\0';
        root = RB_Insert(root, buff);
    }

    printf("Dictionary Loaded Successfully...!\n");
    printf("..................................\n");
    printf("Size : %d\n", count(root));
    printf("..................................\n");
    printf("Height : %d\n", height(root));
    printf("..................................\n");

    printf("\nEnter a Sentence : ");
    while (fgets(str, sizeof(str), stdin) != NULL) {
        str[strlen(str) - 1] = '\0';
        token = strtok(str, " ");
        while (token != NULL) {
            check(root, token);
            token = strtok(NULL, " ");
        }
        fflush(stdin);
        printf("\nEnter a Sentence : ");
    }

    return 0;
}
