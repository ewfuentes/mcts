#include "line_game.hh"

#include <iostream>

int main() {
	game::LineState state{};
	game::StepGame<game::LineState, game::LineAction> step_game = game::step_game;
	game::AvailableActions<game::LineState, game::LineAction> avail_actions =
        game::available_actions;
	(void) state;
	(void) step_game;
	(void) avail_actions;
}
