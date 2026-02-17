/**
 * @file tictactoe_threads.c
 * @author Dristi Barnwal
 * @brief Multi-threaded Tic-Tac-Toe using POSIX threads and mutex.
 *
 * This program simulates a Tic-Tac-Toe game using two threads:
 * Player X and Player O. Each thread randomly selects positions
 * on the board. A mutex is used to ensure safe access to the shared
 * game board (no race conditions or deadlocks).
 *
 * The board is printed every time a player makes a move while
 * inside the critical section (mutex lock).
 *
 * The program terminates when either player wins or when the board
 * is full (draw).
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define N 3
#define EMPTY '.'

/** Shared Tic-Tac-Toe board */
char board[N][N];

/** Flag to indicate if the game is over */
int game_over = 0;

/** Stores winner: 'X', 'O', or '.' for draw */
char winner = EMPTY;

/** Mutex to protect shared board */
pthread_mutex_t lock;

/**
 * @brief Initializes the board with empty cells.
 */
void init_board() {
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            board[i][j] = EMPTY;
        }
    }
}

/**
 * @brief Prints the current state of the board.
 * @param player The player who made the move (X or O)
 */
void print_board(char player) {
    printf("\nPlayer %c inside mutex_lock\n", player);
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            printf(" %c", board[i][j]);
        }
        printf("\n");
    }
}

/**
 * @brief Checks if there is a winner.
 * @return 'X', 'O', or '.' if no winner yet.
 */
char check_winner() {
    // rows and columns
    for(int i = 0; i < N; i++) {
        if(board[i][0] != EMPTY &&
           board[i][0] == board[i][1] &&
           board[i][1] == board[i][2]) return board[i][0];

        if(board[0][i] != EMPTY &&
           board[0][i] == board[1][i] &&
           board[1][i] == board[2][i]) return board[0][i];
    }

    // diagonals
    if(board[0][0] != EMPTY &&
       board[0][0] == board[1][1] &&
       board[1][1] == board[2][2]) return board[0][0];

    if(board[0][2] != EMPTY &&
       board[0][2] == board[1][1] &&
       board[1][1] == board[2][0]) return board[0][2];

    return EMPTY;
}

/**
 * @brief Checks if the board is full.
 * @return 1 if full, 0 otherwise.
 */
int is_full() {
    for(int i = 0; i < N; i++)
        for(int j = 0; j < N; j++)
            if(board[i][j] == EMPTY) return 0;
    return 1;
}

/**
 * @brief Thread function for each player.
 * @param arg pointer to char ('X' or 'O')
 */
void* player_thread(void* arg) {
    char player = *(char*)arg;
    unsigned int seed = time(NULL) + player;

    while(1) {
        pthread_mutex_lock(&lock);

        if(game_over) {
            pthread_mutex_unlock(&lock);
            break;
        }

        int row, col;

        // find random empty cell
        do {
            row = rand_r(&seed) % 3;
            col = rand_r(&seed) % 3;
        } while(board[row][col] != EMPTY);

        // place move
        board[row][col] = player;

        // print board inside critical section
        print_board(player);

        // check winner
        char w = check_winner();
        if(w != EMPTY) {
            game_over = 1;
            winner = w;
            pthread_mutex_unlock(&lock);
            break;
        }

        // check draw
        if(is_full()) {
            game_over = 1;
            winner = EMPTY;
            pthread_mutex_unlock(&lock);
            break;
        }

        pthread_mutex_unlock(&lock);

        // sleep so threads alternate nicely
        usleep(100000);
    }

    return NULL;
}

/**
 * @brief Main function to start threads and control program flow.
 */
int main() {
    pthread_t t1, t2;
    char p1 = 'X', p2 = 'O';

    pthread_mutex_init(&lock, NULL);
    init_board();

    pthread_create(&t1, NULL, player_thread, &p1);
    pthread_create(&t2, NULL, player_thread, &p2);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("\n=== GAME OVER ===\n");
    if(winner == EMPTY)
        printf("Result: Draw\n");
    else
        printf("Winner: %c\n", winner);

    pthread_mutex_destroy(&lock);
    return 0;
}
