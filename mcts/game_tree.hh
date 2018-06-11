#pragma once

#include <cmath>
#include <iterator>
#include <memory>
#include <map>
#include <numeric>
#include <random>

#include <iostream>

#include "game/game.hh"

namespace mcts {
template <class State, class Action>
struct Node {
	Node *parent = nullptr;
	const State state;
	std::map<Action, std::unique_ptr<Node>> next_states;
	int games_played = 0;
	int games_won = 0;
	
	Node(const State &state) : parent{nullptr}, state(state) {};
	Node(Node<State, Action> *parent, const State &state)
        : parent{parent}, state(state) {};
	Node<State, Action> *insert_node(
        const Action &action, const State &new_state) {
		next_states[action] =
            std::make_unique<Node<State, Action>>(this, new_state);
		return next_states[action].get();
    }
};

// Finds the best node that hasn't yet been fully expanded
template <class S, class A> 
Node<S, A> *select_node(Node<S, A> *node) {
	using NextStatePair = std::pair<const A, std::unique_ptr<Node<S, A>>>;
    // If this node has no next states, it definitely hasn't been expanded
	if (node->next_states.size() == 0) {
		return node;	
    }

	// If there is a node that hasn't been explored, return this node
	bool is_not_fully_explored = std::any_of(
		node->next_states.begin(), node->next_states.end(),
			[](NextStatePair &pair) -> bool {
		return pair.second->games_played == 0;
	});
	if (is_not_fully_explored) {
		return node;
	}

    // This node has been fully explored, so choose the best child
	const int games_played = std::accumulate(
		node->next_states.begin(),
		node->next_states.end(),
		0,
		[](int &a, NextStatePair &pair) -> int {
			return a + pair.second->games_played;
		});

	// Select the node with the greatest potential reward
	const auto iter = std::max_element(
		node->next_states.begin(), node->next_states.end(),
		[games_played] (NextStatePair &a,
                        NextStatePair &b) -> bool {
			const double a_val = 
				static_cast<double>(a.second->games_won) / a.second->games_played +
					std::sqrt(2 * std::log(games_played) / a.second->games_played);
			const double b_val = 
				static_cast<double>(b.second->games_won) / b.second->games_played +
					std::sqrt(2 * std::log(games_played) / b.second->games_played);
			return a_val < b_val;
		});
	return select_node(iter->second.get());
}

// Returns a node which should be used to seed a simulation
template <class S, class A>
Node<S, A> *expand_node(Node<S, A> *node,
			      const game::GameOps<S, A> &ops) {
	using NextStatePair = std::pair<const A, std::unique_ptr<Node<S, A>>>;
	// If there are no sub nodes, populate all possible moves
	if (node->next_states.size() == 0) {
		for (const auto &action : ops.available_actions(node->state)) {
			const game::Step step = ops.step_game(node->state, action);
			node->insert_node(action, step.new_state);
		}
	}

	// Select a node to start a simulation from
	const auto iter = std::find_if(
		node->next_states.begin(),
		node->next_states.end(),
		[](const NextStatePair &pair) -> bool {
		return pair.second->games_played == 0;
	});
	return iter->second.get();
}

template <class S, class A>
game::EndCondition simulate_playout(Node<S, A> *node,
                                    const game::GameOps<S, A> &ops,
									std::mt19937 &gen) {
	S state = node->state;
	while (true) {
		// Get a list of actions we could take
		const std::set<A> actions = ops.available_actions(state);
		// Select a random action
		std::uniform_int_distribution<> dis(0, actions.size()-1);
		int index = dis(gen);
		auto iter = actions.begin();		
		std::advance(iter, index);
		const A &action = *iter;
		// Step the game forward
		const game::Step<S> step = ops.step_game(state, action);
		// If the game is over, return the end condition
		if (step.end_condition != game::ONGOING) {
			return step.end_condition;
		}
		state = step.new_state;
	}
}

template <class S, class A>
void backprop_update(Node<S, A> *leaf, const game::EndCondition &end_condition) {
	Node<S, A> *node = leaf;
	while (node != nullptr) {
		node->games_played++;
		if (end_condition == game::WIN) {
			node->games_won++;
		}
		node = node->parent;
	}
}

//template <class S, class A>
//void mcts_iteration(Node<S, A> *root, const game::GameOps<S, A> &ops) {
//	// Select a node to expand
//	Node<S, A> *unexpanded_node = select_node(root);
//
//    // From this node, pick an unexplored option
//	Node<S, A> *node_start = expand_node(unexpanded_node, ops);
//
//    // Simulate playout from this state
//	const game::EndCondition result = simulate_playout(node_start, ops);
//
//    // Backprop the result
//	backprop_update(node_start, result);
//}

}  // namespace mcts
