// Heap size 2048 required!

// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>

// User libraries
#include "BinaryTree.h"

// **** Set any macros or preprocessor directives here ****


// **** Define any module-level, global, or external variables here ****


// **** Declare any function prototypes here ****

int main() {
    BOARD_Init();

    printf("\nWelcome to slin41's Binary Tree test harness, compiled on %s %s\n", __DATE__, __TIME__);

    //this is a simple test binary tree, included so you can write the relatively simple TreePrint() before implementing CreateTree().  In general, you should NOT create your binary trees this way.
    //level 1 (leaf) nodes

    /*printing given tree*/

    Node nodeD = {NULL, NULL, 'D'};
    Node nodeE = {NULL, NULL, 'E'};
    Node nodeF = {NULL, NULL, 'F'};
    Node nodeG = {NULL, NULL, 'G'};
    //level 2 nodes:
    Node nodeB = {&nodeD, &nodeE, 'B'};
    Node nodeC = {&nodeF, &nodeG, 'C'};
    //level 3 node:
    Node nodeA = {&nodeB, &nodeC, 'A'};
    Node * root = &nodeA;

    TreePrint(root, 0);

    /*creating own tree string*/
    char treeData[7] = "abdecfg\0";

    /*creating own tree based on treeData */
    Node *root = TreeCreate(3, treeData);

    /*printing root tree*/
    TreePrint(root, 0);

    while (1);
}

