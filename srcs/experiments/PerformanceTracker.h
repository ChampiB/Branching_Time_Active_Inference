//
// Created by Theophile Champion on 03/08/2021.
//

#ifndef BTAI_PERFORMANCE_TRACKER_H
#define BTAI_PERFORMANCE_TRACKER_H

#include <iostream>

namespace btai::environments {
    class Environment;
}

namespace btai::experiments {

    /**
     * Interface for performance tracker.
     */
    class PerformanceTracker {
    public:
        /**
         * Reset the performance tracker.
         */
        virtual void reset() = 0;

        /**
         * Update the performance based on the state of the environment.
         * @param env the environment whose state determine the agent performance
         */
        virtual void track(const std::shared_ptr<btai::environments::Environment> &env) = 0;

        /**
         * Display the agent performance in the output stream.
         * @param output the stream in which the performance should be written
         */
        virtual void print(std::ostream &output) const = 0;
    };

}

#endif //BTAI_PERFORMANCE_TRACKER_H
