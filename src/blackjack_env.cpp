#include "blackjack_env.hpp"



// Create env and init the random number generator
BlackjackEnv::BlackjackEnv(unsigned int seed) : rng_(seed), done_(false) {}

// Start new hand
// Clear both hands, deal two player cards and two dealer cards
// Return initial state

BlackjackEnv::State BlackjackEnv::reset() {
    player_hand_ = Hand{};
    dealer_hand_ = Hand{};
    done_ = false;

    // Deal init cards
    player_hand_.add_card(draw_card());
    dealer_hand_.add_card(draw_card());

    player_hand_.add_card(draw_card());
    dealer_hand_.add_card(draw_card());

    return get_state();
    
}
// Apply one player action 
// If action is hit, player draws card
// If player busts, episode ends with reward -1 
// If faction is stand, the dealer finishes player and reward is computed based on who wins
BlackjackEnv::StepResult BlackjackEnv::step(bool hit_action) {
    if (done_) {

        return {get_state(), 0.0, true};
    }

    if (hit_action) {
        player_hand_.add_card(draw_card());

        if (player_hand_.is_bust()) {
            done_ = true;
            return {get_state(), -1.0, true};

        }
        // None terminal hit: reward is 0 until hand ends
        return {get_state(), 0.0, false};
        }
        // Standing ends players turn. Dealer now plays out hand
        play_dealer();
        done_ = true; 

        return {get_state(), compute_reward(), true};

    }

// Returns current state to RL agent
BlackjackEnv::State BlackjackEnv::get_state() const {
    // Dealers upcard is frst visible card in dealer hand, or 0 if no cards
    const int dealer_upcard = dealer_hand_.cards().empty() ? 0 : dealer_hand_.cards().front();

    return {player_hand_.value(), dealer_upcard, player_hand_.has_usable_ace()};

}

bool BlackjackEnv::is_done() const { 
    return done_;
}

// Draw a card from an infinit deck model 
// Ace = 1
// 2..9 = face value
// 10, Jack, Queen, King = 10

int BlackjackEnv::draw_card() {
    std::uniform_int_distribution<int> dist(1,13);
    const int raw = dist(rng_);

    // Map face cards to 10
    return raw >= 10 ? 10 : raw;
}

// Dealer policy - keep drawing until total is 17 or more
void BlackjackEnv::play_dealer(){
    while (dealer_hand_.value() < 17) {
        dealer_hand_.add_card(draw_card());
    }
}

double BlackjackEnv::compute_reward() const {
    if (player_hand_.is_bust()) {
        return -1.0;

    }
    if (dealer_hand_.is_bust()) {
        return 1.0;
    }

    const int player_total = player_hand_.value();
    const int dealer_total = dealer_hand_.value();

    if (player_total > dealer_total) {
        return 1.0;
    }

    if (player_total < dealer_total) {
        return -1.0;
    }
    return 0.0; // Draw
}

