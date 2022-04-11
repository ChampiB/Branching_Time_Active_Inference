//
// Created by Theophile Champion on 02/08/2021.
//

#include "MCTS.h"
#include <torch/torch.h>
#include "wrappers/Torch.h"
#include "graphs/TreeNode.h"
#include "algorithms/configurations/ConfigMCTS.h"
#include "algorithms/BayesianFiltering.h"

using namespace torch;
using namespace btai::graphs;
using namespace btai::graphs::data;
using namespace btai::algorithms::configurations;
using namespace btai::wrappers;

namespace btai::algorithms {

    std::unique_ptr<MCTS> MCTS::create(const std::shared_ptr<NodeMCTS> &root, const std::shared_ptr<ConfigMCTS> &config) {
        return std::make_unique<MCTS>(root, config);
    }

    MCTS::MCTS(const std::shared_ptr<NodeMCTS> &root, const std::shared_ptr<ConfigMCTS> &config) {
        _root = root;
        _config = config;
    }

    std::shared_ptr<NodeMCTS> MCTS::selectNode(int nbActions) const {
        auto compareUCT = [this](const std::shared_ptr<NodeMCTS> &n1, const std::shared_ptr<NodeMCTS> &n2) {
            return this->compareUCT(n1, n2);
        };
        std::shared_ptr<NodeMCTS> curr = _root;

        while (curr->nChildren() == nbActions) {
            curr = *std::max_element(curr->childrenBegin(), curr->childrenEnd(), compareUCT);
        }
        return curr;
    }

    std::vector<NodeMCTS *> MCTS::expansion(const std::shared_ptr<NodeMCTS> &node, const torch::Tensor &b) {
        // Initialise the transition matrices, if this is the first time the function is called.
        static std::vector<Tensor> b_mat;
        if (b_mat.empty()) {
            for (int action = 0; action < b.size(2); ++action)
                b_mat.emplace_back(squeeze(narrow(b, 2, action, 1)));
        }

        // Create the expanded nodes.
        std::vector<NodeMCTS *> expandedNodes;
        for (int action = 0; action < b.size(2); ++action) {
            auto beliefs = BayesianFiltering::predict(node->beliefs(), b_mat[action]);
            auto expandedNode = TreeNode::create(beliefs, action);
            node->addChild(expandedNode);
            expandedNode->setParent(node.get());
            expandedNodes.emplace_back(expandedNode.get());
        }
        return expandedNodes;
    }

    void MCTS::evaluation(const std::vector<NodeMCTS *> &nodes, const torch::Tensor &a, const torch::Tensor &c) {
        for (auto node : nodes) {
            auto sBeliefs = node->beliefs();
            auto oBeliefs = BayesianFiltering::predict(sBeliefs, a);
            node->addCost(efe(sBeliefs, oBeliefs, a, c));
        }
    }

    void MCTS::propagation(const std::vector<NodeMCTS *> &nodes) {
        auto bestChild = *std::min_element(nodes.begin(), nodes.end(), &MCTS::compareCost);
        double cost = bestChild->cost();
        auto current = bestChild->parent();

        while (current != nullptr) {
            current->addCost(cost);
            current->increaseVisitCount();
            current = current->parent();
        }
    }

    bool MCTS::compareCost(const NodeMCTS *n1, const NodeMCTS *n2) {
        return n1->cost() < n2->cost();
    }

    bool MCTS::compareUCT(const std::shared_ptr<NodeMCTS> &n1, const std::shared_ptr<NodeMCTS> &n2) const {
        return uct(n1) < uct(n2);
    }

    double MCTS::uct(const std::shared_ptr<NodeMCTS> &node) const {
        int n = node->parent()->visits();
        int n_i = node->visits();

        return - node->cost() / n_i + _config->explorationConstant() * std::sqrt(std::log(n) / n_i);
    }

    double MCTS::efe(const Tensor &sBeliefs, const Tensor &oBeliefs, const Tensor &a, const Tensor &c) {
        auto risk = matmul(oBeliefs, (oBeliefs / c).log()).item<double>();
        auto ambiguity = - matmul(diag(matmul(a.log().t(), a.log())), sBeliefs).item<double>();

        return risk + ambiguity;
    }

    int MCTS::selectAction() const {
        int nbActions = _root->nChildren();
        Tensor w = Torch::empty({nbActions});

        for (int i = 0; i < nbActions; ++i) {
            auto child = _root->child(i);
            w[child->action()] = - _config->actionPrecision() * child->cost() / child->visits();
        }
        return Torch::randomInt(softmax(w, 0));
    }

    void MCTS::setRoot(const std::shared_ptr<TreeNode> &root) {
        _root = root;
    }

    std::shared_ptr<ConfigMCTS> MCTS::config() const {
        return _config;
    }

    std::shared_ptr<TreeNode> MCTS::root() {
        return _root;
    }

}
