#include <memory>
#include <iostream>
#include <random>

#include "mcts/game_tree.hh"
#include "game/line_game.hh"

using GNode = mcts::Node<game::LineState, game::LineAction>;

std::unique_ptr<GNode> make_game_tree() {
	const game::LineState init_state;
	GNode root = GNode(init_state);

	return std::make_unique<GNode>(std::move(root));
}

void print_game_tree(const mcts::Node<game::LineState, game::LineAction> *const root,
                     const int depth = 0) {
	std::cout << "State: " << root->state.position
			  << " p: " << root->games_played
			  << " w: " << root->games_won
              << std::endl;
	for (const auto &pair : root->next_states) {
		for (int i = 0; i < depth + 1; i++) {
			std::cout << " ";
        }
		std::cout << "Action: " << pair.first.step << " ";
        print_game_tree(pair.second.get(), depth + 1);
    }
}


int main() {
	const game::LineState init_state;
	std::mt19937 gen(0);
	
	std::unique_ptr<GNode> root = make_game_tree();
	const auto ops = game::get_line_game_ops();	

	for (int i = 0; i < 10; i++) {
		print_game_tree(root.get());
		auto selected_node = select_node(root.get());
		auto sim_node = expand_node(selected_node, ops);
		const game::EndCondition result = simulate_playout(sim_node, ops, gen);
		backprop_update(sim_node, result);
	}

}
