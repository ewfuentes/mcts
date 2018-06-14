#pragma once

#include <set>

#include "game/game.hh"

namespace game {
struct TicTacToeState {
	static constexpr int NUM_POSITIONS = 9;
	enum CellState { EMPTY, X, O };
	CellState board[NUM_POSITIONS] = { EMPTY };
	CellState current_player = X;
};

struct TicTacToeAction {
	int position = -1;
	constexpr bool operator<(const TicTacToeAction &other) const {
		return position < other.position;
	}
};

Step<TicTacToeState> step_game(const TicTacToeState &state,
								const TicTacToeAction &action);

std::set<TicTacToeAction> available_actions(const TicTacToeState &state);

int current_player(const TicTacToeState &state);

game::GameOps<TicTacToeState, TicTacToeAction> get_tic_tac_toe_ops();
}  // namespace game
