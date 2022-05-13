//
// Created by Theophile Champion on 02/08/2021.
//

#include "BTAI.h"
#include "algorithms/configurations/ConfigMCTS.h"
#include "algorithms/MCTS.h"
#include "algorithms/BayesianFiltering.h"
#include "graphs/TreeNode.h"
#include "graphs/GraphViz.h"
#include "environments/Environment.h"
#include "wrappers/Torch.h"

using namespace btai::environments;
using namespace btai::algorithms::configurations;
using namespace btai::algorithms;
using namespace btai::wrappers;
using namespace btai::graphs::data;
using namespace btai::graphs;
using namespace torch;

namespace btai::zoo {

    std::shared_ptr<BTAI> BTAI::create(
            const std::shared_ptr<Environment> &env,
            const std::shared_ptr<ConfigMCTS> &config,
            const Tensor &obs
    ) {
        return std::make_shared<BTAI>(env, config, obs);
    }

    BTAI::BTAI(const std::shared_ptr<Environment> &env, const std::shared_ptr<ConfigMCTS> &config, const Tensor &obs) {
        // Retrieve model's parameters.
        _a = env->A();
        _b = env->B();
        _d = env->D();

        // Compute prior preferences.
        _c = env->C(config->preferencesPrecision());

        // Compute posterior beliefs over initial state
        auto posterior = BayesianFiltering::integrateEvidence(_d, _a, obs);

        // Create the root of the tree.
        auto root = TreeNode::create(posterior);

        // Create the MCTS algorithm.
        _mcts = MCTS::create(root, config);
    }

    void BTAI::step(const std::shared_ptr<Environment> &env) {
        for (int j = 0; j < _mcts->config()->nbPlanningSteps(); ++j) {
            auto selectedNode = _mcts->selectNode(env->actions());
            auto expandedNodes = _mcts->expansion(selectedNode, _b);
            _mcts->evaluation(expandedNodes, _a, _c);
            _mcts->propagation(expandedNodes);
        }
        int action = _mcts->selectAction();
        integrate(action, env->execute(action));
    }

    void BTAI::integrate(int action, const torch::Tensor &obs) {
        std::shared_ptr<TreeNode> newRoot;

        // Cut-off useless branches.
        for (auto i = _mcts->root()->childrenBegin(); i != _mcts->root()->childrenEnd(); ++i) {
            if ((*i)->action() == action) {
                newRoot = *i;
                newRoot->clearChildren();
                newRoot->disconnectParent();
                break;
            }
        }
        _mcts->setRoot(newRoot);

        // Update posterior beliefs of the new root according to new observation.
        auto posterior = BayesianFiltering::integrateEvidence(newRoot->beliefs(), _a, obs);
        newRoot->setBeliefs(posterior);
    }

    void BTAI::writeGraphviz(const std::string &file_name, const std::vector<std::string> &display) {
        std::pair<std::string, int> dvn("n", 0);
        GraphViz viz(file_name);

        viz.writeTree(dvn, _mcts->root(), display);
    }

}
