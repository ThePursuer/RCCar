cmake_minimum_required (VERSION 2.6)

project (RCCar)

set(ALL_SOURCES RCCar.cpp RCController.cpp EngineAudio.cpp)
set(LINK_LIBS -pthread -lfmodstudio)

#Normal Build
add_executable(RCCar ${ALL_SOURCES} main.cpp RCCar_imp.cpp)
target_link_libraries(RCCar -lpca9685 -lwiringPi ${LINK_LIBS})

#GTest related build
set(TEST_SOURCES test/testMain.cpp test/RCCarFixture.cpp test/RCClassTest.cpp test/RCControllerFixture.cpp test/RCControllerTest.cpp)
add_executable(gtest_RCCar EXCLUDE_FROM_ALL ${ALL_SOURCES} ${TEST_SOURCES})
target_include_directories(gtest_RCCar PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(gtest_RCCar -lgtest ${LINK_LIBS})

#Install
install(TARGETS RCCar DESTINATION "/usr/local/bin")