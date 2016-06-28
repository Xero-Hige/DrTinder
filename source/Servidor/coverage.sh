#! /bin/bash
if [ -d "coverage" ]; then
	rm -rf coverage/
fi
mkdir coverage
cd coverage

DIR='../CMakeFiles'
lcov --directory $DIR --capture --output-file output.info
lcov -r output.info /usr/include/\* -o output.info
lcov -r output.info /usr/local/include/\* -o output.info
lcov -r output.info \*/source/libs/\* -o output.info
lcov -r output.info \*/Servidor/tests/\* -o output.info

genhtml output.info 
