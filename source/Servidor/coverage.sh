#! /bin/bash
if [ -d "coverage" ]; then
	rm -rf coverage/
fi
mkdir coverage
cd coverage

DIR='../CMakeFiles/Test.dir'
lcov --directory $DIR --capture --output-file coverage1.info 
lcov -r coverage1.info  /usr/include/\* -o coverage1.info 
lcov -r coverage1.info  /usr/local/include/\* -o coverage1.info 
lcov -r coverage1.info  \*/source/libs/\* -o coverage1.info 
lcov -r coverage1.info  \*/Servidor/tests/\* -o coverage1.info coverage1.info 

DIR='../CMakeFiles/server.dir'
lcov --directory $DIR --capture --output-file coverage2.info
lcov -r coverage2.info /usr/include/\* -o coverage2.info
lcov -r coverage2.info /usr/local/include/\* -o coverage2.info
lcov -r coverage2.info \*/source/libs/\* -o coverage2.info
lcov -r coverage2.info \*/Servidor/tests/\* -o coverage2.info

lcov --add-tracefile coverage2.info -t testPyth -a coverage1.info -t testC -o coverage_merged.info

genhtml coverage_merged.info
