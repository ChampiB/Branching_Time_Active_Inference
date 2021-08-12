//
// Created by Theophile Champion on 02/08/2021.
//

#include "MCTS.h"
#include <torch/torch.h>
#include "wrappers/Torch.h"
#include "graphs/TreeNode.h"
#include "graphs/data/DataMCTS.h"
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
        std::vector<NodeMCTS *> expandedNodes;

        for (int action = 0; action < b.size(2); ++action) {
            auto b_i = squeeze(torch::narrow(b, 2, action, 1));
            auto beliefs = BayesianFiltering::predict(node->data()->beliefs(), b_i);
            auto expandedNode = TreeNode<DataMCTS>::create(DataMCTS::create(beliefs, action));
            node->addChild(expandedNode);
            expandedNode->setParent(node.get());
            expandedNodes.push_back(expandedNode.get());
        }
        return expandedNodes;
    }

    void MCTS::evaluation(const std::vector<NodeMCTS *> &nodes, const torch::Tensor &a, const torch::Tensor &c) {
        for (auto node : nodes) {
            auto sBeliefs = node->data()->beliefs();
            auto oBeliefs = BayesianFiltering::predict(sBeliefs, a);
            node->data()->addCost(efe(sBeliefs, oBeliefs, a, c));
        }
    }

    void MCTS::propagation(const std::vector<NodeMCTS *> &nodes) {
        auto bestChild = *std::min_element(nodes.begin(), nodes.end(), &MCTS::compareCost);
        double cost = bestChild->data()->cost();
        auto current = bestChild->parent();

        while (current != nullptr) {
            current->data()->addCost(cost);
            current->data()->increaseVisitCount();
            current = current->parent();
        }
    }

    bool MCTS::compareCost(const NodeMCTS *n1, const NodeMCTS *n2) {
        return n1->data()->cost() < n2->data()->cost();
    }

    bool MCTS::compareUCT(const std::shared_ptr<NodeMCTS> &n1, const std::shared_ptr<NodeMCTS> &n2) const {
        return uct(n1) < uct(n2);
    }

    double MCTS::uct(const std::shared_ptr<NodeMCTS> &node) const {
        int n = node->parent()->data()->visits();
        int n_i = node->data()->visits();

        return - node->data()->cost() / n_i + _config->explorationConstant() * std::sqrt(std::log(n) / n_i);
    }

    double MCTS::efe(const Tensor &sBeliefs, const Tensor &oBeliefs, const Tensor &a, const Tensor &c) {
        auto risk = torch::matmul(oBeliefs, oBeliefs.log() - c.log()).item<double>();
        auto ambiguity = - torch::matmul(torch::diag(torch::matmul(a.log().t(), a.log())), sBeliefs).item<double>();

        return risk + ambiguity;
    }

    int MCTS::selectAction() const {
        int nbActions = _root->nChildren();
        Tensor w = Torch::empty({nbActions});

        for (int i = 0; i < nbActions; ++i) {
            auto child = _root->child(i);
            int action = child->data()->action();
            w[action] = - _config->actionPrecision() * child->data()->cost() / child->data()->visits();
        }
        w = softmax(w, 0);
#ifndef NDEBUG
        std::cout << "Probability of actions: " << std::endl;
        std::cout << w << std::endl;
#endif
        return Torch::randomInt(w);
    }

    void MCTS::setRoot(const std::shared_ptr<TreeNode<DataMCTS>> &root) {
        _root = root;
    }

    std::shared_ptr<ConfigMCTS> MCTS::config() const {
        return _config;
    }

    std::shared_ptr<TreeNode<DataMCTS>> MCTS::root() {
        return _root;
    }

}
