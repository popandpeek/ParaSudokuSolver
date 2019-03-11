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
	
	// Used in the print function
	char* border;
	
	Board() {
		for (int i = 0; i < BOARD_SIZE; i++) {
			board[i] = (bool*)malloc((SUB_BOARD_SIZE + 1) * sizeof(bool));
		}
	}
	
	// Destructor to free memory allocations
	~Board() {
		for (int i = 0; i < BOARD_SIZE; i++) {
			free(board[i]);
		}

		free(board);
		delete(border);
	}

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

	// Helper method to get value in cell
	int get_entry(int _loc) {
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

	// Helper method to get potential values in an unfilled cell
	std::set<int> get_potential_set(int _loc) {
		std::set<int> vals;
		if (board[_loc][0] == false) {
			for (int i = 1; i < SUB_BOARD_SIZE + 1; i++) {
				if (board[_loc][i] == true) {
					vals.insert(i)
				}
			}
		}
		return vals;
	}

	// Helper method to remove specified potential values from a cell
	void remove_potential_values(std::set<int> _vals, int _loc) {
		if (board[_loc][0] == false) {
			for (auto it = _vals.begin(); it != _vals.end(); ++it) {
				board[_loc][*it] = false;
			}
		}
	}

	// Helper to remove potential values from a row of a sub-grid
	// assumes row_start is the leftmost cell of the row
	void remove_potential_values_from_row(std::set<int> _vals, int row_start) {
		remove_potential_values(cell_1, row_start);
		remove_potential_values(cell_1, row_start + 1);
		remove_potential_values(cell_1, row_start + 2);
	}


	// Helper to remove potential values from a col of a sub-grid
	// assumes col_start is the topmost cell of the col
	void remove_potential_values_from_col(std::set<int> _vals, int col_start) {
		remove_potential_values(cell_1, col_start);
		remove_potential_values(cell_1, col_start + 9);
		remove_potential_values(cell_1, col_start + 18);
	}


	// combs through sub-grids and removes potential values from them if a double or triple is found
	// sub-grid dims: s-g(0, 0) : top left, s-g(2,2) : bottom right for 9x9 sudoku
	void remove_doubles_and_triples_by_sub_grid() {

		// Iterate by sub grid 
		for (int sub_grid_row = 0; sub_grid_row < SUB_BOARD_DIM; sub_grid_row++) {
			for (int sub_grid_col = 0; sub_grid_col < SUB_BOARD_DIM; sub_grid_col++) {

				// Iterate through sub-grid rows first
				int grid_start = (SUB_BOARD_SIZE * sub_grid_row) + (SUB_BOARD_DIM * sub_grid_col);

				// For 9x9 sudoku
				if (SUB_BOARD_DIM == 3) {

					// For each row, we get the 3 potential sets for the cells
					for (int row = 0; row < SUB_BOARD_DIM; row++) {
						std::set<int>cell_1 = get_potential_set(row * SUB_BOARD_SIZE + grid_start);
						std::set<int>cell_2 = get_potential_set(row * SUB_BOARD_SIZE + grid_start + 1);
						std::set<int>cell_3 = get_potential_set(row * SUB_BOARD_SIZE + grid_start + 2);

						// check for triples
						if (cell_1.size() == 3 && cell_2.size() == 3 && cell_3.size() == 3) {
							if (cell_1 == cell_2 && cell_1 == cell_3) { // found a row triple

								// remove triple from all grid cells besides these 3
								for (int row_remove = 0; row_remove < SUB_BOARD_DIM; row_remove++) {
									if (row_remove != rows) {
										remove_potential_values_from_row(cell_1, SUB_BOARD_SIZE * row_remove + grid_start);
									}
								}

								continue; // If triple was found, we don't want to waste time checking for a double
							}
						}

						// check for doubles - things gonna get a bit messy
						// start with cell 1
						if (cell_1.size() == 2) {
							if (cell_1 == cell_2) { // double found
								// remove cell_3 vals and then the potential vals from other 2 rows
								remove_potential_values(cell_2, row * SUB_BOARD_SIZE + grid_start + 2);
								for (int row_remove = 0; row_remove < SUB_BOARD_DIM; row_remove++) {
									if (row_remove != rows) {
										remove_potential_values_from_row(cell_2, SUB_BOARD_SIZE * row_remove + grid_start);
									}
								}
							} 
							else if (cell_1 == cell_3) { // double found
								// remove cell_2 vals and then the potential vals from other 2 rows
								remove_potential_values(cell_2, row * SUB_BOARD_SIZE + grid_start + 1);
								for (int row_remove = 0; row_remove < SUB_BOARD_DIM; row_remove++) {
									if (row_remove != rows) {
										remove_potential_values_from_row(cell_2, SUB_BOARD_SIZE * row_remove + grid_start);
									}
								}
							}
						} // cell 1 out of the running
						else if (cell_2.size() == 2 && cell_3.size() == 2) {
							if (cell_2 == cell_3) { // double found
								// remove cell_1 vals and then the potential vals from other 2 rows
								remove_potential_values(cell_2, row * SUB_BOARD_SIZE + grid_start);
								for (int row_remove = 0; row_remove < SUB_BOARD_DIM; row_remove++) {
									if (row_remove != rows) {
										remove_potential_values_from_row(cell_2, SUB_BOARD_SIZE * row_remove + grid_start);
									}
								}
							}
						}

						// no doubles or triples found on this row
					}

					// Now we do columns
					for (int cols = 0; col < SUB_BOARD_DIM; col++) {
						std::set<int>cell_1 = get_potential_set(row * SUB_BOARD_SIZE + grid_start);
						std::set<int>cell_2 = get_potential_set(row * SUB_BOARD_SIZE + grid_start + SUB_BOARD_SIZE);
						std::set<int>cell_3 = get_potential_set(row * SUB_BOARD_SIZE + grid_start + SUB_BOARD_SIZE * 2);

						// check for triples
						if (cell_1.size() == 3 && cell_2.size() == 3 && cell_3.size() == 3) {
							if (cell_1 == cell_2 && cell_1 == cell_3) { // found a row triple

								// remove triple from all grid cells besides these 3
								for (int col_remove = 0; col_remove < SUB_BOARD_DIM; col_remove++) {
									if (col_remove != col) {
										remove_potential_values_from_col(cell_1, SUB_BOARD_SIZE * col_remove + grid_start);
									}
								}

								continue; // If triple was found, we don't want to waste time checking for a double
							}
						}

						// check for doubles - things gonna get a bit messy
						// start with cell 1
						if (cell_1.size() == 2) {
							if (cell_1 == cell_2) { // double found
								// remove cell_3 vals and then the potential vals from other 2 cols
								remove_potential_values(cell_2, col * SUB_BOARD_SIZE + grid_start + SUB_BOARD_SIZE * 2);
								for (int col_remove = 0; col_remove < SUB_BOARD_DIM; col_remove++) {
									if (col_remove != rows) {
										remove_potential_values_from_row(cell_2, SUB_BOARD_SIZE * col_remove + grid_start);
									}
								}
							}
							else if (cell_1 == cell_3) { // double found
								// remove cell_2 vals and then the potential vals from other 2 rows
								remove_potential_values(cell_2, col * SUB_BOARD_SIZE + grid_start + SUB_BOARD_SIZE);
								for (int row_remove = 0; row_remove < SUB_BOARD_DIM; row_remove++) {
									if (row_remove != rows) {
										remove_potential_values_from_row(cell_2, SUB_BOARD_SIZE * row_remove + grid_start);
									}
								}
							}
						} // cell 1 out of the running
						else if (cell_2.size() == 2 && cell_3.size() == 2) {
							if (cell_2 == cell_3) { // double found
								// remove cell_1 vals and then the potential vals from other 2 rows
								remove_potential_values(cell_2, col * SUB_BOARD_SIZE + grid_start);
								for (int col_remove = 0; col_remove < SUB_BOARD_DIM; col_remove++) {
									if (col_remove != rows) {
										remove_potential_values_from_col(cell_2, SUB_BOARD_SIZE * col_remove + grid_start);
									}
								}
							}
						}

						// no doubles or triples found on this col
					}
				}
				else {
					// TODO: Any other sudoku dimensions.
					//  e.g. 16x16 sudoku which will need to check for quadruples as well.
				}
			}
		}
	}

	// Prints out current board state
	void print_board() {

		char* border = new char[26]{ "|-------+-------+-------|" };

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
};
