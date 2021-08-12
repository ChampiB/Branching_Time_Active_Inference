//
// Created by Theophile Champion on 02/08/2021.
//

#ifndef BTAI_TORCH_H
#define BTAI_TORCH_H

#include <torch/torch.h>

namespace btai::wrappers {

    class Torch {
    public:
        /**
         * Create a one hot vector.
         * @param size vector size.
         * @param id index where the one should be put.
         * @return the one hot vector.
         */
        static torch::Tensor oneHot(int size, int id);

        /**
         * Create an empty tensor.
         * @param shape of the tensor to be created.
         * @return the created tensor.
         */
        static torch::Tensor empty(torch::IntArrayRef shape);

        /**
         * Create a tensor filled with a value.
         * @param shape of the tensor.
         * @param value used to fill the vector.
         * @return the created tensor.
         */
        static torch::Tensor full(torch::IntArrayRef shape, double value);

        /**
         * Create a tensor containing a range of values.
         * @param start of the range.
         * @param end of the range.
         * @return the created tensor.
         */
        static torch::Tensor range(int start, int end);

        /**
         * Sample a random integer from a categorical distribution.
         * @param probabilities the parameters of the categorical distribution.
         * @return the selected integer.
         */
        static int randomInt(const torch::Tensor &probabilities);

    private:
        /**
         * Turn a 1d torch::Tensor into a std::vector.
         * @param tensor to turn into a std::vector.
         * @return the std::vector.
         */
        static std::vector<double> toStdVector(const torch::Tensor &tensor);

        /**
         * Sample a random integer from a categorical distribution.
         * @param weights the parameters of the categorical distribution.
         * @return the selected integer.
         */
        static int randomInt(const std::vector<double> &weights);
    };

}

#endif //BTAI_TORCH_H
