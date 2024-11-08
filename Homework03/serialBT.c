/* Kyle Tranfaglia & Dustin O'Brien
*  COSC420 - Homework03
*  Last Updated: 11/06/24
*  This is a program that serially creates a 2^20 binary tree using recursion and counts the number of values in range [0,1] that are x < .5
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Node object
struct node {
    float val;  // Randomly assigned value between 0 and 1
    struct node* l;  // Pointer to the left child node
    struct node* r;  // Pointer to the right child node
};

// Create a binary tree recursively given a root node
struct node* createTree(struct node* currentNode, int levels) {

    // Allocate memory for the current node, assign a random value, and initialize the pointers
    currentNode = (struct node*)malloc(sizeof(struct node));
    currentNode->val = (double)rand() / RAND_MAX;
    currentNode->l = currentNode->r = NULL;

    // Recursively create child nodes until no more levels remain
    if (levels != 1) {
        // Create and link nodes in tree
        currentNode->l = createTree(currentNode->l, levels - 1);
        currentNode->r = createTree(currentNode->r, levels - 1);
    }
    return currentNode;
}

// Free all dynamically allocated memory for the tree (free all nodes)
struct node* freeTree(struct node* currentNode) {
    // Recursively free the left and right child nodes if they exist
    if (currentNode->l != NULL) {
        freeTree(currentNode->l);
    }
    if (currentNode->r != NULL) {
        freeTree(currentNode->r);
    }
    free(currentNode);  // Free the current node and set it to NULL

    return NULL;
}

// Count all nodes in the tree such that val < 0.5 
int countNodes(struct node* root) {

    // Base case to terminate recursion
    if (root == NULL) {
        return 0;
    }

    int count = 0;

    // Increment the count if the node's value is less than 0.5
    if (root->val < .5) {
        count += 1;
    }

    // Recurse for left and right children and keep a running total or returned values (count increments)
    count += countNodes(root->l);
    count += countNodes(root->r);

    return count;
}

// Driver program
int main() {

    srand(time(NULL));  // Set seed for random number generation

    double startTime = (double)clock() / CLOCKS_PER_SEC;  // Record the start time

    struct node* tree = createTree(tree, 20);  // Create a binary tree with a depth of 20
    int count = countNodes(tree);  // Count nodes with values less than 0.5
    freeTree(tree);  // Free all memory allocated for the tree

    double endTime = (double)clock() / CLOCKS_PER_SEC;  // Record the end time
    
    printf("Node Count: %d \nTotal Time: %f\n", count, endTime - startTime);  // Display the results
    
    return 0;
}