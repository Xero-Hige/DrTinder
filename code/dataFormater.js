
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
    return data_formateada;
  }

  function userFormater(result){
    var objeto_sin_formato = result['row_to_json'];
    var data_formateada = {
      user : objeto_sin_formato
    }
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

    return data_formateada;
  }


  var dict_keys = {
    location: ["latitude","longitude"],
    interests: ["category","value"]
  }

  //MEJORAR TODO
  function hasKeys(array_keys, json){
      for (var i = 0; i < array_keys.length; i++){
        var element = json[array_keys[i]];
        if (!element){
          //se podria devolver para mandar en la respuesta?
          console.log("FALTA KEY "+ array_keys[i]);
          return false;
        }
        if (element.constructor === Array){
          //caso interests
          var new_keys = dict_keys[array_keys[i]];
            for (var j = 0; j < element.length; j++){
              if (!hasKeys(new_keys,element[j])){
                return false;
              }
            }
        } else if(typeof element === 'object'){
          //caso location
          var new_keys = dict_keys[array_keys[i]];
          if (!hasKeys(new_keys,element)){
            return false;
          }
        }

      }
      return true;
  }

  function validateInterest(interes){
      var keys = ["category","value"];
      return hasKeys(keys, interes);
  }

  function validateLocation(location){
    var keys = ["latitude","longitude"];
    return hasKeys(keys,location);
  }

  function validateUser(user,sin_id){
    sin_id = sin_id ? sin_id: false;
    var keys = ["name","alias","sex","age","photo_profile","interests","location","email"];
    if (!sin_id){
        keys.push("id");
    }
    return hasKeys(keys,user);
  }

  return {
    intereses: interesesFormater,
    users: usersFormater,
    user: userFormater,
    validateUser: validateUser,
    validateInterest: validateInterest
  }
}();