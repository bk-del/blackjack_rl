# Blackjack RL

This is a small C++ project for learning reinforcement learning through a stripped-down Blackjack environment. The code is simple on purpose: a hand model, an environment, a tabular Q-learning agent, and a test suite around the core rules.

## What it does

- Models a single-hand Blackjack game with `hit` and `stand` actions.
- Tracks hand totals, usable aces, and busts.
- Exposes the RL state as `(player total, dealer upcard, usable ace)`.
- Trains a tabular Q-learning agent with epsilon-greedy exploration.
- Evaluates the learned greedy policy after training and prints a small policy snapshot.

## Game assumptions

This is not full casino Blackjack. The environment uses a simplified version that is common in RL examples:

- The deck is treated as infinite.
- Cards are drawn uniformly from 1 to 13, with face cards mapped to `10`.
- The player can only `hit` or `stand`.
- The dealer plays after the player stands and hits until reaching at least 17.
- Rewards are `+1` for a win, `-1` for a loss, and `0` for a draw.
- There is no doubling, splitting, insurance, surrender, or blackjack bonus payout.

## Project layout

- [src/hand.hpp](/home/user/dev/rl/black_jack_rl/src/hand.hpp) and [src/hand.cpp](/home/user/dev/rl/black_jack_rl/src/hand.cpp): hand value logic, usable ace handling, bust detection.
- [src/blackjack_env.hpp](/home/user/dev/rl/black_jack_rl/src/blackjack_env.hpp) and [src/blackjack_env.cpp](/home/user/dev/rl/black_jack_rl/src/blackjack_env.cpp): the Blackjack environment and transition rules.
- [src/q_agent.hpp](/home/user/dev/rl/black_jack_rl/src/q_agent.hpp) and [src/q_agent.cpp](/home/user/dev/rl/black_jack_rl/src/q_agent.cpp): tabular Q-learning agent.
- [src/main.cpp](/home/user/dev/rl/black_jack_rl/src/main.cpp): training loop and policy evaluation.
- [tests](/home/user/dev/rl/black_jack_rl/tests): GoogleTest coverage for the hand logic, environment, and agent updates.

## Build

You need:

- a C++20 compiler
- CMake 3.20 or newer
- `vcpkg`

This repo uses a `vcpkg.json` manifest for GoogleTest. A normal configure/build looks like this:

```bash
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE="$HOME/vcpkg/scripts/buildsystems/vcpkg.cmake"
cmake --build build
```

That builds:

- `blackjack_app`
- `blackjack_tests`

## Run

```bash
./build/blackjack_app
```

The app trains for `500000` episodes, prints the average reward every `5000` episodes, then runs a greedy evaluation pass over `50000` episodes. At the end it prints win/loss/draw counts, average reward, and a few sample state values from the learned table.

## Test

```bash
ctest --test-dir build --output-on-failure
```

At the moment the suite covers:

- hand scoring and ace behavior
- environment reset and step behavior
- terminal and non-terminal Q-value updates
- greedy action selection
