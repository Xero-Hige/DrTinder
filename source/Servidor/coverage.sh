#! /bin/bash
if [ ! -d "coverage" ]; then
	mkdir coverage
fi

cd coverage

FILES='tests/main.cpp Server.cpp DatabaseManager.cpp MessageHandler.cpp Parser.cpp SharedServerClient.cpp User.cpp Interests.cpp Tokenizer.cpp Parsers/CsvParser.cpp Parsers/JsonParser.cpp'
DIR='../CMakeFiles/unit_test.dir/'
GCOV_FILES=''
for i in $FILES; do
	GCOV_FILES="${GCOV_FILES} ${DIR}${i}.cpp"
done;

gcov $GCOV_FILES

#elimino archivos q no corresponden a nosotros
NAMES_START=$(echo $FILES | sed "s-\.cpp--g" )
for file in $(ls -1); do
	FOUND=''
	for i in $NAMES_START; do
		NAME=${i##*/}
		GREP=$(echo $file | grep "$NAME")
		if [ ! -z "$GREP" ]; then
			FOUND='1'
		fi
	done
	if [ -z $FOUND ];then
		rm $file
	fi
done