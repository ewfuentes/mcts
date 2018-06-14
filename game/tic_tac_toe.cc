#include "game/tic_tac_toe.hh"

#include <algorithm>
#include <iterator>

namespace game {
using State = TicTacToeState;
using Action = TicTacToeAction;
namespace {
State::CellState check_for_match(const State &state, int start, int stride) {
	if (state.board[start] == state.board[start + stride] &&
		state.board[start] == state.board[start + 2 * stride] &&
		state.board[start] != State::EMPTY) {
		return state.board[start];
	}
	return State::EMPTY;
}

EndCondition check_game_state(const State &state, const State::CellState &current_player) {
	std::vector<State::CellState> results;
	// Check rows and columns
	for (int i = 0; i < 3; i++) {
		// Check ith row
		results.push_back(check_for_match(state, 3*i, 1));
		// Check ith columns
		results.push_back(check_for_match(state, i, 3));
	}
	// Check the diagonals
	results.push_back(check_for_match(state, 0, 4));
	results.push_back(check_for_match(state, 2, 2));
	
	// Check to see if there are any wins
	const auto iter = std::find_if_not(results.begin(),
									   results.end(),
									   [](const State::CellState &state) {
									       return state == State::EMPTY;
									   });
	// Finally check to see if the board is full
	const auto first_empty_iter = std::find_if_not(
		std::begin(state.board), std::end(state.board),
		[](const State::CellState &state) { return state == State::EMPTY; });
	const bool has_empty = first_empty_iter != std::end(state.board);
	if (iter == results.end()) {
		return has_empty ? EndCondition::ONGOING : EndCondition::DRAW;
	}
	return *iter == current_player ? EndCondition::WIN : EndCondition::LOSS;
}
}  // namespace

Step<State> step_game(const State &state, const Action &action) {
	Step<State> step;
	// Check if the action is valid. If it is not, return the
	// same state, otherwise apply the action
	const bool is_valid_action = state.board[action.position] == State::EMPTY;
	const bool is_game_over =
		check_game_state(state, state.current_player) != ONGOING;

	step.new_state = state;
	if (is_valid_action && !is_game_over) {
		step.new_state.board[action.position] = state.current_player;	
	}
	
	// Check the win conditions
	step.end_condition = check_game_state(state, state.current_player);

	// Set the next player
	if (is_valid_action && step.end_condition == ONGOING) {
		step.new_state.current_player = state.current_player == State::X ?
			State::O : State::X;
	} else {
		step.new_state.current_player = state.current_player;
	}
	return step;
}

std::set<TicTacToeAction> available_actions(const TicTacToeState &state) {
	std::set<TicTacToeAction> out = {};
	for (int pos = 0; pos < State::NUM_POSITIONS; pos++) {
		if (state.board[pos] == TicTacToeState::EMPTY) {
			out.insert({pos});
		}
	}
	return out;
} 
int current_player(const TicTacToeState &state) {
	return state.current_player;
}

game::GameOps<TicTacToeState, TicTacToeAction> get_tic_tac_toe_ops() {
	return {
		.step_game = step_game,
		.available_actions = available_actions,
		.current_player = current_player,
	};
}
}  // namespace game
