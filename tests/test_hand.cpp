#include <gtest/gtest.h> 

#include "hand.hpp" 

TEST(HandTets, EmtpyHandStartsAtZero) {
    Hand hand; 

    EXPECT_EQ(hand.value(), 0);
    EXPECT_FALSE(hand.has_usable_ace());
    EXPECT_FALSE(hand.is_bust());

}

TEST(HandTest, NormalCardsSumCorrectly) {
    Hand hand; 
    hand.add_card(10);
    hand.add_card(7);

    EXPECT_EQ(hand.value(), 17);
    EXPECT_FALSE(hand.has_usable_ace());
    EXPECT_FALSE(hand.is_bust());

}

TEST(HandTest, SingleAceCanCountAsEleven) {
    Hand hand; 
    hand.add_card(1);
    hand.add_card(7);

    EXPECT_EQ(hand.value(), 18);
    EXPECT_TRUE(hand.has_usable_ace());
    EXPECT_FALSE(hand.is_bust());
}

TEST(HandTest, AceFallsBackToOneIfNeeded) {
    Hand hand; 
    hand.add_card(1);
    hand.add_card(10);
    hand.add_card(5);

    EXPECT_EQ(hand.value(), 16);
    EXPECT_FALSE(hand.has_usable_ace());
    EXPECT_FALSE(hand.is_bust());

}

TEST(HandTest, MultipleAcesHandledCorrectly){
    Hand hand; 
    hand.add_card(1);
    hand.add_card(1);
    hand.add_card(9);

    EXPECT_EQ(hand.value(), 21);
    EXPECT_TRUE(hand.has_usable_ace());
    EXPECT_FALSE(hand.is_bust());

}

TEST(HandTest, BustDetectedCorrectly){
    Hand hand;
    hand.add_card(10);
    hand.add_card(10);
    hand.add_card(5);

    EXPECT_EQ(hand.value(), 25);
    EXPECT_FALSE(hand.has_usable_ace());
    EXPECT_TRUE(hand.is_bust());

}
