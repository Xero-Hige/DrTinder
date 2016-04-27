var formater = require('../../dataFormater');
var queryDatabase = require('../../queryDatabase.js');
var queryCreator = require('../../queryCreator.js');
var proccess = require('../../proccess.js');
var respondSimple = require('../../processCallbacks.js').respondSimple;

module.exports = function(request, response) {
	var successPostInterest=function(result){
		return { status: 201,
      result: "Se creo el interes con exito"};
	}

	var processPostInterest=function(result,response){
		proccess(result, response, successPostInterest, respondSimple, "No se inserto el interes");
	}

    var interes = request.body.interest;
    if (!formater.validateInterest(interes)) {
      response.send(400, malFormato);
      return;
    }
    queryDatabase(queryCreator.fAgregarInteres(interes), processPostInterest, response);
  }

