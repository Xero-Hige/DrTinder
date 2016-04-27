var formater = require('../../dataFormater');
var queryDatabase = require('../../queryDatabase.js');
var queryCreator = require('../../queryCreator.js');
var processGet = require('./processGet.js');
var successGetUsers = require('./successGet.js').successGetUsers;

module.exports = function(request, response) {

	var processGetUsers=function(result,response){

		processGet(result, response, successGetUsers, "Base de datos vacia");
	}

    queryDatabase(queryCreator.buscarUsers, processGetUsers, response);

}