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
	// Used in the print function
	char* border;

	// Array of bool pointers to hold cells for board
	// 0 item in each array signifies filled or empty, 1-9 signifies filled value or potential value
	bool **board = (bool **)malloc(BOARD_SIZE * sizeof(bool *));
	int empty_cells = 0;

	Board() {
		for (int i = 0; i < BOARD_SIZE; i++) {
			board[i] = (bool*)malloc((SUB_BOARD_SIZE + 1) * sizeof(bool));
			for (int j = 0; j < SUB_BOARD_SIZE + 1; j++) {
				board[i][j] = false;
			}
		}
	}

	// Destructor to free memory allocations
	~Board() {
		for (int i = 0; i < BOARD_SIZE; i++) {
			free(board[i]);
		}

		free(board);
		delete border;
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

	int* get_potentials(int _loc) {
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

	// Helper method to get potential values in an unfilled cell
	std::set<int> get_potential_set(int _loc) {
		std::set<int> vals;
		if (board[_loc][0] == false) {
			for (int i = 1; i < SUB_BOARD_SIZE + 1; i++) {
				if (board[_loc][i] == true) {
					vals.insert(i);
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
		remove_potential_values(_vals, row_start);
		remove_potential_values(_vals, row_start + 1);
		remove_potential_values(_vals, row_start + 2);
	}


	// Helper to remove potential values from a col of a sub-grid
	// assumes col_start is the topmost cell of the col
	void remove_potential_values_from_col(std::set<int> _vals, int col_start) {
		remove_potential_values(_vals, col_start);
		remove_potential_values(_vals, col_start + SUB_BOARD_SIZE);
		remove_potential_values(_vals, col_start + SUB_BOARD_SIZE * 2);
	}

	// combs through sub-grids and removes potential values from them if a double or triple is found
	// sub-grid dims: s-g(0, 0) : top left, s-g(2,2) : bottom right for 9x9 sudoku
	void remove_doubles_and_triples_by_sub_grid() {

		// Iterate by sub grid 
		for (int sub_grid_row = 0; sub_grid_row < SUB_BOARD_DIM; sub_grid_row++) {
			for (int sub_grid_col = 0; sub_grid_col < SUB_BOARD_DIM; sub_grid_col++) {

				// Iterate through sub-grid rows first
				int grid_start = (SUB_BOARD_DIM * SUB_BOARD_SIZE * sub_grid_row) + (SUB_BOARD_DIM * sub_grid_col);

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
									if (row_remove != row) {
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
									if (row_remove != row) {
										remove_potential_values_from_row(cell_2, SUB_BOARD_SIZE * row_remove + grid_start);
									}
								}
							}
							else if (cell_1 == cell_3) { // double found
								// remove cell_2 vals and then the potential vals from other 2 rows
								remove_potential_values(cell_2, row * SUB_BOARD_SIZE + grid_start + 1);
								for (int row_remove = 0; row_remove < SUB_BOARD_DIM; row_remove++) {
									if (row_remove != row) {
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
									if (row_remove != row) {
										remove_potential_values_from_row(cell_2, SUB_BOARD_SIZE * row_remove + grid_start);
									}
								}
							}
						}

						// no doubles or triples found on this row
					}

					// Now we do columns
					for (int col = 0; col < SUB_BOARD_DIM; col++) {
						std::set<int>cell_1 = get_potential_set(col + grid_start);
						std::set<int>cell_2 = get_potential_set(col + grid_start + SUB_BOARD_SIZE);
						std::set<int>cell_3 = get_potential_set(col + grid_start + SUB_BOARD_SIZE * 2);

						// check for triples
						if (cell_1.size() == 3 && cell_2.size() == 3 && cell_3.size() == 3) {
							if (cell_1 == cell_2 && cell_1 == cell_3) { // found a row triple

								// remove triple from all grid cells besides these 3
								for (int col_remove = 0; col_remove < SUB_BOARD_DIM; col_remove++) {
									if (col_remove != col) {
										remove_potential_values_from_col(cell_1, col_remove + grid_start);
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
								remove_potential_values(cell_2, col + grid_start + SUB_BOARD_SIZE * 2);
								for (int col_remove = 0; col_remove < SUB_BOARD_DIM; col_remove++) {
									if (col_remove != col) {
										remove_potential_values_from_col(cell_2, col_remove + grid_start);
									}
								}
							}
							else if (cell_1 == cell_3) { // double found
								// remove cell_2 vals and then the potential vals from other 2 rows
								remove_potential_values(cell_2, col + grid_start + SUB_BOARD_SIZE);
								for (int col_remove = 0; col_remove < SUB_BOARD_DIM; col_remove++) {
									if (col_remove != col) {
										remove_potential_values_from_col(cell_2, col_remove + grid_start);
									}
								}
							}
						} // cell 1 out of the running
						else if (cell_2.size() == 2 && cell_3.size() == 2) {
							if (cell_2 == cell_3) { // double found
								// remove cell_1 vals and then the potential vals from other 2 rows
								remove_potential_values(cell_2, col + grid_start);
								for (int col_remove = 0; col_remove < SUB_BOARD_DIM; col_remove++) {
									if (col_remove != col) {
										remove_potential_values_from_col(cell_2, col_remove + grid_start);
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

	// Prints out the passed in sudoku game board
	// Assumes N is either 4, 9 or 16 but can be extended to add more sizes
	void print_board() {

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

	void print_cell(int _loc) {
		for (int i = 0; i < SUB_BOARD_SIZE + 1; i++) {
			std::cout << i << " : " << board[_loc][i] << std::endl;
		}
	}
};