var queryDatabase = require('../../queryDatabase.js');
var queryCreator = require('../../queryCreator.js');
var proccess = require('../../proccess.js');
var respondRenderDatos = require('../../processCallbacks.js').respondRenderDatos;
var successGetUsersAndInterests = require('../../ApiMethods/GET/successGet.js').successGetUsersAndInterests;


module.exports = function(request,response, url){

	var succesData = function(result){
		return successGetUsersAndInterests(result,url);
	}

	var processrenderDatos=function(result, response){
		proccess(result, response, succesData, respondRenderDatos, "Error al buscar la base de datos");
	};

    queryDatabase(queryCreator.buscarUsers + queryCreator.buscarIntereses, processrenderDatos, response);
}