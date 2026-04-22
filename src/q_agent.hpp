#pragma once

#include <array> 
#include <map>
#include <random> 
#include <tuple> 

#include "blackjack_env.hpp"

/*
Tabular Q-learning agent for simplified Blackjack

Action encoding 
0 = stand
1 = hit
*/

class QAgent {
public: 
    using State = BlackjackEnv::State;

    //Create Q-Learning agent with fixed hyperparam and random number generator seed
    QAgent(double alpha, double gamma, double epsilon, unsigned int seed = 123);

    // Chose action 
    // If explore = true use epsilon greedy
    // If explore = false use greedy action only
    int choose_action(const State& state, bool explore);

    // Update Q-table using obserbed transition
    void update(const State& state, int action, double reward, const State& next_state, bool done);

    // Returns current Q-valeu pair [stand, hit] for given state
    // If state has not been returned before, return zero 
    std::array<double, 2> get_q_values(const State& state) const; 

    // Retruns the greedy action for state with no exploration 
    int greedy_action(const State& state) const;

private:
    // Ensure state exists in table with deafutl Q-values
    void ensure_state_exists(const State& state);

    double alpha_;
    double gamma_;
    double epsilon_;
    std::mt19937 rng_;
    std::map<State, std::array<double,2 >> q_table_;

};