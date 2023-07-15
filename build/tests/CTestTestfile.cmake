# CMake generated Testfile for 
# Source directory: D:/WarGame/tests
# Build directory: D:/WarGame/build/tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(tests "D:/WarGame/build/tests/Debug/tests.exe")
  set_tests_properties(tests PROPERTIES  _BACKTRACE_TRIPLES "D:/WarGame/tests/CMakeLists.txt;13;add_test;D:/WarGame/tests/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(tests "D:/WarGame/build/tests/Release/tests.exe")
  set_tests_properties(tests PROPERTIES  _BACKTRACE_TRIPLES "D:/WarGame/tests/CMakeLists.txt;13;add_test;D:/WarGame/tests/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test(tests "D:/WarGame/build/tests/MinSizeRel/tests.exe")
  set_tests_properties(tests PROPERTIES  _BACKTRACE_TRIPLES "D:/WarGame/tests/CMakeLists.txt;13;add_test;D:/WarGame/tests/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test(tests "D:/WarGame/build/tests/RelWithDebInfo/tests.exe")
  set_tests_properties(tests PROPERTIES  _BACKTRACE_TRIPLES "D:/WarGame/tests/CMakeLists.txt;13;add_test;D:/WarGame/tests/CMakeLists.txt;0;")
else()
  add_test(tests NOT_AVAILABLE)
endif()
