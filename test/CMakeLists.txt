enable_testing()

add_executable(${UNIT_TEST_NAME}
        apriori_tests.cpp
)

target_link_libraries(${UNIT_TEST_NAME} PRIVATE ${RULES_LIB}
        gtest
        gtest_main
)
