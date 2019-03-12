/*
** Ben Pittman
** Greg Smith
** Calvin Winston Fei
** Term Project - Source.cpp
** Static class for checking solutions.
** Assumptions: Assumes valid board size and 1D memory allocation
*/

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <set>
#include "board.h"
#include <chrono>
#include <string>

using namespace std::chrono;

int main() {
	// https://www.puzzles.ca/sudoku_puzzles/sudoku_easy_487.html
	int* test_board_easy = new int[81]{ 0, 7, 0, 0, 0, 1, 0, 0, 0,
										6, 0, 0, 0, 0, 0, 0, 0, 0, 
										0, 0, 0, 0, 5, 3, 0, 0, 0,
										0, 0, 0, 8, 0, 0, 0, 2, 0,
										0, 3, 0, 0, 4, 7, 1, 6, 0,
										4, 0, 0, 0, 0, 0, 5, 7, 0,
										0, 0, 0, 0, 1, 0, 7, 5, 0,
										0, 6, 0, 5, 2, 0, 0, 4, 0,
										3, 0, 0, 0, 0, 9, 0, 8, 6 };

	Board easy_sudoku = Board();
	easy_sudoku.set_board(test_board_easy);
	easy_sudoku.print_board();
	auto start = high_resolution_clock::now();
	easy_sudoku.annotate_potential_entries();
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);
	std::cout << duration.count() << "ms" << std::endl;
	easy_sudoku.print_cell(0);
	easy_sudoku.print_cell(1);
	//easy_sudoku.annotate_potential_entries();
	//easy_sudoku.print_board();

	return 0;
}