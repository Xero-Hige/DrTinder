.. _dbDiagram:

Shared Server DB Model
======================

.. image:: db.png

**********
Definicion
**********
.. code-block:: plpgsql

	DROP DATABASE IF EXISTS tinder;
	CREATE DATABASE tinder WITH  CONNECTION LIMIT = -1;
	\c tinder
	CREATE TYPE sexo AS ENUM ('man', 'woman');

	CREATE TABLE users_table(
	 	id  serial PRIMARY KEY,
 		alias  varchar(20),  
 		name   varchar(20), 
 		email  varchar(30) UNIQUE,
 		sex    sexo,
 		age    integer,
 		photo_profile  text
	);

	CREATE TABLE location_table(
		id integer UNIQUE references users_table(id),
		latitude double precision DEFAULT 0,
		longitude double precision DEFAULT 0
	);

	CREATE TABLE interest_table(
		category varchar(20),
		value varchar(20),
		PRIMARY KEY (category,value)
	);


	INSERT INTO interest_table (category,value)
	VALUES ('sex','man'),
		('sex','woman'),
		('music','rock'),
		('sport','tennis'),
		('outdoors','running'),
		('travel','beach'),
		('food','meat'),
		('music/band','pearl jam');
	
	
	CREATE TABLE users_interest(
		id integer references users_table(id),
		category varchar(20),
		value varchar(20),
		FOREIGN KEY (category, value) REFERENCES interest_table (category, value)
	);
