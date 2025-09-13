#ifndef NPUZZLE_H
#define NPUZZLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#define MAX_SIZE 4
#define MAX_NODES 1000

// Heuristic function types
typedef enum {
    TILES_OUT_OF_PLACE,
    MANHATTAN_DISTANCE
} HeuristicType;

// Tree node structure for N-Puzzle states
struct TreeNode {
    int puzzle[MAX_SIZE][MAX_SIZE];  // Puzzle state
    int empty_row, empty_col;        // Position of empty tile (0)
    int EXP;                         // Expansion flag (1 if expanded, 0 otherwise)
    int GST;                         // Goal state flag (1 if goal, 0 otherwise)
    int PST;                         // Path state flag (1 if on solution path, 0 otherwise)
    int g;                           // Cost from start to current node
    int h;                           // Heuristic cost to goal
    int f;                           // Total cost (g + h)
    int depth;                       // Depth in search tree
    struct TreeNode* parent;         // Parent node
    struct TreeNode* children[4];    // Children nodes (up, down, left, right)
    int num_children;                // Number of children
};

// Function declarations
struct TreeNode* create_node(int puzzle[MAX_SIZE][MAX_SIZE], int n);
void copy_puzzle(int src[MAX_SIZE][MAX_SIZE], int dest[MAX_SIZE][MAX_SIZE], int n);
bool is_equal_state(int state1[MAX_SIZE][MAX_SIZE], int state2[MAX_SIZE][MAX_SIZE], int n);
void find_empty_position(int puzzle[MAX_SIZE][MAX_SIZE], int n, int* row, int* col);
bool is_valid_move(int row, int col, int n);
struct TreeNode* make_move(struct TreeNode* node, int new_row, int new_col, int n);
int calculate_heuristic(int puzzle[MAX_SIZE][MAX_SIZE], int goal[MAX_SIZE][MAX_SIZE],
                        int n, HeuristicType type);
int tiles_out_of_place(int puzzle[MAX_SIZE][MAX_SIZE], int goal[MAX_SIZE][MAX_SIZE], int n);
int manhattan_distance(int puzzle[MAX_SIZE][MAX_SIZE], int goal[MAX_SIZE][MAX_SIZE], int n);
bool is_goal_state(int puzzle[MAX_SIZE][MAX_SIZE], int goal[MAX_SIZE][MAX_SIZE], int n);
struct TreeNode* a_star_search(int initial[MAX_SIZE][MAX_SIZE], int goal[MAX_SIZE][MAX_SIZE],
                               int n, HeuristicType heuristic_type);
void generate_children(struct TreeNode* node, int goal[MAX_SIZE][MAX_SIZE],
                       int n, HeuristicType heuristic_type);
struct TreeNode* find_min_f_node(struct TreeNode** open_list, int* open_count);
bool is_in_closed_list(struct TreeNode** closed_list, int closed_count,
                       int puzzle[MAX_SIZE][MAX_SIZE], int n);
void print_puzzle(int puzzle[MAX_SIZE][MAX_SIZE], int n);
void print_solution_path(struct TreeNode* goal_node);
void print_search_tree(struct TreeNode* root, int level);
void print_search_statistics(struct TreeNode* root, struct TreeNode* goal_node);
void display_final_search_tree(struct TreeNode* root);
void free_tree(struct TreeNode* node);
int count_nodes_in_tree(struct TreeNode* root);
struct TreeNode* find_goal_node(struct TreeNode* root);

#endif