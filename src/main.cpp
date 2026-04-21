#include <iostream>

#include "blackjack_env.hpp"
#include "q_agent.hpp"

int main() {
    // Hyperprams for Q-learning agent
    const int num_episodes = 500000;
    const double alpha = 0.1; 
    const double gamma = 1.0;
    const double epsilon = 0.1;

    BlackjackEnv env(42);
    QAgent agent(alpha, gamma, epsilon, 123);

    double running_reward_sum = 0.0;

    for (int episode = 1; episode <=num_episodes; ++episode) {
        auto state = env.reset();
        bool done = false;
        double episode_reward = 0.0;

        while(!done) {
            const int action = agent.choose_action(state);
            
            const bool hit_action = (action == 1);
            const auto result = env.step(hit_action);

            agent.update(state, action, result.reward, result.next_state, result.done);

            state = result.next_state; 
            done = result.done;
            episode_reward += result.reward;

        }
        running_reward_sum += episode_reward;

        // Print progress every 5000 episodes
        if (episode % 5000 == 0) {
            const double avg_reward = running_reward_sum / 5000.0;
            std::cout << "Episode " << episode
                      << " | average reward over 5000 episodes = " << avg_reward << '\n';
            running_reward_sum = 0.0;
        }
    }
    // Print few example Q-values 
    const BlackjackEnv::State sample_state_1{20, 10, false};
    const auto q_values_1 = agent.get_q_values(sample_state_1);
    std::cout << "\nLearned Q-values for state (20, dealer 10, no usable ace): "
              << "stand=" << q_values_1[0] << ", hit=" << q_values_1[1] << '\n';

    const BlackjackEnv::State sample_state_2{12, 2, false};
    const auto q_values_2 = agent.get_q_values(sample_state_2);
    std::cout << "Learned Q-values for state (12, dealer 4, no usable ace): "
              << "stand=" << q_values_2[0] << ", hit=" << q_values_2[1] << '\n';

    return 0;
}
