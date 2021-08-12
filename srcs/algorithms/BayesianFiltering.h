//
// Created by Theophile Champion on 01/08/2021.
//

#ifndef BTAI_BAYESIAN_FILTERING_H
#define BTAI_BAYESIAN_FILTERING_H

#include <torch/torch.h>

namespace btai::algorithms {

    class BayesianFiltering {
    public:
        /**
         * Compute the posterior beliefs.
         * @param prior over the hidden states.
         * @param likelihood mapping.
         * @param obs evidence.
         * @return the posterior beliefs.
         */
        static torch::Tensor integrateEvidence(const torch::Tensor &prior, const torch::Tensor &likelihood, const torch::Tensor &obs);

        /**
         * Predict the posterior beliefs over future state.
         * @param prior the prior over current state.
         * @param transition mapping.
         * @return the posterior beliefs over future state.
         */
        static torch::Tensor predict(const torch::Tensor &prior, const torch::Tensor &transition);
    };

}

#endif //BTAI_BAYESIAN_FILTERING_H
