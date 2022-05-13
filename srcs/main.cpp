//
// Created by Theophile Champion on 01/08/2021.
//

#include <cstdlib>
#include <iostream>
#include "algorithms/MCTS.h"
#include "algorithms/configurations/ConfigMCTS.h"
#include "environments/GraphEnv.h"
#include "environments/EnvType.h"
#include "environments/MazeEnv.h"
#include "environments/DisentangleSpritesEnv.h"
#include "experiments/GraphPerformanceTracker.h"
#include "experiments/MazePerformanceTracker.h"
#include "experiments/SpritesPerformanceTracker.h"
#include "experiments/TimeTracker.h"
#include "zoo/BTAI.h"

using namespace btai::environments;
using namespace btai::graphs::data;
using namespace btai::graphs;
using namespace btai::algorithms;
using namespace btai::algorithms::configurations;
using namespace btai::experiments;
using namespace btai::zoo;

std::vector<std::pair<int, int>> getLocalMinima(const std::string &key) {
    static std::map<std::string, std::vector<std::pair<int, int>>> map = {
            {"1.maze", {{3,4}}},
            {"5.maze", {{3,3}}},
            {"7.maze", {{8,3}, {4,7}, {1,3}, {4,1}, {6,3}, {4,5}}},
            {"8.maze", {{3,7}, {7,7}}},
            {"9.maze", {{5,3},{3,5}}},
            {"14.maze", {{3,4}, {7,4}}}
    };
    return map[key];
}

int main() {
    // Demo hyper-parameters.
    int NB_SIMULATIONS = 100;
    int NB_ACTION_PERCEPTION_CYCLES = 20;
    std::string OUTPUT_FILE_NAME = "../results/btai_bf_results_d_sprites.txt";

    // Maze environment hyper-parameters.
    EnvType envType = EnvType::D_SPRITES;

    // Maze environment hyper-parameters.
    std::string MAZES_PATH = "../mazes/";
    std::string MAZE_FILE_NAME = "1.maze";
    std::string FULL_MAZE_FILE_NAME = MAZES_PATH + MAZE_FILE_NAME;
    std::vector<std::pair<int,int>> LOCAL_MINIMA = getLocalMinima(MAZE_FILE_NAME);

    // Graph environment hyper-parameters.
    int NB_GOOD_PATHS = 3;
    int NB_BAD_PATHS = 5;
    std::vector<int> GOOD_PATHS_SIZES = {6,5,8};

    // Disentanglement sprites environment hyper-parameters.
    std::string D_SPRITES_PATH = "../d_sprites/";
    int GRANULARITY = 2;
    int REPEAT = 8;

    // MCTS hyper-parameters.
    int    NB_PLANNING_STEPS = 50;
    double EXPLORATION_CONSTANT = 2.4;
    double PRECISION_PRIOR_PREFERENCES = 1;
    double PRECISION_ACTION_SELECTION = 100;

    // Open the file in which the result should be written.
    std::ofstream file;
    file.open(OUTPUT_FILE_NAME, std::ios_base::app);
    file << "========== EXPERIMENT CONFIGURATION ==========" << std::endl;
    file << "NB_SIMULATIONS: " << NB_SIMULATIONS << std::endl;
    file << "NB_ACTION_PERCEPTION_CYCLES: " << NB_ACTION_PERCEPTION_CYCLES << std::endl;
    if (envType == EnvType::GRAPH) {
        file << "NB_GOOD_PATHS: " << NB_GOOD_PATHS << std::endl;
        file << "NB_BAD_PATHS: " << NB_BAD_PATHS << std::endl;
        file << "GOOD_PATHS_SIZES: " << GOOD_PATHS_SIZES << std::endl;
    } else if (envType == EnvType::MAZE) {
        file << "MAZE_FILE_NAME: " << MAZE_FILE_NAME << std::endl;
        file << "LOCAL_MINIMA: " << LOCAL_MINIMA << std::endl;
    } else {
        file << "D_SPRITES_PATH: " << D_SPRITES_PATH << std::endl;
        file << "GRANULARITY: " << GRANULARITY << std::endl;
        file << "REPEAT: " << REPEAT << std::endl;
    }
    file << "NB_PLANNING_STEPS: " << NB_PLANNING_STEPS << std::endl;
    file << "EXPLORATION_CONSTANT: " << EXPLORATION_CONSTANT << std::endl;
    file << "PRECISION_PRIOR_PREFERENCES: " << PRECISION_PRIOR_PREFERENCES << std::endl;
    file << "PRECISION_ACTION_SELECTION: " << PRECISION_ACTION_SELECTION << std::endl << std::endl;

    // Create the graph environment.
    std::shared_ptr<Environment> env;
    if (envType == EnvType::GRAPH)
        env = GraphEnv::create(NB_GOOD_PATHS, NB_BAD_PATHS, GOOD_PATHS_SIZES);
    else if (envType == EnvType::MAZE)
        env = MazeEnv::create(FULL_MAZE_FILE_NAME);
    else
        env = DisentangleSpritesEnv::create(D_SPRITES_PATH, GRANULARITY, REPEAT);

    // Create configuration of the MCTS algorithm.
    auto conf = ConfigMCTS::create(
        NB_PLANNING_STEPS, EXPLORATION_CONSTANT, PRECISION_PRIOR_PREFERENCES, PRECISION_ACTION_SELECTION
    );

    // Create time and performance trackers.
    std::unique_ptr<PerformanceTracker> perf_tracker;
    if (envType == EnvType::GRAPH)
        perf_tracker = GraphPerformanceTracker::create();
    else if (envType == EnvType::MAZE)
        perf_tracker = MazePerformanceTracker::create(LOCAL_MINIMA);
    else
        perf_tracker = SpritesPerformanceTracker::create();
    auto time_tracker = TimeTracker::create(NB_SIMULATIONS);

    // Initialise performance tracker.
    perf_tracker->reset();

    // Run the episodes.
    for (int j = 0; j < NB_SIMULATIONS; ++j) {

        // Reset environment and create agent.
        auto obs = env->reset();
        auto agent = BTAI::create(env, conf, obs);

        // Run one episode.
        // env->print(); //TODO
        time_tracker->tic();
        for (int k = 0; k < NB_ACTION_PERCEPTION_CYCLES; ++k) {
            agent->step(env);
            // env->print(); //TODO
            if (env->solved()) {
                break;
            }
        }
        time_tracker->toc();

        // Evaluate performance of the simulation.
        perf_tracker->track(env);
    }

    // Print trackers results
    time_tracker->print(file);
    perf_tracker->print(file);

    return EXIT_SUCCESS;
}
