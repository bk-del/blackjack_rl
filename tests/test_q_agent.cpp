#include <gtest/gtest.h>

#include "q_agent.hpp"

TEST(QAgentTest, UnseenStateStartsWithZeroQValues) {
    QAgent agent(0.1, 1.0, 0.1, 123);
    const BlackjackEnv::State state{15, 10, false};

    const auto q_values = agent.get_q_values(state);

    EXPECT_DOUBLE_EQ(q_values[0], 0.0);
    EXPECT_DOUBLE_EQ(q_values[1], 0.0);

}

TEST(QAgentTest, TerminalUpdateMovesQValueTowardReward) {
    QAgent agent(0.5, 1.0, 0.0, 123);
    const BlackjackEnv::State state{15,10,false};
    const BlackjackEnv::State next_state{20,10,false};

    //Update action with terminal win 
    agent.update(state, 0, 1.0, next_state, true);

    const auto q_values = agent.get_q_values(state);

    // 0 + 0.5 * (1 - 0) = 0.5
    EXPECT_DOUBLE_EQ(q_values[0], 0.5);
    EXPECT_DOUBLE_EQ(q_values[1], 0.0);


}

TEST(QAgentTest, NonTerminalUpdateUsesBestNextQValue) {
    QAgent agent(0.5, 1.0, 0.0, 123);

    const BlackjackEnv::State state{13, 6, false};
    const BlackjackEnv::State next_state{18, 6, false};

    //Make next_state have known good value on action 1
    agent.update(next_state, 1, 1.0, next_state, true);
    
    //Update original state using non-terminal transition
    agent.update(state, 1, 0.0, next_state, false);

    const auto q_values = agent.get_q_values(state); 

    // Target = 0 + 1.0 * 0.5 = 0.5
    // New value = 0 + 0.5 * (0.5 - 0) = 0.25
    EXPECT_DOUBLE_EQ(q_values[1], 0.25);

}

TEST(QAgentTest, GreedyActionChoosesHigherQValue) {
    QAgent agent(1.0, 1.0, 0.0, 123);
    const BlackjackEnv::State state{16, 10, false};
    const BlackjackEnv::State dummy_next{20, 10, false};

    // Set stand value to +1 exactly.
    agent.update(state, 0, 1.0, dummy_next, true);

    EXPECT_EQ(agent.greedy_action(state), 0);
}