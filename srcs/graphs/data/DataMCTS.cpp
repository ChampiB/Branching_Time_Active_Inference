//
// Created by Theophile Champion on 01/08/2021.
//

#include "DataMCTS.h"

using namespace torch;

namespace btai::graphs::data {

    std::unique_ptr<DataMCTS> DataMCTS::create(const Tensor &beliefs, int action, double cost, int visits) {
        return std::make_unique<DataMCTS>(beliefs, action, cost, visits);
    }

    DataMCTS::DataMCTS(const Tensor &beliefs, int action, double cost, int visits) {
        _action = action;
        _cost = cost;
        _beliefs = beliefs;
        _visits = visits;
    }

    void DataMCTS::increaseVisitCount() {
        ++_visits;
    }

    void DataMCTS::addCost(double cost) {
        _cost += cost;
    }

    void DataMCTS::setBeliefs(const Tensor &beliefs) {
        _beliefs = beliefs;
    }

    int DataMCTS::visits() const {
        return _visits;
    }

    int DataMCTS::action() const {
        return _action;
    }

    double DataMCTS::cost() const {
        return _cost;
    }

    Tensor DataMCTS::beliefs() const {
        return _beliefs;
    }

    std::string DataMCTS::get(const string &attributeName) {
        if (attributeName == "G")
            return std::to_string(_cost);
        if (attributeName == "U")
            return std::to_string(_action);
        if (attributeName == "N")
            return std::to_string(_visits);
        if (attributeName == "S")
            return std::to_string(torch::argmax(_beliefs).item<int>());
        throw std::runtime_error("In DataMCTS::get(key), unsupported key.");
    }

}
