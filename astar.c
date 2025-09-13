#include "npuzzle.h"

/**
 * Generates all possible children states from the current node
 */
void generate_children(struct TreeNode* node, int goal[MAX_SIZE][MAX_SIZE], 
                      int n, HeuristicType heuristic_type) {
    // Possible moves: up, down, left, right
    int moves[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    
    node->num_children = 0;
    
    for (int i = 0; i < 4; i++) {
        int new_row = node->empty_row + moves[i][0];
        int new_col = node->empty_col + moves[i][1];
        
        // Check if the move is valid
        if (is_valid_move(new_row, new_col, n)) {
            struct TreeNode* child = make_move(node, new_row, new_col, n);
            if (child) {
                // Calculate heuristic and f value
                child->h = calculate_heuristic(child->puzzle, goal, n, heuristic_type);
                child->f = child->g + child->h;
                
                // Check if this child is the goal state
                if (is_goal_state(child->puzzle, goal, n)) {
                    child->GST = 1;
                }
                
                // Add child to parent's children array
                node->children[node->num_children] = child;
                node->num_children++;
            }
        }
    }
    
    // Mark this node as expanded
    node->EXP = 1;
}

/**
 * Finds the node with minimum f value from the open list
 */
struct TreeNode* find_min_f_node(struct TreeNode** open_list, int* open_count) {
    if (*open_count == 0) return NULL;
    
    int min_index = 0;
    int min_f = open_list[0]->f;
    
    // Find node with minimum f value
    for (int i = 1; i < *open_count; i++) {
        if (open_list[i]->f < min_f) {
            min_f = open_list[i]->f;
            min_index = i;
        }
    }
    
    struct TreeNode* selected_node = open_list[min_index];
    
    // Remove selected node from open list
    for (int i = min_index; i < *open_count - 1; i++) {
        open_list[i] = open_list[i + 1];
    }
    (*open_count)--;
    
    return selected_node;
}

/**
 * Checks if a state is already in the closed list
 */
bool is_in_closed_list(struct TreeNode** closed_list, int closed_count, 
                      int puzzle[MAX_SIZE][MAX_SIZE], int n) {
    for (int i = 0; i < closed_count; i++) {
        if (is_equal_state(closed_list[i]->puzzle, puzzle, n)) {
            return true;
        }
    }
    return false;
}

/**
 * A* Search Algorithm implementation
 */
struct TreeNode* a_star_search(int initial[MAX_SIZE][MAX_SIZE], int goal[MAX_SIZE][MAX_SIZE], 
                              int n, HeuristicType heuristic_type) {
    printf("\n=== Starting A* Search ===\n");
    printf("Heuristic: %s\n", 
           (heuristic_type == TILES_OUT_OF_PLACE) ? "Tiles Out of Place" : "Manhattan Distance");
    printf("Puzzle size: %dx%d\n\n", n, n);
    
    // Check if initial state is already the goal state
    if (is_goal_state(initial, goal, n)) {
        printf("Initial state is already the goal state!\n");
        struct TreeNode* root = create_node(initial, n);
        root->GST = 1;
        root->PST = 1;
        return root;
    }
    
    // Initialize open and closed lists
    struct TreeNode* open_list[MAX_NODES];
    struct TreeNode* closed_list[MAX_NODES];
    int open_count = 0;
    int closed_count = 0;
    
    // Create root node and add to open list
    struct TreeNode* root = create_node(initial, n);
    root->h = calculate_heuristic(initial, goal, n, heuristic_type);
    root->f = root->g + root->h;
    
    open_list[open_count] = root;
    open_count++;
    
    int iteration = 0;
    struct TreeNode* goal_node = NULL;
    
    printf("Initial State:\n");
    print_puzzle(initial, n);
    printf("g=%d, h=%d, f=%d\n\n", root->g, root->h, root->f);
    
    printf("Goal State:\n");
    print_puzzle(goal, n);
    printf("\n");
    
    // Main A* loop
    while (open_count > 0) {
        iteration++;
        printf("--- Iteration %d ---\n", iteration);
        
        // Select node with minimum f value from open list
        struct TreeNode* current = find_min_f_node(open_list, &open_count);
        if (!current) break;
        
        printf("Expanding node with f=%d:\n", current->f);
        print_puzzle(current->puzzle, n);
        printf("g=%d, h=%d, f=%d\n", current->g, current->h, current->f);
        
        // Add current node to closed list
        closed_list[closed_count] = current;
        closed_count++;
        
        // Generate children
        generate_children(current, goal, n, heuristic_type);
        
        printf("Generated %d children:\n", current->num_children);
        
        // Process each child
        for (int i = 0; i < current->num_children; i++) {
            struct TreeNode* child = current->children[i];
            
            // Skip if child is in closed list
            if (is_in_closed_list(closed_list, closed_count, child->puzzle, n)) {
                printf("Child %d: Already explored (skipped)\n", i + 1);
                continue;
            }
            
            printf("Child %d: g=%d, h=%d, f=%d", i + 1, child->g, child->h, child->f);
            
            // Check if this child is the goal
            if (child->GST == 1) {
                printf(" [GOAL FOUND!]\n");
                goal_node = child;
                break;
            } else {
                printf("\n");
            }
            
            // Add child to open list
            if (open_count < MAX_NODES) {
                open_list[open_count] = child;
                open_count++;
            }
        }
        
        if (goal_node) break;
        
        printf("Open list size: %d\n", open_count);
        printf("Closed list size: %d\n\n", closed_count);
        
        // Safety check to prevent infinite loops
        if (iteration > 1000) {
            printf("Maximum iterations reached. Search terminated.\n");
            break;
        }
    }
    
    if (goal_node) {
        printf("=== GOAL REACHED! ===\n");
        printf("Total iterations: %d\n", iteration);
        printf("Total nodes explored: %d\n", closed_count);
        printf("Solution depth: %d moves\n", goal_node->depth);
        
        // Mark solution path
        struct TreeNode* path_node = goal_node;
        while (path_node) {
            path_node->PST = 1;
            path_node = path_node->parent;
        }
        
    } else {
        printf("=== NO SOLUTION FOUND ===\n");
        printf("Search space exhausted or maximum iterations reached.\n");
    }
    
    return (goal_node) ? root : NULL;
}
