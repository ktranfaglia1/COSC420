/* Kyle Tranfaglia & Dustin O'Brien
*  COSC420 - Homework03
*  Last Updated: 11/08/24
*  This is an OpenMP program that, in parallel, creates a 2^20 binary tree using recursion and counts the number of values in range [0,1] that are val < 0.5 *Optimized version*
*/
#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>

// Node object
struct node {
    float val;  // Randomly assigned value between 0 and 1
    struct node* l;  // Pointer to the left child node
    struct node* r;  // Pointer to the right child node
};

// Helper function that handles parallel creation based on level threshold
struct node* createTreeParallel(struct node* currentNode, int levels, int threshold_level) {
    // Base case to stop creating nodes when final level reached
    if (levels == 0) {
        return NULL;
    }

    // Allocate memory for the current node, assign a random value to the node between 0 and 1, and initialize left and right children to NULL
    currentNode = (struct node*)malloc(sizeof(struct node));
    currentNode->val = (float)rand() / RAND_MAX;
    currentNode->l = currentNode->r = NULL;

    // Create subtrees if there are more than one level to go
    if (levels > 1) {
        // Only parallelize at higher levels to avoid overhead at lower levels
        if (threshold_level >= 0) {
            // Parallelize the creation of left and right subtrees using OpenMP sections
            #pragma omp parallel sections
            {
                #pragma omp section
                currentNode->l = createTreeParallel(currentNode->l, levels - 1, threshold_level - 1);
                
                #pragma omp section
                currentNode->r = createTreeParallel(currentNode->r, levels - 1, threshold_level - 1);
            }
        } 
        else {
            // If below threshold, create subtrees serially to reduce overhead
            currentNode->l = createTreeParallel(currentNode->l, levels - 1, -1);
            currentNode->r = createTreeParallel(currentNode->r, levels - 1, -1);
        }
    }
    return currentNode;
}

// Create tree and determine optimal parallelization level
struct node* createTree(struct node* currentNode, int levels) {
    int num_threads = omp_get_max_threads();  // Get the maximum number of threads available in the system
   
    // Calculate a threshold level based on the number of threads available to prevent excessive overhead
    int threshold_level = 0;
    for (int i = num_threads; i > 1; i /= 2) {
        threshold_level++;
    }
    return createTreeParallel(currentNode, levels, threshold_level -8 + (levels / 6));  // Start parallel creation only at higher levels (reduce threshold slightly based on tree depth)
}

// Free all dynamically allocated memory for the tree (free all nodes)
void freeTree(struct node* currentNode) {
    // Base case for when there is no node to free
    if (currentNode == NULL) {
        return;
    }

    // Recursively free the left and right child nodes if they exist and free the current node memory
    freeTree(currentNode->l);
    freeTree(currentNode->r);
    free(currentNode);
}

// Helper function for parallel counting of nodes
int countNodesParallel(struct node* root, int threshold_level) {
    // Base case to stop counting when there are no more nodes
    if (root == NULL) { 
        return 0;
    }

    // Check the current node's value and increment count if it satisfies the condition
    int count = 0;
    if (root->val < 0.5) {
        count = 1;
    }
    
    // Check if at a level high enough to parallelize the counting
    if (threshold_level >= 0) {
        int left_count = 0, right_count = 0;

        // Parallelize counting of left and right subtrees using OpenMP sections
        #pragma omp parallel sections
        {
            #pragma omp section
            left_count = countNodesParallel(root->l, threshold_level - 1);
            
            #pragma omp section
            right_count = countNodesParallel(root->r, threshold_level - 1);
        }
        return count + left_count + right_count;  // Return the total count of nodes, including current node and results from subtrees
    } 
    // If below the threshold for parallelization, count serially
    else {
        return count + countNodesParallel(root->l, -1) + countNodesParallel(root->r, -1);
    }
}

// Count nodes with values < 0.5
int countNodes(struct node* root) {
    int num_threads = omp_get_max_threads();  // Get the maximum number of threads available in the system
    int threshold_level = 0;

    // Calculate a threshold level based on the number of threads available
    for (int i = num_threads; i > 1; i /= 2) {
        threshold_level++;
    }
    return countNodesParallel(root, threshold_level - 1);  // Call the parallel counting function with the calculated threshold level
}

// Driver program
int main() {
    srand(time(NULL));  // Set seed for random number generation

    double startTime = omp_get_wtime();  // Record the start time
    
    struct node* tree = createTree(NULL, 20);  // Create a binary tree with a depth of 20 using the parallelized tree creation function
    int count = countNodes(tree);  // Count the nodes with values less than 0.5 in the created tree
    freeTree(tree);  // Free the memory allocated for the tree after counting
    
    double endTime = omp_get_wtime();  // Record the end time using OpenMP's timing function
    
    printf("Node Count: %d\nTotal Time: %f\n", count, endTime - startTime);  // Display the result
    
    return 0;
}