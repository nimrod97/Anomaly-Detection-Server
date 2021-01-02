cmake_minimum_required(VERSION 3.0.0)
project(milestone3 VERSION 0.1.0)

# include(CTest)
# enable_testing()
add_compile_options(-std=c++11)

add_executable(milestone3 mainTrain.cpp HybridAnomalyDetector.cpp SimpleAnomalyDetector.cpp minCircle.cpp timeseries.cpp anomaly_detection_util.cpp CLI.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)
