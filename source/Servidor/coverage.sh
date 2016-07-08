#! /bin/bash
if [ -d "coverage" ]; then
	rm -rf coverage/
fi
mkdir coverage
cd coverage
BASE='../'
DIR='../CMakeFiles/Test.dir'
lcov -b $BASE -c -d $DIR --output-file coverage1.info 


DIR='../CMakeFiles/server.dir'
lcov -b $BASE -c -d $DIR --output-file coverage2.info

#lcov --add-tracefile coverage2.info -t test1 -a coverage1.info -t test2 -o coverage_merged.info
lcov -a coverage1.info -a coverage2.info -o coverage_merged.info
lcov -r coverage_merged.info /usr/include/\* -o coverage_merged.info 
lcov -r coverage_merged.info  /usr/local/include/\* -o coverage_merged.info 
lcov -r coverage_merged.info  \*/source/libs/\* -o coverage_merged.info 
lcov -r coverage_merged.info  \*/source/Servidor/tests/\* -o coverage_merged.info 

genhtml coverage_merged.info
