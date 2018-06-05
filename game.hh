#pragma once

#include <functional>
#include <set> 

namespace game {
// Interface for a generic game

// Current game state for the agent
enum EndCondition { ONGOING, WIN, LOSS, DRAW };
template<class State>
struct Step {
   EndCondition end_condition = ONGOING; 
   State new_state;
};
template <class State, class Action>
using StepGame = std::function<Step<State>(const State &, const Action &)>;

template <class State, class Action>
using AvailableActions = std::function<std::set<Action>(const State &)>;

template <class State, class Action>
struct GameOps {
	const StepGame<State, Action> step_game;
    const AvailableActions<State, Action> available_actions;
};

}  // namespace game
