//
// Created by Theophile Champion on 30/07/2021.
//

#ifndef BTAI_GRAPH_ENV_H
#define BTAI_GRAPH_ENV_H

#include "Environment.h"

namespace btai::environments {

    class GraphEnv : public Environment {
    public:
        /**
         * The two type of observation that can be made by the agent.
         */
        enum ObsType {
            GOOD = 0,
            BAD  = 1
        };

    public:
        /**
         * Create a new graph environment.
         * @param nGood the number of good paths
         * @param nBad the number of bad paths
         * @param sizeGoodPaths the size of each good path
         * @return the created graph environment
         */
        static std::shared_ptr<GraphEnv> create(int nGood, int nBad, const std::vector<int> &sizeGoodPaths);

        /**
         * Construct the graph environment.
         * @param nGood the number of good path after the initial state
         * @param nBad the number of bad path after the initial state
         * @param sizeGoodPaths the size of each good paths
         */
        GraphEnv(int nGood, int nBad, std::vector<int> sizeGoodPaths);

        /**
         * Reset the environment to its initial state.
         * @return the initial observation
         */
        torch::Tensor reset() override;

        /**
         * Execute an action in the environment.
         * @param action the action to be executed
         * @return the observation made after executing the action
         */
        torch::Tensor execute(int action) override;

        /**
         * Display the environment.
         */
        void print() const override;

        /**
         * Getter.
         * @return the number of actions available to the agent
         */
        [[nodiscard]] int actions() const override;

        /**
         * Getter.
         * @return the number of states in the environment
         */
        [[nodiscard]] int states() const override;

        /**
         * Getter.
         * @return the number of observations in the environment
         */
        [[nodiscard]] int observations() const override;

        /**
         * Getter.
         * @return the true likelihood mapping
         */
        torch::Tensor A() override;

        /**
         * Getter.
         * @return the true transition mapping
         */
        torch::Tensor B() override;

        /**
         * Getter.
         * @param prefPrecision the precision over the prior preferences.
         * @return the prior preferences over observations
         */
        [[nodiscard]] torch::Tensor C(double prefPrecision) const override;

        /**
         * Getter.
         * @return the true initial hidden states
         */
        torch::Tensor D() override;

        /**
         * Make the function "print" verbose.
         */
        void verbose();

        /**
         * Getter.
         * @return the index of the goal
         */
        [[nodiscard]] int goalState() const;

        /**
         * Getter.
         * @return the state in which the agent is located
         */
        [[nodiscard]] int agentState() const;

        /**
         * Getter.
         * @return true if the agent solved the environment, false otherwise.
         */
        bool solved() override;

        /**
         * Getter.
         * @return the type of environment.
         */
        [[nodiscard]] EnvType type() const override;

    private:
        /**
         * Getter.
         * @return a vector of string containing the paths' states.
         */
        [[nodiscard]] std::vector<std::string> getPathsStates() const;

        /**
         * Getter.
         * @param paths_states the paths' states.
         * @return a vector of string containing the paths' name.
         */
        [[nodiscard]] std::vector<std::string> getPathsName(std::vector<std::string> &paths_states) const;

        /**
         * Display a detailed description of the environment dynamic.
         */
        void printHelp() const;

        /**
         * Simulate the execution of an action in a state.
         * @param action to simulate.
         * @param agent_state from which the action is taken.
         * @return the state reached.
         */
        [[nodiscard]] int execute(int action, int agent_state) const;

        /**
         * Initialise the good paths' states.
         * @param sizeGoodPaths the size of each good path.
         * @return a vector of states for each good path.
         */
        static std::vector<std::vector<int>> initialisePaths(const std::vector<int> &sizeGoodPaths);

    private:
        int agent_state;
        int n_states;
        int n_good;
        int n_bad;
        int breadth;
        std::vector<std::vector<int>> paths;
        bool _verbose;
        int longest_path_size;
    };

}

#endif //BTAI_GRAPH_ENV_H
