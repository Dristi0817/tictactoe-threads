# Multithreaded Tic Tac Toe (POSIX Threads)

Author: Dristi Barnwal

## Description
This program simulates a Tic-Tac-Toe game using two threads:
- Player X
- Player O

Each thread randomly selects positions on the board. A mutex lock is used to ensure safe access to the shared game board.

The board is printed every time a player makes a move inside the critical section.

The game ends when:
- A player wins, OR
- The board is full (draw)

## How to Compile
gcc -Wall -Wextra -pthread tictactoe_threads.c -o ttt

## How to Run
./ttt

## Concepts Used
- POSIX Threads (pthread)
- Mutex Locks
- Critical Section
- Race Condition Prevention
- Deadlock Avoidance
