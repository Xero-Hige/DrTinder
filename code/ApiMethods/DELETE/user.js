var formater = require('../../dataFormater');
var queryDatabase = require('../../queryDatabase.js');
var queryCreator = require('../../queryCreator.js');
var proccess = require('../../proccess.js');
var respondSimple = require('../../processCallbacks.js').respondSimple;

module.exports = function(request, response) {
 var successDelete=function(result){
  return { status: 200,
   result: "Elimino" };
 }

 var processDelete= function(result,response){
  proccess(result, response, successDelete, respondSimple, "El usuario a eliminar no existe");
 }
 queryDatabase(queryCreator.fDeleteUser(request.params.id), processDelete, response);
}