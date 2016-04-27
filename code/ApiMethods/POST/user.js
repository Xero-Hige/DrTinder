var formater = require('../../dataFormater');
var queryDatabase = require('../../queryDatabase.js');
var queryCreator = require('../../queryCreator.js');
var getLastUser = require('../GET/lastUser.js');

module.exports = function(request, response) {
	
    var user = request.body.user;
    if (!formater.validateUser(user, true)) {
      response.send(400, malFormato);
      return;
    }

    queryDatabase(queryCreator.fAltaUser(user), getLastUser, response);
}