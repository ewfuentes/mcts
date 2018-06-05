#pragma once
#include "game.hh" 
namespace game {
struct LineState {
	int position = 0;
};

struct LineAction {
	int step = 0;
	
	constexpr bool operator<(const LineAction &other) const {
		return step < other.step;
	}
};

Step<LineState> step_game(const LineState &state, const LineAction &action);

std::set<LineAction> available_actions(const LineState &state);

game::GameOps<LineState, LineAction> get_line_game_ops();

}  // namespace game
