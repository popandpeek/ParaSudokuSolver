/*
** Ben Pittman
** Greg Smith
** Calvin Winston Fei
** Term Project - board.h
** Static class for checking solutions.
** Assumptions: Assumes valid board size and 1D memory allocation
*/

#include <stdio.h>
#include <iostream>
#include <set>

#pragma once

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

	Board();

	~Board();

	// Method to set the board according to passed integer array
	// assumes the filled integer array is of size BOARD_SIZE contains only values between 1 and 9	
	void set_board(int*);

	// sets a cell
	void set_cell(int, int, int);

	// method for finding potential values for empty cells
	void annotate_potential_entries();

	// Helper method to get value in cell
	int get_entry(int);

	int* get_potentials(int);

	std::set<int> get_potential_set(int _loc);

	void remove_potential_values(std::set<int> _vals, int _loc);

	void remove_potential_values_from_row(std::set<int> _vals, int row_start);

	void remove_potential_values_from_col(std::set<int> _vals, int col_start);

	void remove_doubles_and_triples_by_sub_grid();

	// Prints out the passed in sudoku game board
	// Assumes N is either 4, 9 or 16 but can be extended to add more sizes
	void print_board();

	void print_cell(int);

};