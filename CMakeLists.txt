cmake_minimum_required (VERSION 2.6)

project (RCCar)

set(ALL_SOURCES RCCar.cpp RCController.cpp Tachometer.cpp EngineSoundThread.cpp)

#Normal Build
add_executable(RCCar ${ALL_SOURCES} main.cpp)
target_link_libraries(RCCar -lpca9685 -lwiringPi -pthread)

#GTest related build
set(TEST_SOURCES test/testMain.cpp test/RCCarFixture.cpp test/RCClassTest.cpp test/RCControllerFixture.cpp test/RCControllerTest.cpp test/TachometerTest.cpp)
add_executable(gtest_RCCar EXCLUDE_FROM_ALL ${ALL_SOURCES} ${TEST_SOURCES})
target_include_directories(gtest_RCCar PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(gtest_RCCar -lgtest -pthread)

#Install
install(TARGETS RCCar DESTINATION "/usr/local/bin")