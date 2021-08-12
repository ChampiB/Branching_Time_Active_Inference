//
// Created by Theophile Champion on 01/08/2021.
//

#include "BayesianFiltering.h"

using namespace torch;

namespace btai::algorithms {

    Tensor BayesianFiltering::integrateEvidence(const Tensor &prior, const Tensor &likelihood, const Tensor &obs) {
        Tensor posterior = torch::matmul(likelihood.t(), obs) * prior;
        return posterior / posterior.sum();
    }

    torch::Tensor BayesianFiltering::predict(const Tensor &prior, const Tensor &transition) {
        return torch::matmul(transition, prior);
    }

}
