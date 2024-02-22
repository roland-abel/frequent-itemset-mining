
add_library(${RULES_LIB}
        apriori.cpp
)

target_include_directories(${RULES_LIB} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ../include)
