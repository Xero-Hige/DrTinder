var queryDatabase = require('../../queryDatabase.js');
var queryCreator = require('../../queryCreator.js');
var proccess = require('../../proccess.js');
var formater = require('../../dataFormater.js');
var respondRenderDatos = require('../../processCallbacks.js').respondRenderDatos;
var successGetUsersAndInterests = require('../../ApiMethods/GET/successGet.js').successGetUsersAndInterests;


module.exports = function(request,response){

	var processrenderDatos=function(result, response){
		proccess(result, response, successGetUsersAndInterests, respondRenderDatos, "Error al buscar la base de datos");
	};

    queryDatabase(queryCreator.buscarUsers + queryCreator.buscarIntereses, processrenderDatos, response);
}