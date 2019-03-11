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
const int SUB_BOARD_DIM = 3;

class Board {

public:

	// Array of bool pointers to hold cells for board
	// 0 item in each array signifies filled or empty, 1-9 signifies filled value or potential value
	bool **board = (bool **)malloc(BOARD_SIZE * sizeof(bool *));
	int empty_cells = 0;

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
			else {
				++empty_cells;
			}
		}
	}

	// sets a cell
	void set_cell(int _row, int _col, int _val) {
		int board_cell = _row + _col * SUB_BOARD_SIZE;
		board[board_cell][0] = true;
		for (int i = 1; i < SUB_BOARD_SIZE + 1; i++) {
			if (board[board_cell][i] == true && i != _val) {
				board[board_cell][0] = false;
			}
		}
	}

	// method for finding potential values for empty cells
	void annotate_potential_entries() {	

		for (int row = 0; row < SUB_BOARD_SIZE; row++) {
			// find non-filled values in the row and add to set
			std::set<int> row_vals;
			for (int n = 1; n < SUB_BOARD_SIZE; n++) {
				row_vals.insert(n);
			}
			// remove values from set that correspond to filled cells in the row
			for (int i = row * SUB_BOARD_SIZE; i < (row * SUB_BOARD_SIZE) + SUB_BOARD_SIZE; i++) {
				if (board[i][0]) {
					for (int j = 1; j < SUB_BOARD_SIZE; j++) {
						if (board[i][j]) {
							row_vals.erase(j);
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

						// check for single potential value and set if true
						int count = 0;
						for (int j = 1; j < SUB_BOARD_SIZE; j++) {
							if (board[i][j] == true) {
								++count;
							}
						}
						if (count == 1) {
							board[i][0] = true;
							--empty_cells;
						}
					}
				}
			}
		}

		// scan col for filled in values and store in temp set
		for (int col = 0; col < SUB_BOARD_SIZE; col++) {
			std::set<int> col_vals;
			for (int i = col; i < col * SUB_BOARD_SIZE - col; i += SUB_BOARD_SIZE) {
				if (board[i][0]) {
					for (int j = 1; j < SUB_BOARD_SIZE; j++) {
						if (board[i][j]) {
							col_vals.insert(j);
						}
					}
				}
			}

			// Reduce potential entries for column intersections
			if (!col_vals.empty()) {
				for (int i = col; i < col * BOARD_SIZE - col; i += SUB_BOARD_SIZE) {
					if (!board[i][0]) {
						for (auto it = col_vals.begin(); it != col_vals.end(); ++it) {
							if (board[i][*it] == true) {
								board[i][*it] = false;
							}
						}

						// check for single potential value and set if true
						int count = 0;
						for (int j = 1; j < SUB_BOARD_SIZE; j++) {
							if (board[i][j] == true) {
								++count;
							}
						}
						if (count == 1) {
							board[i][0] = true;
							--empty_cells;
						}
					}
				}
			}
		}

		// Reduce potentials for sub grid intersections
		for (int row_block = 0; row_block < SUB_BOARD_SIZE; row_block += SUB_BOARD_DIM) {
			for (int col_block = 0; col_block < SUB_BOARD_SIZE; col_block += SUB_BOARD_DIM) {
				std::set<int> grid_vals;
				for (int row = 0; row < SUB_BOARD_DIM; row++) {
					for (int col = 0; col < SUB_BOARD_DIM; col++) {
						int loc = (row_block + row) * (col_block + col);
						if (board[loc][0]) {
							for (int j = 1; j < SUB_BOARD_SIZE; j++) {
								if (board[loc][j]) {
									grid_vals.insert(j);
								}
							}
						}
					}
				}
				
				for (int row = 0; row < SUB_BOARD_DIM; row++) {
					for (int col = 0; col < SUB_BOARD_DIM; col++) {
						int loc = (row_block + row) * (col_block + col);
						if (!board[loc][0]) {
							for (auto it = grid_vals.begin(); it != grid_vals.end(); ++it) {
								if (board[loc][*it] == true) {
									board[loc][*it] = false;
								}
							}

							// check for single potential value and set if true
							int count = 0;
							for (int i = 1; i < SUB_BOARD_SIZE; i++) {
								if (board[loc][i] == true) {
									++count;
								}
							}
							if (count == 1) {
								board[loc][0] = true;
								--empty_cells;
							}
						}
					}
				}
			}
		}
	}
	
	// Prints out the passed in sudoku game board
	// Assumes N is either 4, 9 or 16 but can be extended to add more sizes
	void print_board(int *sudoku) {

		char* border;
		if (SUB_BOARD_SIZE == 4) {
			border = new char[14]{ "|-----+-----|" };
		}
		else if (SUB_BOARD_SIZE == 9) {
			border = new char[26]{ "|-------+-------+-------|" };
		}
		else if (SUB_BOARD_SIZE == 16) {
			border = new char[42]{ "|---------+---------+---------+---------|" };
		}
		else {
			return;
		}

		std::cout << border << std::endl;
		int split = sqrt(SUB_BOARD_SIZE);
		for (int i = 0; i < SUB_BOARD_SIZE*SUB_BOARD_SIZE; i++) {
			if (i % SUB_BOARD_SIZE == 0) {
				std::cout << "| ";
			}
			else if (i % split == 0) {
				std::cout << "| ";
			}
			
			// change to call a get_entry fucntion that will return the value
			int value = sudoku[i];
			if (value != 0) {
				std::cout << value << " ";
			}
			else {
				std::cout << ". ";
			}

			if (i % SUB_BOARD_SIZE == SUB_BOARD_SIZE - 1) {
				std::cout << "|" << std::endl;

				if (((i + 1) % (SUB_BOARD_SIZE * SUB_BOARD_SIZE / split)) == 0) {
					std::cout << border << std::endl;
				}
			}
		}
		std::cout << std::endl;
}
};
