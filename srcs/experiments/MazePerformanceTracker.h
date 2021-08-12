//
// Created by Theophile Champion on 01/07/2021.
//

#ifndef BTAI_MAZE_PERFORMANCE_TRACKER_H
#define BTAI_MAZE_PERFORMANCE_TRACKER_H

#include <vector>
#include <memory>
#include <iostream>
#include "PerformanceTracker.h"

namespace btai::experiments {

    class MazePerformanceTracker : public PerformanceTracker {
    public:
        /**
         * Create a maze performance tracker.
         * @return
         */
        static std::unique_ptr<MazePerformanceTracker> create(
                const std::vector<std::pair<int, int>> &local_minimums_pos,
                int tolerance_level = 1
        );

        /**
         * Constructor of the performance tracker.
         * @param local_minimums_pos the position of the local minimums
         * @param tolerance_level the tolerance level in distance unit
         */
        explicit MazePerformanceTracker(const std::vector<std::pair<int, int>> &local_minimums_pos, int tolerance_level = 1);

        /**
         * Reset the performance tracker.
         */
        void reset() override;

        /**
         * Update the performance based on the state of the environment.
         * @param env the environment whose state determine the agent performance
         */
        void track(const std::shared_ptr<btai::environments::Environment> &env) override;

        /**
         * Display the agent performance in the output stream.
         * @param output the stream in which the performance should be written
         */
        void print(std::ostream &output) const override;

    private:
        int tolerance;
        std::vector<std::pair<int, int>> local_pos;
        std::vector<double> perf;
    };

}

#endif //BTAI_MAZE_PERFORMANCE_TRACKER_H
