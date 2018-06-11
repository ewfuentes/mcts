#include <iostream>

#include "line_game.hh"
#include "game_tree.hh"

void print_state(const game::LineState &state) {
	std::cout << "Position: " << state.position << std::endl;
}

game::LineAction select_action(const game::LineState &state) {
	std::cout << "Select action: ";
	const auto actions = game::available_actions(state);
	std::vector<game::LineAction> actions_vec{};
	auto iter = actions.begin();
	for (int i = 0; i < static_cast<int>(actions.size()); i++) {
        std::cout << " " << i << ". " << iter->step;
		actions_vec.push_back(*iter);
		iter++;
    }
	std::cout << std::endl;
	int a = 0;
	std::cin >> a;
	return actions_vec[a];
}

void print_game_tree(const mcts::Node<game::LineState, game::LineAction> *const root,
                     const int depth) {
	std::cout << "State: " << root->state.position << std::endl;
	for (const auto &pair : root->next_states) {
		for (int i = 0; i < depth + 1; i++) {
			std::cout << " ";
        }
		std::cout << "Action: " << pair.first.step << " ";
        print_game_tree(pair.second.get(), depth + 1);
    }
}

int main() {
    game::LineState state;
	mcts::Node root = mcts::Node<game::LineState, game::LineAction>{state};
	auto *node = &root;
    bool run = true;
	while (run) {
		print_state(state);
		const auto action = select_action(state);
		const auto step = step_game(state, action);
		node = node->insert_node(action, step.new_state);
		run = step.end_condition == game::ONGOING;
		state = step.new_state;
    }
	std::cout << "Game ended!" << std::endl;
	print_game_tree(&root, 0);
}
