//
// Created by Theophile Champion on 28/11/2020.
//

#ifndef BTAI_ENVIRONMENT_H
#define BTAI_ENVIRONMENT_H

#include <torch/torch.h>
#include "environments/EnvType.h"

namespace btai::environments {

    /**
     * Interface representing a general environment.
     */
    class Environment {
    public:
        /**
         * Reset the environment to its initial state.
         * @return the initial observation
         */
        virtual torch::Tensor reset() = 0;

        /**
         * Execute an action in the environment.
         * @param action the action to be executed
         * @return the observation made after executing the action
         */
        virtual torch::Tensor execute(int action) = 0;

        /**
         * Display the environment.
         */
        virtual void print() const = 0;

        /**
         * Getter.
         * @return the number of actions available to the agent
         */
        [[nodiscard]] virtual int actions() const = 0;

        /**
         * Getter.
         * @return the number of states in the environment
         */
        [[nodiscard]] virtual int states() const = 0;

        /**
         * Getter.
         * @return the number of observations in the environment
         */
        [[nodiscard]] virtual int observations() const = 0;

        /**
         * Getter.
         * @return the true likelihood mapping
         */
        virtual torch::Tensor A() = 0;

        /**
         * Getter.
         * @return the true transition mapping
         */
        virtual torch::Tensor B() = 0;

        /**
         * Getter.
         * @return the true initial hidden states
         */
        virtual torch::Tensor D() = 0;

        /**
         * Getter.
         * @return true if the agent solved the environment, false otherwise.
         */
        virtual bool solved() = 0;

        /**
         * Getter.
         * @return the type of environment.
         */
        virtual EnvType type() const = 0;
    };

}

#endif //BTAI_ENVIRONMENT_H
