/*
 * File:   BinaryTree.c
 * Author: Stephanie Lin
 *
 * Created on May 24, 2019, 2:30 PM
 */

#include <stdio.h>
#include "xc.h"
#include "BinaryTree.h"

Node *TreeCreate(int level, const char *data) {
    /*allocate memory with malloc()*/
    Node *head = malloc(sizeof (Node));
    /*check whether it has failed*/
    if (head == NULL) {
        return NULL;
    }
    if (level == 0) {
        return NULL;
    }
        /*base case*/
    else if (level == 1) {
        head->data = *data;
        head->leftChild = NULL;
        head->rightChild = NULL;
        return head;
    } else {
        /*recursive case*/
        head->data = *data;
        head->leftChild = TreeCreate(level - 1, data + 1);
        head->rightChild = TreeCreate(level - 1, data + (1 << level - 1));
        /*pointer to root node*/
        return head;
    }
}

/*check memory allocation for children, if any failed, return NULL*/

Node *GetLeftChild(Node *root) {
    /*check if child is NULL; if so, end of tree*/
    if (root->leftChild != NULL) {
        return root->leftChild;
    } else {
        return NULL;
    }
}

Node *GetRightChild(Node *root) {
    /*check if child is NULL; if so, end of tree*/
    if (root->rightChild != NULL) {
        return root->rightChild;
    } else {
        return NULL;
    }
}

void TreePrint(Node *node, int space) {
    int n_space;
    int i;
    n_space = space;
    if (node == NULL) {
    } 
    else {
        n_space += LEVEL_SPACES;
        /*call rightChild to print*/
        TreePrint(node->rightChild, n_space);
        printf("\n");
        /*add four spaces in between*/
        for (i = n_space; i > 0; i--) {
            printf(" ");
        }
        /*print current node*/
        printf("%c", node->data);
        /*call leftChild to print*/
        TreePrint(node->leftChild, n_space);
    }
}