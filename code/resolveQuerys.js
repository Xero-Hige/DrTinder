

module.exports = function() {
  var pg = require('pg');
  var queryCreator = require('./queryCreator');
  var formater = require('./dataFormater');
  var malFormato = "Error en formato";

  /*Para hacer cualequier query le tengo que pasar:
  - Un query string
  - como procesar el resultado
  - la response para que el proceso del resultado pueda dar una respuesta*/

  function queryDatabase(query, processResult, response) {
    //conecto a base de datos

    pg.connect(process.env.DATABASE_URL, function(err, client, done) {
      
      if (err) {
      	done();
        //probema que tengo en la fiuba
        response.send(500, "No hay conexion a la base de datos");
      } else {
        client.query(query, function(err, result) {
          done();
          if (err) {
            response.send(500, err);
          } else {
            processResult(result, response);
          }
        });
      }
    });
  }


  /*Procesar result
  -response para dar la respuesta
  -succes es la funcion que devuelve el resultado correcto
  -callback es la fucnion que decide como dar el resultado al que lo pidio
  -err_msg el error a dar en caso de que no hubo respuesta desada*/

  function proccess(result, response, success, callback, err_msg) {
    var resultado;
    //Si no trajo nada
    if (result.rowCount == 0) {
      console.log("No respondio nada");
      resultado = {
        status: 400,
        result: err_msg
      };
    } else {
      resultado = success(result);
    }
    callback(resultado, response);
  }

  /*Que resultado traer si lo consegui correctamente*/

  function successGet(result, formater) {
    var data_formateada = formater(result.rows[0]);
    resultado = {
      status: 200,
      result: data_formateada
    };
    return resultado;
  }

  function successGetUsers(result) {
    return successGet(result, formater.users);
  }

  function successGetUser(result) {
     return successGet(result, formater.user);
  }

  function successGetInterests(result) {
    return successGet(result, formater.intereses);
  }

  function successGetUsersAndInterests(result){
    var formatedData = successGetUsers(result);
    var interests = formater.intereses(result.rows[1]);
    formatedData.result["intereses"] = interests.interests;
    return formatedData;
  }

  function successPut(result) {
    return {
      status: 200,
      result: "Se modifico con exito"
    };
  }

  function successPostInterest(result) {
    return {
      status: 201,
      result: "Se creo el interes con exito"
    };
  }

  function successPostUser(result) {
    var user = formater.user(result.rows[0]);
    return {
      status: 201,
      result: user
    };
  }

  function successDelete(result) {
    return {
      status: 200,
      result: "Elimino"
    };
  }


  /*Calbacks basicos de los requests*/

  //API respond basico 
  function respondSimple(resultado, respuesta) {
    respuesta.send(resultado.status, resultado.result);
  }

  //Web, mostrar users
  function respondRenderDatos(resultado, response) {
    if (resultado.status >= 400) {
      respondSimple(resultado, respuesta);
    } else {

      response.render('pages/db', {
        results: resultado.result.users,
        intereses: resultado.result.intereses
      });
    }
  }

  //Para devolver Json
  function respondJson(resultado, respuesta) {
    if (resultado.status >= 400) {
      respondSimple(resultado, respuesta);
    } else {
      console.log(resultado.result);
      respuesta.status(resultado.status).jsonp(resultado.result);
    }
  }
  

  //para los querys que buscan datos, tienen que darle el success valido
  //para el formato de data
  function processGet(result, response, success, err_msg) {
    proccess(result, response, success, respondJson, err_msg);
  }

  function processGetUser(result, response) {
    processGet(result, response, successGetUser, "El usuario a buscar no existe");
  }

  function processGetUsers(result, response) {
    processGet(result, response, successGetUsers, "Base de datos vacia");
  }

  function processGetInterests(result, response) {
    processGet(result, response, successGetInterests, "No encontro intereses");
  }

  function processPostInterest(result, response) {
    proccess(result, response, successPostInterest, respondSimple, "No se inserto el interes");
  }
  //proceso put
  function processPut(result, response, err_msg) {
    proccess(result, response, successPut, respondSimple, err_msg);
  }

  function processPutPhoto(result, response) {
    processPut(result, response, "No Puedo modificar la foto");
  }

  function processPutUser(result,response){
  	processPut(result, response, "No pudo modificar el usuario");	
  }

  function processDelete(result, response) {
    proccess(result, response, successDelete, respondSimple, "El usuario a eliminar no existe");
  }

  //Web, mostrar users
  function processrenderDatos(result, response) {
    proccess(result, response, successGetUsersAndInterests, respondRenderDatos, "Error al buscar la base de datos");
  }


  /*
  Querys a responder:

  WEB 
  -Render

  Funciones API
  -Gets
  -Posts
  -Puts
  -Delte*/


  //REnder
  function renderDatos(request,response){
    queryDatabase(queryCreator.buscarUsers + queryCreator.buscarIntereses, processrenderDatos, response);
  }

  //GET last user, para probar lo hice
  function getLastUser(request,response){
    
    var processGetLastUser = function(result, response){
      proccess(result, response, successPostUser, respondJson, "No encontro el last user");
    }
    console.log(queryCreator.lastUser);
    queryDatabase(queryCreator.lastUser, processGetLastUser, response);
  }

  //GET a /users
  function getAllUsersAPI(request, response) {
    queryDatabase(queryCreator.buscarUsers, processGetUsers, response);

  }
  //GET a /users/id
  function getUserAPI(request, response) {
    var id = request.params.id;
    queryDatabase(queryCreator.fBuscarUser(id), processGetUser, response);
  }

  //POST a /users
  function postUserAPI(request, response) {
    var user = request.body.user;
    console.log(request.body);
    if (!formater.validateUser(user, true)) {
      response.send(400, malFormato);
      return;
    }
    queryDatabase(queryCreator.fAltaUser(user), getLastUser, response);
  }

  //PUT a /users/id
  function modifyUserAPI(request, response) {
    //verificar que id a postear y id de user es el mismo
    var user = request.body.user;
    if (!formater.validateUser(user)) {
      response.send(400, malFormato);
      return;
    }
    //response.send(200,queryCreator.fActualizarUser(user));
    queryDatabase(queryCreator.fActualizarUser(user), processPutUser, response);
  }

  //PUT a /users/id/photo
  function putFotoAPI(request, response) {
    var id = request.params.id;
    var photo_text = request.body.photo;
    queryDatabase(queryCreator.fActualizarFoto(photo_text, id), processPutPhoto, response);
  }

  //DELTE a /users/id
  function deleteUserAPI(request, response) {
    queryDatabase(queryCreator.fDeleteUser(request.params.id), processDelete, response);
  }

  //GET a /interests
  function getInteresesAPI(request, response) {
    queryDatabase(queryCreator.buscarIntereses, processGetInterests, response);
  }

  //POST a /interest
  //201
  function postInteresAPI(request, response) {
    var interes = request.body.interest;
    if (!formater.validateInterest(interes)) {
      response.send(400, malFormato);
      return;
    }
    queryDatabase(queryCreator.fAgregarInteres(interes), processPostInterest, response);
  }

  return {
    getUsers: getAllUsersAPI,
    getUser: getUserAPI,
    getInterests: getInteresesAPI,
    postUser: postUserAPI,
    postInteres: postInteresAPI,
    modifyUser: modifyUserAPI,
    putFoto: putFotoAPI,
    deleteUser: deleteUserAPI,
    renderDatos: renderDatos,
    last: getLastUser
  };

}();