//
// Created by Theophile Champion on 02/08/2021.
//

#include "Torch.h"
#include <random>

using namespace torch;
using namespace torch::nn;

namespace btai::wrappers {

    Tensor Torch::oneHot(int size, int id) {
        return functional::one_hot(torch::tensor({id}), size).to(kDouble).squeeze();
    }

    Tensor Torch::empty(torch::IntArrayRef shape) {
        return torch::empty(shape).to(kDouble);
    }

    Tensor Torch::zeros(torch::IntArrayRef shape) {
        return torch::zeros(shape).to(kDouble);
    }

    Tensor Torch::tensor(torch::IntArrayRef values) {
        return torch::tensor(values).to(kDouble);
    }

    Tensor Torch::uniform(const IntArrayRef &sizes, int dim) {
        // Check that inputs are correct
        size_t dims = sizes.size();
        assert(dims <= 3 && "Ops::uniform, dimension described by input sizes must be <= 3.");
        assert(dim < dims && "Ops::uniform, input \"dim\" must be inferior to the number of dimensions.");

        // Implement the function
        auto n_elems = (double) sizes[dim];
        return Torch::full(sizes, 1.0 / n_elems);
    }

    Tensor Torch::full(torch::IntArrayRef shape, double value) {
        return torch::full(shape, value).to(kDouble);
    }

    Tensor Torch::range(int start, int end) {
        return torch::arange(start, end).to(kDouble);
    }

    int Torch::randomInt(const Tensor &probabilities) {
        return randomInt(toStdVector(probabilities));
    }

    std::vector<double> Torch::toStdVector(const Tensor &w) {
        std::vector<double> weight(w.size(0));
        for (int i = 0; i < w.size(0); ++i) {
            weight[i] = w[i].item<double>();
        }
        return weight;
    }

    int Torch::randomInt(const std::vector<double> &weights) {
        static std::random_device dev;
        static std::mt19937 engine(dev());
        std::discrete_distribution<int> rand_int(weights.begin(), weights.end());

        return rand_int(engine);
    }

}
