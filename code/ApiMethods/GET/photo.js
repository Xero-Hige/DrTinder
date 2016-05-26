var queryDatabase = require('../../queryDatabase.js');
var queryCreator = require('../../queryCreator.js');
var processGet = require('./processGet.js');
var successGetPhoto = require('./successGet.js').successGetPhoto;

module.exports = function(request, response) {

    var processGetPhoto=function(result,response){
    	processGet(result, response, successGetPhoto, "No existe la foto a buscar");
    }
    
    var id = request.params.id;
    queryDatabase(queryCreator.fGetPhoto(id), processGetPhoto, response);
 }