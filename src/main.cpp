#include <iostream>

#include "blackjack_env.hpp"
#include "q_agent.hpp"

namespace  {

    // Store summary statistics from greedy policy evaluation run 
struct EvalStats {
    int win = 0;
    int losses = 0;
    int draws = 0;
    double average_reward = 0.0;

};

EvalStats evaluate_policy(QAgent &agent, int num_episodes) {
    BlackjackEnv eval_env(999);
    EvalStats stats;
    double reward_sum = 0.0;

    for (int episode = 0; episode < num_episodes; ++episode) {
        auto state = eval_env.reset();
        bool done = false;

        while(!done) {
            // No exploration during evaluation, only greedy action
            const int action = agent.choose_action(state, false);
            const bool hit_action = (action == 1);
            const auto result = eval_env.step(hit_action);

            state = result.next_state;
            done = result.done;

            if (done) {
                reward_sum += result.reward;

                if (result.reward > 0.0) {
                    ++stats.win;
                } else if (result.reward < 0.0) {
                    ++stats.losses;
                } else {
                    ++stats.draws;
                }
            }
        }
    }
    stats.average_reward = reward_sum / static_cast<double>(num_episodes);
    return stats;
};

void print_sample_policy(QAgent& agent) {
    const BlackjackEnv::State s1{20, 10, false};
    const BlackjackEnv::State s2{16, 10, false};
    const BlackjackEnv::State s3{12, 4, false};
    const BlackjackEnv::State s4{18, 9, true};

    const auto q1 = agent.get_q_values(s1);
    const auto q2 = agent.get_q_values(s2);
    const auto q3 = agent.get_q_values(s3);
    const auto q4 = agent.get_q_values(s4);

    std::cout << "\nSample learned policy snapshot:\n";
    std::cout << "State (20, dealer 10, no usable ace): stand=" <<q1[0]
            << ", hit=" << q1[1] << ", greedy_action=" << agent.greedy_action(s1) << '\n';

    std::cout << "State (16, dealer 10, no usable ace): stand=" <<q2[0]
            << ", hit=" << q2[1] << ", greedy_action=" << agent.greedy_action(s2) << '\n';

    std::cout << "State (12, dealer 4, no usable ace): stand=" <<q3[0]
            << ", hit=" << q3[1] << ", greedy_action=" << agent.greedy_action(s3) << '\n';

    std::cout << "State (18, dealer 9, usable ace): stand=" <<q4[0]
            << ", hit=" << q4[1] << ", greedy_action=" << agent.greedy_action(s4) << '\n';
}
} //namespace
int main() {
    // Hyperprams for Q-learning agent
    const int num_training_episodes = 500000;
    const int num_eval_episodes = 50000;
    const double alpha = 0.1; 
    const double gamma = 1.0;
    const double epsilon = 0.1;

    BlackjackEnv env(42);
    QAgent agent(alpha, gamma, epsilon, 123);

    double running_reward_sum = 0.0;

    for (int episode = 1; episode <=num_training_episodes; ++episode) {
        auto state = env.reset();
        bool done = false;
        double episode_reward = 0.0;

        while(!done) {
            const int action = agent.choose_action(state, true);
            
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
                      << " | average reward over 5000 episodes = " 
                      << avg_reward << '\n';
            running_reward_sum = 0.0;
        }
    }
    const EvalStats stats = evaluate_policy(agent, num_eval_episodes);
    
    std::cout << "\nGreedy policy evaluation over " << num_eval_episodes << " episodes:\n";
    std::cout << "Wins: " << stats.win << '\n';
    std::cout << "Losses: " << stats.losses << '\n';
    std::cout << "Draws: " << stats.draws << '\n';
    std::cout << "Average reward: " << stats.average_reward << '\n';

    print_sample_policy(agent);


    return 0;
}
