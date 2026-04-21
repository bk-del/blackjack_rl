#pragma once

#include <random> 
#include <tuple> 

#include "hand.hpp"

/*
Environment for one hand blackjack
Player can hit or stand and the environment returns the next observation, reward, or if the episode is done
*/

class BlackjackEnv {
public: 
    // Player total, dealer upcard, usable ace
    using State = std::tuple<int, int, bool>;

    struct StepResult {
        State next_state;
        double reward;
        bool done;
    };
    // Create environment with determinstic seed
    explicit BlackjackEnv(unsigned int seed = 42);

    // New hand and initial state
    State reset();

    // Take action: hit = true, stand = false
    StepResult step(bool hit_action);

    State get_state() const; 

    bool is_done() const;

private:
    int draw_card();

    // Dealer plays when hit below 17 and stands otherwise
    void play_dealer();

    // Compare hands and return reward: +1 for win, -1 for loss, 0 for draw
    double compute_reward() const;

        // 32-bit psuedo-random number generator from <random>
    std::mt19937 rng_;
    Hand player_hand_;
    Hand dealer_hand_;
    bool done_;
};


