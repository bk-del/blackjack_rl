#pragma once

# include <vector> 

class Hand {
public: 
    void add_card(int card_value); 
    int value() const;
    bool has_usable_ace() const;
    bool is_bust() const;

    const std::vector<int>& cards() const;

private: 
    std::vector<int> cards_;

        
    
};