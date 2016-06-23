/*
 * setUpDataBase.h
 *
 *  Created on: Jun 23, 2016
 *      Author: tobias
 */

#ifndef SERVIDOR_TESTS_SETUPDATABASE_H_
#define SERVIDOR_TESTS_SETUPDATABASE_H_
#include "../../libs/loger/easylogging++.h"


void setUpDatabaseTest(rocksdb::DB** db, std::string db_name) {
	rocksdb::Options options;
	options.create_if_missing = true;

	LOGG(INFO) << "Opening " << db_name;

	rocksdb::Status status = rocksdb::DB::Open(options, db_name, db);
	if (! status.ok()) {
		LOGG(FATAL) << "Could not open database";
	}else{
		LOGG(INFO) << "Conexion exitosa a la base de datos";
	}
}


#endif /* SERVIDOR_TESTS_SETUPDATABASE_H_ */
