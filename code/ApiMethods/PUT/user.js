var formater = require('../../dataFormater');
var queryDatabase = require('../../queryDatabase.js');
var queryCreator = require('../../queryCreator.js');
var processPut = require('./processPut.js');

module.exports = function(request, response) {
  // TODO verificar que id a postear y id de user es el mismo
  
  var processPutUser=function(result,response){
    processPut(result, response, "No pudo modificar el usuario"); 
  }

  var user = request.body.user;
  if (!formater.validateUser(user)) {
    response.send(400, malFormato);
    return;
  }
  queryDatabase(queryCreator.fActualizarUser(user), processPutUser, response);
}