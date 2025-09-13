#include "npuzzle.h"

/**
 * Calculates heuristic value based on the specified type
 */
int calculate_heuristic(int puzzle[MAX_SIZE][MAX_SIZE], int goal[MAX_SIZE][MAX_SIZE],
                        int n, HeuristicType type) {
    switch (type) {
        case TILES_OUT_OF_PLACE:
            return tiles_out_of_place(puzzle, goal, n);
        case MANHATTAN_DISTANCE:
            return manhattan_distance(puzzle, goal, n);
        default:
            return 0;
    }
}

/**
 * Tiles out of place heuristic: counts misplaced tiles
 * h(n) = number of misplaced tiles between current state and goal state
 */
int tiles_out_of_place(int puzzle[MAX_SIZE][MAX_SIZE], int goal[MAX_SIZE][MAX_SIZE], int n) {
    int misplaced = 0;
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            // Don't count the empty tile (0)
            if (puzzle[i][j] != 0 && puzzle[i][j] != goal[i][j]) {
                misplaced++;
            }
        }
    }
    
    return misplaced;
}

/**
 * Manhattan distance heuristic: sum of distances of tiles from their goal positions
 * h(n) = sum of |x1 - x2| + |y1 - y2| for all misplaced tiles
 */
int manhattan_distance(int puzzle[MAX_SIZE][MAX_SIZE], int goal[MAX_SIZE][MAX_SIZE], int n) {
    int total_distance = 0;
    
    // For each tile in the current puzzle
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int tile_value = puzzle[i][j];
            
            // Skip empty tile
            if (tile_value == 0) continue;
            
            // Find where this tile should be in the goal state
            int goal_row = -1, goal_col = -1;
            for (int gi = 0; gi < n; gi++) {
                for (int gj = 0; gj < n; gj++) {
                    if (goal[gi][gj] == tile_value) {
                        goal_row = gi;
                        goal_col = gj;
                        break;
                    }
                }
                if (goal_row != -1) break;
            }
            
            // Calculate Manhattan distance for this tile
            if (goal_row != -1 && goal_col != -1) {
                total_distance += abs(i - goal_row) + abs(j - goal_col);
            }
        }
    }
    
    return total_distance;
}