var queryDatabase = require('../../queryDatabase.js');
var queryCreator = require('../../queryCreator.js');
var getLastUser = require('../GET/lastUser.js');
var validator = require('../../validator.js');

module.exports = function(request, response) {

    var validation = validator.validatePostUser(request.body);
    if (!validation.ok) {
      response.send(400, validation.msg);
      return;
    }
    var user = request.body.user;
    queryDatabase(queryCreator.fAltaUser(user), getLastUser, response);
}