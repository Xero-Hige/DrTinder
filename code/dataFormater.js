var validator = require('./validator.js');
module.exports = function(){
  var version = "0.2"; //deberia sacarlos del server no???
  function usersFormater(resultado){
    var objeto_sin_formato = resultado['array_to_json'];
    var array_users = [];
    objeto_sin_formato.forEach(function(object){
      array_users.push( { user : object} );
    });

    var data_formateada = {
      users: array_users,
      metadata: {
        version: version,
        count: array_users.length
      }
    }
    //var v = validator.validateUsers(data_formateada);
    //console.log("Users:",v);
    return data_formateada;
  }

  function userFormater(result){
    var objeto_sin_formato = result['row_to_json'];
    var data_formateada = {
      user : objeto_sin_formato,
      metadata: {
      	version: version
      }
    }
    //var v = validator.validateUser(data_formateada);
    //console.log("Userrr:",v);
    return data_formateada;
  }

  function interesesFormater(result){
    var objeto_sin_formato = result['array_to_json'];
    var data_formateada = {
      interests: objeto_sin_formato,
      metadata: {
        version: version,
        count: objeto_sin_formato.length
      }
    }
    //var v = validator.validateInterests(data_formateada);
    //console.log("Interests:",v);
    return data_formateada;
  }

  return {
    intereses: interesesFormater,
    users: usersFormater,
    user: userFormater
  }
}();