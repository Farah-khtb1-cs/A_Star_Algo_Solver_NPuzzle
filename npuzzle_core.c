#include "npuzzle.h"

/**
 * Creates a new tree node with the given puzzle state
 */
struct TreeNode* create_node(int puzzle[MAX_SIZE][MAX_SIZE], int n) {
    struct TreeNode* node = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    if (!node) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    
    // Initialize the node
    copy_puzzle(puzzle, node->puzzle, n);
    find_empty_position(puzzle, n, &node->empty_row, &node->empty_col);
    node->EXP = 0;
    node->GST = 0;
    node->PST = 0;
    node->g = 0;
    node->h = 0;
    node->f = 0;
    node->depth = 0;
    node->parent = NULL;
    node->num_children = 0;
    
    // Initialize children pointers
    for (int i = 0; i < 4; i++) {
        node->children[i] = NULL;
    }
    
    return node;
}

/**
 * Copies puzzle from source to destination
 */
void copy_puzzle(int src[MAX_SIZE][MAX_SIZE], int dest[MAX_SIZE][MAX_SIZE], int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            dest[i][j] = src[i][j];
        }
    }
}

/**
 * Checks if two puzzle states are equal
 */
bool is_equal_state(int state1[MAX_SIZE][MAX_SIZE], int state2[MAX_SIZE][MAX_SIZE], int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (state1[i][j] != state2[i][j]) {
                return false;
            }
        }
    }
    return true;
}

/**
 * Finds the position of the empty tile (0)
 */
void find_empty_position(int puzzle[MAX_SIZE][MAX_SIZE], int n, int* row, int* col) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (puzzle[i][j] == 0) {
                *row = i;
                *col = j;
                return;
            }
        }
    }
}

/**
 * Checks if a move to the given position is valid
 */
bool is_valid_move(int row, int col, int n) {
    return (row >= 0 && row < n && col >= 0 && col < n);
}

/**
 * Creates a new state by moving the empty tile to the specified position
 */
struct TreeNode* make_move(struct TreeNode* node, int new_row, int new_col, int n) {
    if (!is_valid_move(new_row, new_col, n)) {
        return NULL;
    }
    
    // Create new puzzle state
    int new_puzzle[MAX_SIZE][MAX_SIZE];
    copy_puzzle(node->puzzle, new_puzzle, n);
    
    // Swap empty tile with the tile at new position
    new_puzzle[node->empty_row][node->empty_col] = new_puzzle[new_row][new_col];
    new_puzzle[new_row][new_col] = 0;
    
    // Create new node
    struct TreeNode* new_node = create_node(new_puzzle, n);
    new_node->parent = node;
    new_node->depth = node->depth + 1;
    new_node->g = node->g + 1;  // Cost increases by 1 for each move
    
    return new_node;
}

/**
 * Checks if the current state is the goal state
 */
bool is_goal_state(int puzzle[MAX_SIZE][MAX_SIZE], int goal[MAX_SIZE][MAX_SIZE], int n) {
    return is_equal_state(puzzle, goal, n);
}

/**
 * Prints a puzzle state
 */
void print_puzzle(int puzzle[MAX_SIZE][MAX_SIZE], int n) {
    printf("+");
    for (int j = 0; j < n; j++) {
        printf("---+");
    }
    printf("\n");
    
    for (int i = 0; i < n; i++) {
        printf("|");
        for (int j = 0; j < n; j++) {
            if (puzzle[i][j] == 0) {
                printf("   |");
            } else {
                printf(" %d |", puzzle[i][j]);
            }
        }
        printf("\n+");
        for (int j = 0; j < n; j++) {
            printf("---+");
        }
        printf("\n");
    }
}

/**
 * Counts the total number of nodes in the search tree
 */
int count_nodes_in_tree(struct TreeNode* root) {
    if (!root) return 0;
    
    int count = 1;  // Count current node
    for (int i = 0; i < root->num_children; i++) {
        count += count_nodes_in_tree(root->children[i]);
    }
    return count;
}

/**
 * Finds the goal node in the tree
 */
struct TreeNode* find_goal_node(struct TreeNode* root) {
    if (!root) return NULL;
    
    if (root->GST == 1) return root;
    
    for (int i = 0; i < root->num_children; i++) {
        struct TreeNode* goal = find_goal_node(root->children[i]);
        if (goal) return goal;
    }
    
    return NULL;
}

/**
 * Frees memory allocated for the tree
 */
void free_tree(struct TreeNode* node) {
    if (!node) return;
    
    for (int i = 0; i < node->num_children; i++) {
        free_tree(node->children[i]);
    }
    free(node);
}
