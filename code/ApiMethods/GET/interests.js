var queryDatabase = require('../../queryDatabase.js');
var queryCreator = require('../../queryCreator.js');
var processGet = require('./processGet.js');
var successGetInterests = require('./successGet.js').successGetInterests;

module.exports = function(request, response) {

    var processGetInterests=function(result,response){
    	processGet(result, response, successGetInterests, "No encontro intereses");
    }

    queryDatabase(queryCreator.buscarIntereses, processGetInterests, response);
}