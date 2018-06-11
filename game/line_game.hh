#pragma once

#include "game/game.hh" 

namespace game {
struct LineState {
	int position = 0;
};

struct LineAction {
	int step = 0;
	
	// Action must define this operator so that they can be inserted into a 
	// std::set
	constexpr bool operator<(const LineAction &other) const {
		return step < other.step;
	}
};

// Given a state and an action produce the next state
Step<LineState> step_game(const LineState &state, const LineAction &action);

// Given a state, return the available actions
std::set<LineAction> available_actions(const LineState &state);

// Get all of the ops for this game
game::GameOps<LineState, LineAction> get_line_game_ops();

}  // namespace game
