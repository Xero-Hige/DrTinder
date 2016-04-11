module.exports = function (){
	var queryBuscarUsers = 
	"SELECT array_to_json(array_agg(row_to_json(users))) " +
	"FROM (SELECT *, " +               
	"(SELECT row_to_json(d) " +
	"FROM (SELECT latitude, longitude " +
	"FROM location_table " +
	"WHERE location_table.id = users_table.id" +
	") d"+
	") AS location, " +
	"( SELECT array_to_json(array_agg(row_to_json(i))) " +
	"FROM (SELECT category,value " +
	"FROM users_interest " +
	"WHERE users_interest.id = users_table.id " +
	") i " +
	") AS interests " +
	"FROM users_table) AS users;";

	var queryBuscarIntereses = "SELECT array_to_json(array_agg(row_to_json(interest))) " + 
	"FROM (SELECT * FROM interest_table) as interest;"


	function queryBuscarUser(id){

		var query = 
		"SELECT row_to_json(users) " +
		"FROM (SELECT *, " +
		"(SELECT row_to_json(d) "+
		"FROM (SELECT latitude, longitude " +
		"FROM location_table " +
		"WHERE location_table.id = users_table.id " +
		") d " +
		") AS location, " +
		"( SELECT array_to_json(array_agg(row_to_json(i))) " +
		"FROM (SELECT category,value " +
		"FROM users_interest " +
		"WHERE users_interest.id = users_table.id " +
		") i " +
		") AS interests " +
		"FROM users_table WHERE id = " + id + " " +
		") AS users;";

		return query;
	}

	function queryAgregarInteres(parametros){
		var category = parametros.category;
		var value = parametros.value;
		var query = 
		"INSERT INTO interest_table ( category, value) " +
		"VALUES ('" + category + "','" + value +"');";

		return query;
	}

	function queryDeleteUser(id){
		var query = 
		"DELETE FROM location_table " +
		"WHERE location_table.id= " + id + "; ";

		query += 
		"DELETE FROM users_interest " +
		"WHERE users_interest.id= " + id + ";";

		query += 
		"DELETE FROM users_table " + 
		"WHERE users_table.id=" + id + ";";

		return query;
	}

	function queryActualizarUser(user){
		var query = 
		"UPDATE users_table "+
		"SET photo_profile="+ text + " " +
		"WHERE id = " + id + ";";
	}

	function queryAltaUser(user){

		var queryStart = "DO $$ " +
		"DECLARE lastid bigint; " +
		"BEGIN ";

		var queryUsersTable = 
		"INSERT INTO users_table (alias, name, email, sex, age , photo_profile) " +
		"VALUES ('" + user.alias + "','" + user.name + "','" +
		user.email + "','" + user.sex + "','" + user.age + "','" +
		user.photo_profile + "') " +
		"RETURNING id INTO lastid; ";

		var queryLocation = 
		"INSERT INTO location_table (id, latitude, longitude) " + 
		"VALUES ( lastid, '" + user.location.latitude + "','" + 
		user.location.longitude + "'); ";

		//habria que revisar efectividad, no son muchos intereses pero por las dudas
		var values_intereses = "";
		for (var i = 0; i < user.interests.length; i++){
			var interes = user.interests[i];
			values_intereses += "( lastid,'" + interes.category + 
			"','" + interes.value  + "'),";
		}

		//para sacar coma de mas
		values_intereses = values_intereses.slice( 0, -1);

		var queryIntereses = 
		"INSERT INTO users_interest (id, category, value) " + 
		"VALUES " +	values_intereses + "; ";

		var queryEnd = "END $$;";


		return queryStart + queryUsersTable + queryLocation + queryIntereses + queryEnd  ;
		//agrego a todas las tablas
	}

	function queryActualizarFoto(text,id){
		var query = 
		"UPDATE users_table "+
		"SET photo_profile="+ text + " " +
		"WHERE id = " + id + ";";

		return query;
	}
	


	return {
		buscarUsers : queryBuscarUsers,
		buscarIntereses: queryBuscarIntereses,
		//funciones :
		fBuscarUser: queryBuscarUser,
		fAgregarInteres: queryAgregarInteres,
		fDeleteUser: queryDeleteUser,
		fActualizarUser: queryActualizarUser,
		fAltaUser: queryAltaUser,
		fActualizarFoto: queryActualizarFoto

	};
}();