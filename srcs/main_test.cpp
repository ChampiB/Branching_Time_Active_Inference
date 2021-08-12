//
// Created by Theophile Champion on 01/08/2021.
//

#include <cstdlib>
#include <iostream>
#include <math.h>
#include "experiments/TimeTracker.h"

using namespace btai::experiments;

int main() {
    int n = 10000000;
    std::string OUTPUT_FILE_NAME = "../results/btai_bf_results.txt";

    std::ofstream file;
    file.open(OUTPUT_FILE_NAME, std::ios_base::app);

    auto time_tracker = TimeTracker::create();

    // Compute exponential function n times
    time_tracker->tic();
    for (int j = 0; j < n; ++j) {
        std::exp(j);
    }
    time_tracker->toc();
    time_tracker->print(file);

    // Compute logarithm function n times
    time_tracker->tic();
    for (int j = 0; j < n; ++j) {
        std::log(j);
    }
    time_tracker->toc();
    time_tracker->print(file);

    // Compute addition n times
    time_tracker->tic();
    for (int j = 0; j < n; ++j) {
        j + j;
    }
    time_tracker->toc();
    time_tracker->print(file);

    // Compute addition n times
    time_tracker->tic();
    for (int j = 0; j < n; ++j) {
        j * j;
    }
    time_tracker->toc();
    time_tracker->print(file);

    return EXIT_SUCCESS;
}