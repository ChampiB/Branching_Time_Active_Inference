//
// Created by Theophile Champion on 30/07/2021.
//

#ifndef BTAI_GRAPH_PERFORMANCE_TRACKER_H
#define BTAI_GRAPH_PERFORMANCE_TRACKER_H

#include <vector>
#include <memory>
#include <ostream>
#include "PerformanceTracker.h"

namespace btai::experiments {

    class GraphPerformanceTracker : public PerformanceTracker{
    public:
        enum PerfOutcome {
            GOAL = 0,
            STILL_RUNNING = 1,
            BAD_STATE = 2
        };

    public:
        /**
         * Create a graph performance tracker.
         * @return
         */
        static std::unique_ptr<GraphPerformanceTracker> create();

        /**
         * Constructor of the performance tracker.
         */
        explicit GraphPerformanceTracker();

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
        std::vector<double> perf;
    };

}

#endif //BTAI_GRAPH_PERFORMANCE_TRACKER_H
