#include <stdio.h>
#include <stdlib.h>
#include "npuzzle.h"

/**
 * Reads puzzle configuration from user input
 */
void read_puzzle(int puzzle[MAX_SIZE][MAX_SIZE], int n, const char* puzzle_name) {
    printf("Enter the %s state (%dx%d puzzle):\n", puzzle_name, n, n);
    printf("Use 0 for the empty space. Enter row by row:\n");
    
    for (int i = 0; i < n; i++) {
        printf("Row %d: ", i + 1);
        for (int j = 0; j < n; j++) {
            scanf("%d", &puzzle[i][j]);
        }
    }
}

/**
 * Validates if a puzzle configuration is valid
 */
bool is_valid_puzzle(int puzzle[MAX_SIZE][MAX_SIZE], int n) {
    bool found[MAX_SIZE * MAX_SIZE] = {false};
    
    // Check if all numbers from 0 to n*n-1 are present exactly once
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int value = puzzle[i][j];
            if (value < 0 || value >= n * n || found[value]) {
                return false;
            }
            found[value] = true;
        }
    }
    
    return true;
}

/**
 * Sets up a default 3x3 puzzle configuration for testing
 */
void setup_default_puzzle(int initial[MAX_SIZE][MAX_SIZE], int goal[MAX_SIZE][MAX_SIZE], int n) {
    // Default initial state (already solved for quick test)
    int default_initial[3][3] = {
        {1, 2, 3},
        {8, 0, 4},
        {7, 6, 5}
    };
    
    // Default goal state
    int default_goal[3][3] = {
        {1, 2, 3},
        {8, 0, 4},
        {7, 6, 5}
    };
    
    // Copy to the arrays
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            initial[i][j] = default_initial[i][j];
            goal[i][j] = default_goal[i][j];
        }
    }
}

/**
 * Sets up a more challenging test puzzle
 */
void setup_test_puzzle(int initial[MAX_SIZE][MAX_SIZE], int goal[MAX_SIZE][MAX_SIZE], int n) {
    // Test initial state
    int test_initial[3][3] = {
        {2, 8, 3},
        {1, 6, 4},
        {7, 0, 5}
    };
    
    // Goal state (standard solved puzzle)
    int test_goal[3][3] = {
        {1, 2, 3},
        {8, 0, 4},
        {7, 6, 5}
    };
    
    // Copy to the arrays
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            initial[i][j] = test_initial[i][j];
            goal[i][j] = test_goal[i][j];
        }
    }
}

/**
 * Displays menu options
 */
void display_menu() {
    printf("\n=== N-PUZZLE SOLVER MENU ===\n");
    printf("1. Use default test puzzle (easy)\n");
    printf("2. Use challenging test puzzle\n");
    printf("3. Enter custom puzzle\n");
    printf("4. Exit\n");
    printf("Choose an option (1-4): ");
}

/**
 * Displays heuristic selection menu
 */
void display_heuristic_menu() {
    printf("\n=== HEURISTIC SELECTION ===\n");
    printf("1. Tiles Out of Place\n");
    printf("2. Manhattan Distance\n");
    printf("Choose heuristic (1-2): ");
}

/**
 * Main function - Entry point of the program
 */
int main() {
    printf("=================================================\n");
    printf("        N-PUZZLE SOLVER USING A* ALGORITHM      \n");
    printf("=================================================\n");
    printf("This program solves N-Puzzle using A* search\n");
    printf("with different heuristic functions.\n");

    int initial[MAX_SIZE][MAX_SIZE];
    int goal[MAX_SIZE][MAX_SIZE];
    int n = 3;  // Default 3x3 puzzle
    int choice, heuristic_choice;
    HeuristicType heuristic_type;

    while (1) {
        display_menu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("\nUsing default test puzzle (already solved):\n");
                setup_default_puzzle(initial, goal, n);
                break;

            case 2:
                printf("\nUsing challenging test puzzle:\n");
                setup_test_puzzle(initial, goal, n);
                break;

            case 3:
                printf("\nEnter puzzle size (3 for 3x3): ");
                scanf("%d", &n);

                if (n < 2 || n > MAX_SIZE) {
                    printf("Invalid size! Using default size 3.\n");
                    n = 3;
                }

                read_puzzle(initial, n, "initial");
                if (!is_valid_puzzle(initial, n)) {
                    printf("Invalid initial puzzle configuration!\n");
                    continue;
                }

                read_puzzle(goal, n, "goal");
                if (!is_valid_puzzle(goal, n)) {
                    printf("Invalid goal puzzle configuration!\n");
                    continue;
                }
                break;

            case 4:
                printf("Goodbye!\n");
                return 0;

            default:
                printf("Invalid choice! Please try again.\n");
                continue;
        }

        // Display selected puzzles
        printf("\nInitial State:\n");
        print_puzzle(initial, n);
        printf("\nGoal State:\n");
        print_puzzle(goal, n);

        // Select heuristic function
        display_heuristic_menu();
        scanf("%d", &heuristic_choice);

        switch (heuristic_choice) {
            case 1:
                heuristic_type = TILES_OUT_OF_PLACE;
                break;
            case 2:
                heuristic_type = MANHATTAN_DISTANCE;
                break;
            default:
                printf("Invalid choice! Using Manhattan Distance.\n");
                heuristic_type = MANHATTAN_DISTANCE;
                break;
        }

        // Perform A* search
        printf("\n=================================================\n");
        struct TreeNode* solution_tree = a_star_search(initial, goal, n, heuristic_type);

        if (solution_tree) {
            // Find goal node
            struct TreeNode* goal_node = find_goal_node(solution_tree);

            if (goal_node) {
                print_solution_path(goal_node);
                print_search_statistics(solution_tree, goal_node);
                display_final_search_tree(solution_tree);
            }

            free_tree(solution_tree);
        } else {
            printf("\nNo solution found or search failed.\n");
        }

        printf("\nPress Enter to continue...");
        getchar(); // consume leftover newline
        getchar(); // wait for Enter
    }

    return 0;
}