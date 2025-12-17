# Test executable
add_executable(unit_tests test_main.cpp)
target_link_libraries(unit_tests PRIVATE msh_lib)
add_test(NAME CoreTests COMMAND unit_tests)

# Sequence Test Executable
add_executable(manual_sequence_test manual_sequence_test.cpp)
target_link_libraries(manual_sequence_test PRIVATE msh_lib)
add_test(NAME SequenceTests COMMAND manual_sequence_test)