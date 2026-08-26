cmake_minimum_required(VERSION 3.28)
project(PortfolioOptimizationandRiskEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)   # ← enforce C++20; don't silently fall back

add_executable(main project/main.cpp)
add_executable(iterators cppPrimerLearning/iteratorslearning.cpp)
add_executable(arrays cppPrimerLearning/arraylearning.cpp)
add_executable(readingwriting cppPrimerLearning/readingwriting.cpp)
add_executable(vectorwriting cppPrimerLearning/vectorwriting.cpp)
add_executable(reallocation cppPrimerLearning/reallocation_learning.cpp)
add_executable(lambdas cppPrimerLearning/lambdas.cpp)
add_executable(maps cppPrimerLearning/maps.cpp)
add_executable(classExercise cppPrimerLearning/classExercisew.cpp)
add_executable(engine project/main.cpp project/Matrix.cpp project/DataLoader.cpp)