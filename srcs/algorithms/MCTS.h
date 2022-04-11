//
// Created by Theophile Champion on 02/08/2021.
//

#ifndef BTAI_MCTS_H
#define BTAI_MCTS_H

#include <memory>
#include <torch/torch.h>

namespace btai::algorithms::configurations {
    class ConfigMCTS;
}
namespace btai::graphs {
    class TreeNode;
}
namespace btai::graphs::data {
    class DataMCTS;
}
using NodeMCTS = btai::graphs::TreeNode;

namespace btai::algorithms {

    class MCTS {
    public:
        /**
         * Create a Monte Carlo Tree Search algorithm.
         * @param root the root of the tree on which MCTS is run.
         * @param config the configuration of the MCTS algorithm.
         * @return the MCTS algorithm.
         */
        static std::unique_ptr<MCTS> create(
            const std::shared_ptr<NodeMCTS> &root,
            const std::shared_ptr<configurations::ConfigMCTS> &config
        );

        /**
         * Constructor.
         * @param root the root of the tree on which MCTS is run.
         * @param config the configuration of the MCTS algorithm.
         */
        explicit MCTS(
            const std::shared_ptr<NodeMCTS> &root,
            const std::shared_ptr<configurations::ConfigMCTS> &config
        );

        /**
         * Select the node to be expanded.
         * @param nbActions the number of actions in the environment.
         * @return the selected node.
         */
        [[nodiscard]] std::shared_ptr<NodeMCTS> selectNode(int nbActions) const;

        /**
         * Perform an expansion of the selected node.
         * @param node the node selected for expansion.
         * @param b the transition mapping.
         * @return the list of newly expanded nodes.
         */
        static std::vector<NodeMCTS*> expansion(const std::shared_ptr<NodeMCTS> &node, const torch::Tensor &b) ;

        /**
         * Evaluate the cost of all expanded nodes.
         * @param nodes the newly expanded nodes.
         * @param a the likelihood mapping.
         * @param b the transition mapping.
         */
        static void evaluation(const std::vector<NodeMCTS*> &nodes, const torch::Tensor &a, const torch::Tensor &b) ;

        /**
         * Propagate the cost of the newly expanded nodes and update the number of visits.
         * @param nodes the newly expanded nodes.
         */
        static void propagation(const std::vector<NodeMCTS*> &nodes) ;

        /**
         * Select the action to be performed.
         * @return the selected action.
         */
        [[nodiscard]] int selectAction() const;

        /**
         * Getter.
         * @return the configuration of the MCTS algorithm.
         */
        [[nodiscard]] std::shared_ptr<configurations::ConfigMCTS> config() const;

        /**
         * Setter.
         * @param root the new root.
         */
        void setRoot(const std::shared_ptr<graphs::TreeNode> &root);

        /**
         * Getter.
         * @return the root of the tree on which MCTS is operating.
         */
        std::shared_ptr<graphs::TreeNode> root();

    private:
        /**
         * Compare the cost of the two input nodes.
         * @param n1 first input node.
         * @param n2 second input node.
         * @return true if n1 is less than n2, false otherwise.
         */
        static bool compareCost(const NodeMCTS *n1, const NodeMCTS *n2);

        /**
         * Compare the uct criterion of the two input nodes.
         * @param n1 first input node.
         * @param n2 second input node.
         * @return true if n1 is less than n2, false otherwise.
         */
        [[nodiscard]] bool compareUCT(const std::shared_ptr<NodeMCTS> &n1, const std::shared_ptr<NodeMCTS> &n2) const;

        /**
         * Compute the uct criterion of the input node.
         * @param node the node whose uct criterion must be computed.
         * @return the uct criterion.
         */
        [[nodiscard]] double uct(const std::shared_ptr<NodeMCTS> &node) const;

        /**
         * Compute the expected free energy of a node.
         * @param sBeliefs posterior beliefs over states.
         * @param oBeliefs posterior beliefs over observations.
         * @param a the likelihood mapping.
         * @param c prior preferences over observations.
         * @return the expected free energy.
         */
        static double efe(const torch::Tensor &sBeliefs, const torch::Tensor &oBeliefs, const torch::Tensor &a, const torch::Tensor &c);

    private:
        std::shared_ptr<NodeMCTS> _root;
        std::shared_ptr<configurations::ConfigMCTS> _config;
    };

}

#endif //BTAI_MCTS_H
