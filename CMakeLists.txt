# Require at least CMake 3.16
cmake_minimum_required(VERSION 3.16)

# Create the Branching Time Active Inference
project(BTAI LANGUAGES CXX)

# Compilation flags
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast")

# Require C++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add cmake module to cmake path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Project sources
set(BTAI_SOURCES
        # Experiments package
        srcs/experiments/PerformanceTracker.h
        srcs/experiments/GraphPerformanceTracker.cpp srcs/experiments/GraphPerformanceTracker.h
        srcs/experiments/MazePerformanceTracker.cpp srcs/experiments/MazePerformanceTracker.h
        srcs/experiments/SpritesPerformanceTracker.cpp srcs/experiments/SpritesPerformanceTracker.h
        srcs/experiments/TimeTracker.cpp srcs/experiments/TimeTracker.h
        # Wrapper package
        srcs/wrappers/Torch.cpp srcs/wrappers/Torch.h
        # Algorithm package
        srcs/algorithms/BayesianFiltering.cpp srcs/algorithms/BayesianFiltering.h
        srcs/algorithms/MCTS.cpp srcs/algorithms/MCTS.h
        srcs/algorithms/configurations/ConfigMCTS.cpp srcs/algorithms/configurations/ConfigMCTS.h
        # Environment package
        srcs/environments/EnvType.h
        srcs/environments/Environment.h
        srcs/environments/GraphEnv.cpp srcs/environments/GraphEnv.h
        srcs/environments/MazeEnv.cpp srcs/environments/MazeEnv.h
        srcs/environments/DisentangleSpritesEnv.cpp srcs/environments/DisentangleSpritesEnv.h
        # Graph package
        srcs/graphs/TreeNode.cpp srcs/graphs/TreeNode.h
        srcs/graphs/GraphViz.cpp srcs/graphs/GraphViz.h
        # Zoo package
        srcs/zoo/BTAI.cpp srcs/zoo/BTAI.h
        # Program entry point
        srcs/main.cpp)

# Create the Branching Time Active Inference executable
add_executable(btai ${BTAI_SOURCES})

# Add include directory to executable
target_include_directories(btai PRIVATE srcs)

# Link OpenCV to hopi
find_package(OpenCV REQUIRED)
target_include_directories(btai PUBLIC ${OpenCV_INCLUDE_DIRS})
target_link_libraries(btai PUBLIC ${OpenCV_LIBS})

# Link libtorch to executable
include(LinkLibTorchToTarget)
link_libtorch_to_target(
        TARGET btai
        TORCH_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs/torch"
)