//
// Created by Theophile Champion on 02/08/2021.
//

#ifndef BTAI_BTAI_H
#define BTAI_BTAI_H

#include <memory>
#include <torch/torch.h>

namespace btai::environments {
    class Environment;
}
namespace btai::algorithms::configurations {
    class ConfigMCTS;
}
namespace btai::algorithms {
    class MCTS;
}
namespace btai::graphs {
    template<class DataType> class TreeNode;
}
namespace btai::graphs::data {
    class DataMCTS;
}

namespace btai::zoo {

    class BTAI {
    public:
        /**
         * Create a Partially Observable Markov Decision Process.
         * @param env the environment.
         * @param config the configuration of the tree search.
         * @param obs the initial observation.
         * @return the BTAI agent.
         */
        static std::shared_ptr<BTAI> create(
            const std::shared_ptr<environments::Environment> &env,
            const std::shared_ptr<algorithms::configurations::ConfigMCTS> &config,
            const torch::Tensor &obs
        );

        /**
         * Create a Partially Observable Markov Decision Process.
         * @param env the environment.
         * @param config the configuration of the tree search.
         * @param obs the initial observation.
         */
        BTAI(
            const std::shared_ptr<environments::Environment> &env,
            const std::shared_ptr<algorithms::configurations::ConfigMCTS> &config,
            const torch::Tensor &obs
        );

        /**
         * Execute on step of the action perception cycle in the environment.
         * @param env the environment to act in.
         */
        void step(const std::shared_ptr<environments::Environment> &env);

        /**
         * Update the tree to reflect the action taken and the observation obtained.
         * @param action action performed in the environment.
         * @param obs observation retrieved from the environment.
         */
        void integrate(int action, const torch::Tensor &obs);

        /**
         * Write the planning tree into a file using GraphViz format.
         * @param file_name the output file name.
         * @param display the list of attributes to display.
         */
        void writeGraphviz(const std::string &file_name, const std::vector<std::string> &display);

    private:
        torch::Tensor _a;
        torch::Tensor _b;
        torch::Tensor _c;
        torch::Tensor _d;

        std::unique_ptr<algorithms::MCTS> _mcts;
        std::shared_ptr<graphs::TreeNode<graphs::data::DataMCTS>> _root;
    };

}

#endif //BTAI_BTAI_H
