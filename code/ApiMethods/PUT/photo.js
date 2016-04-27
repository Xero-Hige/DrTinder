var formater = require('../../dataFormater');
var queryDatabase = require('../../queryDatabase.js');
var queryCreator = require('../../queryCreator.js');
var processPut = require('./processPut.js');

module.exports =putFotoAPI = function(request, response) {

  var processPutPhoto=function(result,response){
   
    processPut(result, response, "No Puedo modificar la foto");

   } 

  //validar photo TODO
  var id = request.params.id;
  var photo_text = request.body.photo;
  queryDatabase(queryCreator.fActualizarFoto(photo_text, id), processPutPhoto, response);
}