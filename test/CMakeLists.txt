
find_package(GTest REQUIRED)

add_executable(strqueue_test
    strqueue_test.cpp
)

target_link_libraries(strqueue_test
    PRIVATE
        GTest::gtest
        GTest::gtest_main
        strqueue
)

# Enable testing (if you want to use CTest)
#enable_testing()
#add_test(
#   NAME event_recorder_test
#   COMMAND event_recorder_test
#)
