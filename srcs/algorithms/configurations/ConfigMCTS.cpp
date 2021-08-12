//
// Created by Theophile Champion on 02/08/2021.
//

#include "ConfigMCTS.h"

namespace btai::algorithms::configurations {

    std::shared_ptr<ConfigMCTS> ConfigMCTS::create(int planningSteps, double expConst, double prefPrecision, double actionPrecision) {
        return std::make_shared<ConfigMCTS>(planningSteps, expConst, prefPrecision, actionPrecision);
    }

    ConfigMCTS::ConfigMCTS(int planningSteps, double expConst, double prefPrecision, double actionPrecision) {
        _planningSteps = planningSteps;
        _expConst = expConst;
        _cPrecision = prefPrecision;
        _aPrecision = actionPrecision;
    }

    double ConfigMCTS::explorationConstant() const {
        return _expConst;
    }

    double ConfigMCTS::actionPrecision() const {
        return _aPrecision;
    }

    double ConfigMCTS::preferencesPrecision() const {
        return _cPrecision;
    }

    int ConfigMCTS::nbPlanningSteps() const {
        return _planningSteps;
    }

    void ConfigMCTS::setPlanningSteps(int value) {
        _planningSteps = value;
    }

    void ConfigMCTS::setPrecisionPreferences(double value) {
        _cPrecision = value;
    }

    void ConfigMCTS::setPrecisionActionSelection(double value) {
        _aPrecision = value;
    }

    void ConfigMCTS::setExplorationConstant(double value) {
        _expConst = value;
    }

}
