/*
** Ben Pittman
** Greg Smith
** Calvin Winston Fei
** Term Project - board.cpp
** Static class for checking solutions.
** Assumptions: Assumes valid board size and 1D memory allocation
*/

#include <stdio.h>
#include <iostream>
#include <set>

const int BOARD_SIZE = 81;
const int SUB_BOARD_SIZE = 9;

class Board {

public:

	// Array of bool pointers to hold cells for board
	// 0 item in each array signifies filled or empty, 1-9 signifies filled value or potential value
	bool *board[BOARD_SIZE];
	Board() {
		for (int i = 0; i < BOARD_SIZE; i++) {
			board[i] = (bool*)malloc((SUB_BOARD_SIZE + 1) * sizeof(bool));
		}
	};

	// Method to set the board according to passed integer array
	// assumes the filled integer array is of size BOARD_SIZE contains only values between 1 and 9
	void set_board(int* filled) {
		for (int i = 0; i < BOARD_SIZE; i++) {
			if (filled[i] != 0) {
				board[i][0] = true;
				board[i][filled[i]] = true;
			}
		}
	}

	// method for finding potential values for empty cells
	void annotate_potential_entries(int row, int col) {
		// scan row for values and store in temp set
		std::set<int> row_vals;
		std::set<int> col_vals;

		// find filled cells in the row and add to array
		for (int i = row * SUB_BOARD_SIZE; i < (row * SUB_BOARD_SIZE) + SUB_BOARD_SIZE; i++) {
			if (board[i][0]) {
				for (int j = 1; j < SUB_BOARD_SIZE; j++) {
					if (board[i][j]) {
						row_vals.insert(j);
					}
				}
			}
		}

		// scan col for values and store in temp set
		for (int i = col; i < col * SUB_BOARD_SIZE - col; i += SUB_BOARD_SIZE) {
			if (board[i][0]) {
				for (int j = 1; j < SUB_BOARD_SIZE; j++) {
					if (board[i][j]) {
						col_vals.insert(j);
					}
				}
			}
		}

		// Fill potential entries in rows
		if (!row_vals.empty()) {
			for (int i = row * SUB_BOARD_SIZE; i < (row * SUB_BOARD_SIZE) + SUB_BOARD_SIZE; i++) {
				if (!board[i][0]) {
					for (auto it = row_vals.begin(); it != row_vals.end(); ++it) {
						board[i][*it] = true;
					}
				}
			}
		}

		// Fill potential entries in columns
		if (!col_vals.empty()) {
			for (int i = col; i < col * BOARD_SIZE - col; i += SUB_BOARD_SIZE) {
				if (board[i][0]) {
					for (auto it = col_vals.begin(); it != col_vals.end(); ++it) {
						board[i][*it] = true;
					}
				}
			}
		}
	}
};