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
				if (*board[i][0]) {
					for (int j = 1; j < SUB_BOARD_SIZE; j++) {
						if (*board[i][j]) {
							row_vals.erase(j);
						}
					}
				}
			}

			// Fill potential entries in rows
			if (!row_vals.empty()) {
				for (int i = row * SUB_BOARD_SIZE; i < (row * SUB_BOARD_SIZE) + SUB_BOARD_SIZE; i++) {
					if (!*board[i][0]) {
						for (auto it = row_vals.begin(); it != row_vals.end(); ++it) {
							*board[i][*it] = true;
						}
					}
				}
			}
		}

		for (int col = 0; col < SUB_BOARD_SIZE; col++) {
			// scan col for filled in values and store in temp set
			std::set<int> col_vals;
			for (int i = col; i < col * SUB_BOARD_SIZE - col; i += SUB_BOARD_SIZE) {
				if (*board[i][0]) {
					for (int j = 1; j < SUB_BOARD_SIZE; j++) {
						if (*board[i][j]) {
							col_vals.insert(j);
						}
					}
				}
			}

			// Fill potential entries in columns
			if (!col_vals.empty()) {
				for (int i = col; i < col * BOARD_SIZE - col; i += SUB_BOARD_SIZE) {
					if (!*board[i][0]) {
						for (auto it = col_vals.begin(); it != col_vals.end(); ++it) {
							if (*board[i][*it] == true) {
								*board[i][*it] = false;
							}
						}
					}
				}
			}
		}
	}
