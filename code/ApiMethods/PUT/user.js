var queryDatabase = require('../../queryDatabase.js');
var queryCreator = require('../../queryCreator.js');
var processPut = require('./processPut.js');
var validator = require('../../validator.js');

module.exports = function(request, response) {

  var processPutUser=function(result,response){
    processPut(result, response, "No pudo modificar el usuario"); 
  }

  var validation = validator.validateUser(request.body);
  if (!validation.ok){
    response.send(400, validation.msg);
    return;
  }
  var user = request.body.user;
  if (request.params.id != user.id){
    response.send(400,"No coincide el id del json con el id del parametro");
    return;
  }
  

  queryDatabase(queryCreator.fActualizarUser(user), processPutUser, response);
}