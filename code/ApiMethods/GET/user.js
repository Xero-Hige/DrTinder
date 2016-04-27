var formater = require('../../dataFormater');
var queryDatabase = require('../../queryDatabase.js');
var queryCreator = require('../../queryCreator.js');
var processGet = require('./processGet.js');
var successGetUser = require('./successGet.js').successGetUser;

module.exports = function(request, response) {

    var processGetUser=function(result,response){
    	processGet(result, response, successGetUser, "El usuario a buscar no existe");
    }
    
    var id = request.params.id;
    queryDatabase(queryCreator.fBuscarUser(id), processGetUser, response);
 }