#include "q_agent.hpp"

#include <algorithm> 

//Creates agent and stores Q-learning hpyer param
QAgent::QAgent(double alpha, double gamma, double epsilon, unsigned int seed) : alpha_(alpha), gamma_(gamma), epsilon_(epsilon), rng_(seed) {}

//.Ensure unseen state starts with Q-val of zero

void QAgent::ensure_state_exists(const State& state) {
    if (!q_table_.contains(state)) {
        q_table_[state] = {0.0, 0.0};
    }

}

// Choose action with epsilon-greedy policy
// W/ prob epsilon choose rand action O.W. chose action with larger Q-Value

int QAgent::choose_action(const State& state, bool explore) {
    ensure_state_exists(state);
    // Explore = true when training
    // Explore = false during evaluation
    if (explore) { 

        std::uniform_real_distribution<double> prob_dist(0.0, 1.0);
        const double draw = prob_dist(rng_);

        if (draw < epsilon_) {
            std::uniform_int_distribution<int> action_dist(0,1);
            return action_dist(rng_);

        }
    }

    return greedy_action(state);
}

void QAgent::update(const State& state, int action, double reward, const State& next_state, bool done) {

    ensure_state_exists(state);
    ensure_state_exists(next_state);

    const double current_q = q_table_[state][action];

    // Terminal transition there is no future value term

    double target = reward; 

    if(!done) {
        const auto& next_qs = q_table_[next_state];
        const double best_next_q = std::max(next_qs[0], next_qs[1]);

        target += gamma_ * best_next_q;

    }

    // Bellman update
    q_table_[state][action] = current_q + alpha_ * (target - current_q);


}

// Return current Q-values for state
std::array<double, 2> QAgent::get_q_values(const State& state) const {
    const auto it = q_table_.find(state);

    if (it == q_table_.end()) {
        return {0.0, 0.0};
    }
    return it->second;
}

// Return greedy action
// Stand if equal, otherwise whichever action has higher Q-value
int QAgent::greedy_action(const State& state) const {
    const auto q_values = get_q_values(state);

    // Tie break toward stand for determinism
    if (q_values[0] >= q_values[1]) {
        return 0;
    }
    return 1;
}