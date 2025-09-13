#include "npuzzle.h"

/**
 * Prints the solution path from initial state to goal state
 */
void print_solution_path(struct TreeNode* goal_node) {
    if (!goal_node) {
        printf("No solution path available.\n");
        return;
    }
    
    // Create array to store path nodes
    struct TreeNode* path[1000];
    int path_length = 0;
    
    // Traverse from goal to root to build path
    struct TreeNode* current = goal_node;
    while (current) {
        path[path_length] = current;
        path_length++;
        current = current->parent;
    }
    
    printf("\n=== SOLUTION PATH ===\n");
    printf("Number of moves: %d\n\n", path_length - 1);
    
    // Print path from root to goal
    for (int i = path_length - 1; i >= 0; i--) {
        if (i == path_length - 1) {
            printf("Initial State (Step %d):\n", path_length - 1 - i);
        } else if (i == 0) {
            printf("Goal State (Step %d):\n", path_length - 1 - i);
        } else {
            printf("Step %d:\n", path_length - 1 - i);
        }
        
        print_puzzle(path[i]->puzzle, 3); // Assuming 3x3 for display
        printf("g=%d, h=%d, f=%d\n", path[i]->g, path[i]->h, path[i]->f);
        
        if (i > 0) {
            printf("   ↓\n");
        }
        printf("\n");
    }
}

/**
 * Prints the search tree structure (simplified version)
 */
void print_search_tree(struct TreeNode* root, int level) {
    if (!root) return;
    
    // Print indentation
    for (int i = 0; i < level; i++) {
        printf("  ");
    }
    
    // Print node information
    printf("Node[g=%d,h=%d,f=%d", root->g, root->h, root->f);
    if (root->PST == 1) printf(",PATH");
    if (root->GST == 1) printf(",GOAL");
    if (root->EXP == 1) printf(",EXPANDED");
    printf("]\n");
    
    // Print children
    for (int i = 0; i < root->num_children; i++) {
        print_search_tree(root->children[i], level + 1);
    }
}

/**
 * Prints detailed statistics about the search
 */
void print_search_statistics(struct TreeNode* root, struct TreeNode* goal_node) {
    if (!root) {
        printf("No search tree available.\n");
        return;
    }
    
    printf("\n=== SEARCH STATISTICS ===\n");
    
    // Count total nodes in tree
    int total_nodes = count_nodes_in_tree(root);
    printf("Total nodes in search tree: %d\n", total_nodes);
    
    if (goal_node) {
        printf("Solution found: YES\n");
        printf("Solution depth: %d moves\n", goal_node->depth);
        printf("Solution cost: %d\n", goal_node->g);
        
        // Count nodes on solution path
        int path_nodes = 0;
        struct TreeNode* current = goal_node;
        while (current) {
            path_nodes++;
            current = current->parent;
        }
        printf("Nodes on solution path: %d\n", path_nodes);
        
        // Calculate branching factor (approximate)
        if (goal_node->depth > 0) {
            double branching_factor = (double)(total_nodes - 1) / goal_node->depth;
            printf("Average branching factor: %.2f\n", branching_factor);
        }
    } else {
        printf("Solution found: NO\n");
    }
    
    printf("\n");
}

/**
 * Displays the final search tree with detailed node information
 */
void display_final_search_tree(struct TreeNode* root) {
    if (!root) {
        printf("No search tree to display.\n");
        return;
    }
    
    printf("\n=== FINAL SEARCH TREE STRUCTURE ===\n");
    printf("Legend: PATH=on solution path, GOAL=goal state, EXPANDED=node was expanded\n\n");
    
    print_search_tree(root, 0);
    printf("\n");
}

/**
 * Prints a compact representation of the puzzle state (for tree display)
 */
void print_compact_puzzle(int puzzle[MAX_SIZE][MAX_SIZE], int n) {
    printf("[");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (puzzle[i][j] == 0) {
                printf(" ");
            } else {
                printf("%d", puzzle[i][j]);
            }
            if (i * n + j < n * n - 1) printf(",");
        }
    }
    printf("]");
}
