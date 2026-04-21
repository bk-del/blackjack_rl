#include <gtest/gtest.h> 

#include "blackjack_env.hpp"

TEST(BlackjackEnvTest, ResetProducesInitialState) {
    BlackjackEnv env(123);

    const auto [player_total, dealer_upcard, usable_ace] = env.reset();

    // Initial player total must be at least 2 and at most 21
    EXPECT_GE(player_total, 2);
    EXPECT_LE(player_total,21);

    // Dealer upcard must be valid Blackjack card value
    EXPECT_GE(dealer_upcard, 1);
    EXPECT_LE(dealer_upcard, 10);

    // Usable ace can be true or false
    EXPECT_TRUE(usable_ace == true || usable_ace == false);

    EXPECT_FALSE(env.is_done());

}

TEST(BlackjackEnvTest, HitReturnsAValidStepResult) {
    BlackjackEnv env(123);
    env.reset();

    const auto result = env.step(true);

    EXPECT_TRUE(result.reward == -1.0 || result.reward == 0);
}

TEST(BlackjackEnvTest, StandAlwaysEndsTheEpisode) {
    BlackjackEnv env(123);
    env.reset();

    const auto result = env.step(false);

    EXPECT_TRUE(result.done);
    EXPECT_TRUE(env.is_done());
    EXPECT_TRUE(result.reward == -1.0 || result.reward == 0 || result.reward == 1.0);

}

TEST(BlackjackEnvTest, RepeatedHitsEventuallyEndEpisode) {
    BlackjackEnv env(123);
    env.reset();

    bool done = false;
    int safety_counter = 0;
    while (!done && safety_counter < 20) {
        const auto result = env.step(true);
        done = result.done;
        ++safety_counter;
    }

    EXPECT_TRUE(done);
    EXPECT_TRUE(env.is_done());
}