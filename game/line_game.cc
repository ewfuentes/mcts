#include "game/line_game.hh"

namespace game {
Step<LineState> step_game(const LineState &state, const LineAction &action) {
	Step<LineState> out{}; out.new_state.position = state.position + action.step;
	if (out.new_state.position >= 10) {
        out.end_condition = game::WIN;
	} else if (out.new_state.position <= -10) {
        out.end_condition = game::LOSS;
    } else {
        out.end_condition = game::ONGOING;
	}
	return out;	
}

std::set<LineAction> available_actions(const LineState &state) {
	(void)state;
	return {{-1}, {1}};
}

int current_player(const LineState &state) { return 1; }

game::GameOps<LineState, LineAction> get_line_game_ops() {
	return game::GameOps<LineState, LineAction>{step_game, available_actions, current_player};
}
}  // namespace game
