/*
 * Interests.cpp
 *
 *  Created on: Jun 2, 2016
 *      Author: tobias
 */

#include "Interests.h"

using namespace std;

Interests::Interests() {
	// TODO Auto-generated constructor stub

}

void Interests::importFrom(string interest, dataType type){
	switch(type){
		case JSON_DATA:
			//TODO parseo
			break;
		case CSV_DATA:
			//TODO parseo
			break;
		default:
			return;
	}
}

void Interests::add(std::string category, std::string value){
	map<string,vector<string>>::iterator it = this->intereses.find(category);

	if ( it != this->intereses.end() ){
		vector<string> *valores = &it->second;
		//http://stackoverflow.com/questions/3450860/check-if-a-stdvector-contains-a-certain-object
		if (std::find(valores->begin(), valores->end(), value) == valores->end()){
			valores->push_back(value);
		}

	}else{
		vector<string> valores;
		valores.push_back(value);
		this->intereses[category] = valores;
	}
}

bool Interests::getAll(string category,vector<string> &values){
	map<string,vector<string>>::iterator it = this->intereses.find(category);

	if ( it != this->intereses.end() ){
		//devuelve copia
		values = it->second;
		return true;
	}
	return false;

}

bool Interests::has(string category, string value){
	map<string,vector<string>>::iterator it = this->intereses.find(category);

	if ( it != this->intereses.end() ){
		//codigo duplicado TODO
		vector<string> *valores = &it->second;
		if (std::find(valores->begin(), valores->end(), value) != valores->end()){
			return true;
		}
	}

	return false;
}

void Interests::remove(string category, string value){
	map<string,vector<string>>::iterator it = this->intereses.find(category);

	if ( it != this->intereses.end() ){
		vector<string> *valores = &it->second;
		vector<string>::iterator position = find(valores->begin(), valores->end(), value);
		if (position != valores->end()){
			valores->erase(position);
		}

	}
}

Interests::~Interests() {

}

