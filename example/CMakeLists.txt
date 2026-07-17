add_executable(strqueue_example
    special_queue.cpp
)

target_link_libraries(strqueue_example
    PRIVATE
        strqueue
)
