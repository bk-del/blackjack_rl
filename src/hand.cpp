#include "hand.hpp" 

#include <numeric> 

void Hand::add_card(int card_value) {
    cards_.push_back(card_value);
    
}

int Hand::value() const { 
    int total = std::accumulate(cards_.begin(), cards_.end(), 0);

    int ace_count = 0;
    for (int card : cards_) {
        if (card == 1) {
            ++ace_count;

        }
    }

    while (ace_count > 0 && total + 10 <= 21) {
        total+= 10;
        --ace_count;
    }
    return total;
}

bool Hand::has_usable_ace() const {
    int total = std::accumulate(cards_.begin(), cards_.end(), 0);
    for (int card : cards_) {
        if (card == 1 && total +10 <= 21) {
            return true;
        }
    }
    return false;
}

bool Hand::is_bust() const {
    return value() > 21; 
}

const std::vector<int>& Hand::cards() const { 
    return cards_;
}