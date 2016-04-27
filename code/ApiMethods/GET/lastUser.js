var formater = require('../../dataFormater');
var queryDatabase = require('../../queryDatabase.js');
var queryCreator = require('../../queryCreator.js');
var processGet = require('./processGet.js');

module.exports = function(request,response){
    var successPostUser=function(result){
    	var user = formater.user(result.rows[0]);
    	return {
      	status: 201,
      	result: user
    	};
    }
    var processGetLastUser = function(result, response){
      processGet(result, response, successPostUser, "No encontro el last user");
    }

    queryDatabase(queryCreator.lastUser, processGetLastUser, response);
}