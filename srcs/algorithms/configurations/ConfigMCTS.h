//
// Created by Theophile Champion on 02/08/2021.
//

#ifndef BTAI_CONFIG_MCTS_H
#define BTAI_CONFIG_MCTS_H

#include <memory>

namespace btai::algorithms::configurations {

    class ConfigMCTS {
    public:
        /**
         * Create a configuration for the MCTS algorithm.
         * @param planningSteps number of planning iteration.
         * @param expConst exploration constant.
         * @param prefPrecision precision over prior preferences.
         * @param actionPrecision precision over actions.
         * @return the configuration.
         */
        static std::shared_ptr<ConfigMCTS> create(int planningSteps, double expConst, double prefPrecision, double actionPrecision);

        /**
         * Constructor.
         * @param planningSteps number of planning iteration.
         * @param expConst exploration constant.
         * @param prefPrecision precision over prior preferences.
         * @param actionPrecision precision over actions.
         */
        ConfigMCTS(int planningSteps, double expConst, double prefPrecision, double actionPrecision);

        /**
         * Getter.
         * @return the exploration constant of the MCTS algorithm.
         */
        [[nodiscard]] double explorationConstant() const;

        /**
         * Getter.
         * @return the precision over actions of the MCTS algorithm.
         */
        [[nodiscard]] double actionPrecision() const;

        /**
         * Getter.
         * @return the precision over prior preferences.
         */
        [[nodiscard]] double preferencesPrecision() const;

        /**
         * Getter.
         * @return the number of planning iterations.
         */
        [[nodiscard]] int nbPlanningSteps() const;

        /**
         * Setter.
         * @param value new number of planning iteration.
         */
        void setPlanningSteps(int value);

        /**
         * Setter.
         * @param value new precision over prior preferences.
         */
        void setPrecisionPreferences(double value);

        /**
         * Setter.
         * @param value new precision over action.
         */
        void setPrecisionActionSelection(double value);

        /**
         * Setter.
         * @param value new exploration constant.
         */
        void setExplorationConstant(double value);

    private:
        double _expConst;
        double _aPrecision;
        double _cPrecision;
        int _planningSteps;
    };

}

#endif //BTAI_CONFIG_MCTS_H
