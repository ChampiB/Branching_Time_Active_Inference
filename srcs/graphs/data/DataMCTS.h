//
// Created by Theophile Champion on 01/08/2021.
//

#ifndef BTAI_DATA_MCTS_H
#define BTAI_DATA_MCTS_H

#include <torch/torch.h>

namespace btai::graphs::data {

    class DataMCTS {
    public:
        /**
         * Factory.
         * @param action the node's action.
         * @param cost the node's cost.
         * @param beliefs the node's posterior beliefs.
         * @param visits the node's number of visits.
         * @return the created data.
         */
        static std::unique_ptr<DataMCTS> create(const torch::Tensor &beliefs, int action = -1, double cost = 0, int visits = 1);

        /**
         * Constructor.
         * @param action the node's action.
         * @param cost the node's cost.
         * @param beliefs the node's beliefs.
         * @param visits the node's number of visits.
         */
        DataMCTS(const torch::Tensor &beliefs, int action, double cost, int visits);

        /**
         * Increase the number of visits of the node.
         */
        void increaseVisitCount();

        /**
         * Add cost to node.
         * @param cost the additional cost.
         */
        void addCost(double cost);

        /**
         * Setter.
         * @param beliefs new node's beliefs.
         */
        void setBeliefs(const torch::Tensor &beliefs);

        /**
         * Getter.
         * @return the node's number of visits.
         */
        [[nodiscard]] int visits() const;

        /**
         * Getter.
         * @return the node's action.
         */
        [[nodiscard]] int action() const;

        /**
         * Getter.
         * @return the node's cost.
         */
        [[nodiscard]] double cost() const;

        /**
         * Getter.
         * @return the node's posterior beliefs.
         */
        [[nodiscard]] torch::Tensor beliefs() const;

        /**
         * Getter.
         * @param attributeName the name of the attribute whose value should be returned.
         * @return the value of the attribute.
         */
        std::string get(const std::string &attributeName);

    private:
        int _visits;
        int _action;
        double _cost;
        torch::Tensor _beliefs;
    };

}

#endif //BTAI_DATA_MCTS_H
