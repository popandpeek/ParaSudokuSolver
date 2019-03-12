/*
** Ben Pittman
** Greg Smith
** Calvin Winston Fei
** Term Project - board.cpp
** Static class for checking solutions.
** Assumptions: Assumes valid board size and 1D memory allocation
*/

#include "board.h"

Board::Board() {
	for (int i = 0; i < BOARD_SIZE; i++) {
		board[i] = (bool*)malloc((SUB_BOARD_SIZE + 1) * sizeof(bool));
		for (int j = 0; j < SUB_BOARD_SIZE + 1; j++) {
			board[i][j] = false;
		}
	}
}

// Destructor to free memory allocations
Board::~Board() {
	for (int i = 0; i < BOARD_SIZE; i++) {
		free(board[i]);
	}

	free(board);
	delete border;
}

// Method to set the board according to passed integer array
// assumes the filled integer array is of size BOARD_SIZE contains only values between 1 and 9
void Board::set_board(int* filled) {
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
void Board::set_cell(int _row, int _col, int _val) {
	int board_cell = _row + _col * SUB_BOARD_SIZE;
	board[board_cell][0] = true;
	for (int i = 1; i < SUB_BOARD_SIZE + 1; i++) {
		if (board[board_cell][i] == true && i != _val) {
			board[board_cell][0] = false;
		}
	}
}

// method for finding potential values for empty cells
void Board::annotate_potential_entries() {
	// std::cout << empty_cells << std::endl;
	for (int row = 0; row < SUB_BOARD_SIZE; row++) {
		// find non-filled values in the row and add to set
		std::set<int> row_vals;
		for (int n = row * SUB_BOARD_SIZE; n < SUB_BOARD_SIZE + (row * SUB_BOARD_SIZE); n++) {
			if (board[n][0] == true) {
				for (int k = 1; k < SUB_BOARD_SIZE + 1; k++) {
					if (board[n][k] == true) {
						row_vals.insert(k);
						break;
					}
				}
			}
		}

		// std::cout << row_vals.size() << std::endl;
		// remove values from set that correspond to filled cells in the row
		for (int i = row * SUB_BOARD_SIZE; i < SUB_BOARD_SIZE + (row * SUB_BOARD_SIZE); i++) {
			if (board[i][0] == true) {
				for (int j = 1; j < SUB_BOARD_SIZE; j++) {
					if (board[i][j] == true) {
						row_vals.erase(j);
					}
				}
			}
		}

		// std::cout << "Got to 76" << std::endl;
		// Fill potential entries in rows
		if (!row_vals.empty()) {
			for (int i = row * SUB_BOARD_SIZE; i < (row * SUB_BOARD_SIZE) + SUB_BOARD_SIZE; i++) {
				if (board[i][0] == false) {
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

	// std::cout << "Got to 101" << std::endl;
	// scan col for filled in values and store in temp set
	for (int col = 0; col < SUB_BOARD_SIZE; col++) {
		std::set<int> col_vals;
		for (int i = col; i < BOARD_SIZE - SUB_BOARD_SIZE + col + 1; i += SUB_BOARD_SIZE) {
			// std::cout << "Got to 106" << std::endl;
			if (board[i][0] == true) {
				for (int j = 1; j < SUB_BOARD_SIZE + 1; j++) {
					if (board[i][j] == true) {
						col_vals.insert(j);
						break;
					}
				}
			}
		}
		// std::cout << col_vals.size() << std::endl;
		// std::cout << "Got to 115" << std::endl;
		// Reduce potential entries for column intersections
		if (!col_vals.empty()) {
			for (int i = col; i < BOARD_SIZE - SUB_BOARD_SIZE + col; i += SUB_BOARD_SIZE) {
				if (board[i][0] == false) {
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
	// std::cout << "Got to 141" << std::endl;
	// Reduce potentials for sub grid intersections
	for (int grid_x = 0; grid_x < SUB_BOARD_DIM; grid_x++) {
		for (int grid_y = 0; grid_y < SUB_BOARD_DIM; grid_y++) {
			std::set<int> grid_vals;
			int grid_start = grid_x * 9 * 3 + grid_y * 3;
			for (int row = 0; row < SUB_BOARD_DIM; row++) {
				for (int loc = grid_start + row * 9; loc < (grid_start + row * 9) + 3; loc++) {
					if (board[loc][0] == true) {
						for (int j = 1; j < SUB_BOARD_SIZE + 1; j++) {
							if (board[loc][j] == true) {
								grid_vals.insert(j);
								break;
							}
						}
					}
				}
			}

			// std::cout << grid_vals.size() << std::endl;
			for (int row = 0; row < SUB_BOARD_DIM; row++) {
				for (int col = 0; col < SUB_BOARD_DIM; col++) {
					int loc = (grid_x + row) * (grid_y + col);
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
	// std::cout << empty_cells << std::endl;
}

// Helper method to get value in cell
int Board::get_entry(int _loc) {
	int ret_val = 0;
	if (board[_loc][0] == true) {
		for (int i = 1; i < SUB_BOARD_SIZE + 1; i++) {
			if (board[_loc][i] == true) {
				ret_val = i;
			}
		}
	}

	return ret_val;
}

int* Board::get_potentials(int _loc) {
	int* to_pass = nullptr;
	if (board[_loc][0] == false) {
		to_pass = new int[SUB_BOARD_SIZE];
		for (int i = 0; i < SUB_BOARD_SIZE; i++) {
			if (board[_loc][i] == true) {
				to_pass[i] = i;
			}

			else {
				to_pass[i] = 0;
			}
		}
	}

	return to_pass;
}

// Prints out the passed in sudoku game board
// Assumes N is either 4, 9 or 16 but can be extended to add more sizes
void Board::print_board() {

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
	for (int i = 0; i < BOARD_SIZE; i++) {
		if (i % SUB_BOARD_SIZE == 0) {
			std::cout << "| ";
		}
		else if (i % split == 0) {
			std::cout << "| ";
		}

		// change to call a get_entry function that will return the value
		int value = get_entry(i);
		if (value != 0) {
			std::cout << value << " ";
		}
		else {
			std::cout << ". ";
		}

		if (i % SUB_BOARD_SIZE == SUB_BOARD_SIZE - 1) {
			std::cout << "|" << std::endl;

			if (((i + 1) % (BOARD_SIZE / split)) == 0) {
				std::cout << border << std::endl;
			}
		}
	}
	std::cout << std::endl;
}

void Board::print_cell(int _loc) {
	for (int i = 0; i < SUB_BOARD_SIZE + 1; i++) {
		std::cout << i << " : " << board[_loc][i] << std::endl;
	}
}