var pg = require('pg');
var express = require('express');
var app = express();
var router = express.Router();
var bodyParser = require('body-parser');
var queryCreator = require('./queryCreator');
var formater = require('./dataFormater');

app.use(bodyParser.json());
app.set('port', (process.env.PORT || 5000));

// views is directory for all template files, dir base cuando renderea
app.set('views', __dirname + '/views');
app.set('view engine', 'ejs');

//cuando en la app voy a / realizo esto
app.get('/', function(request, response) {
  response.render('pages/index');
});

//muestra puerto en donde estoy corriendo
app.listen(app.get('port'), function() {
  console.log('Node app is running on port', app.get('port'));
});

//que pasa cuando voy a 
app.get('/db', function (request, response) {
  //conexion a database
  console.log(queryCreator.buscarUsers);
  
  queryDatabase(queryCreator.buscarUsers, processGetUsersWEB , response);

});

function renderDatos(resultado,response){
  if (resultado.status >= 400){ 
    response.send("Error " + resultado.result);
  }
  else {
    response.render('pages/db', 
      {results: resultado.result.users });
  }
}


//COMIENZO API
function respondJson(resultado,respuesta){
  if (resultado.status >= 400){
    respuesta.send(resultado.status,resultado.result);
  }

  else {
    console.log(resultado.result);
    respuesta.status(200).jsonp(resultado.result);
  }
}

//para realizar cualquier query mando el string de query, la funcion a llamar
//cuando termine y el response para que esa funcione envie la respuesta
function queryDatabase(query, processResult, response){
  //conecto a base de datos
  pg.connect(process.env.DATABASE_URL, function(err, client, done) {
    //realizo query
    client.query(query,function(err, result){
      done();
      if (err){
        //si hubo errores al realiza query
        response.send(500,err);
        console.log("error al hacer query");  
      }
      else {
        //proceso el resultado
        processResult(result, response);
      }
      

    });
  });
}


function processGet(result, response, formater_result, err_msg, callback){
  var resultado;
  //Si no trajo nada
  if (result.rowCount == 0){
    console.log(result);
    resultado = {status:400, result: err_msg};
  } else {
    //formateo la data
    var data_formateada = formater_result(result.rows[0]);
    //la devuelvo
    resultado = { status: 200, result: data_formateada};
  }

  callback(resultado, response);
}


function processPOST(result, response, succes_msg, err_msg){
  var resultado;
  //Si no trajo nada
  if (result.rowCount == 0){
    //revisar esto
    console.log(result);
    resultado = {status:400, result: err_msg};
  } else {
    console.log(result);
    //la devuelvo
    resultado = { status:201, result: succes_msg};
  }

  response.send(resultado.status,resultado.result);
}


function processGetUser(result, response){
  processGet(result, response, formater.user, "No existe el usuario pedido", respondJson);
}

function processGetUsers(result, response){
  processGet(result, response, formater.users, "No existen usuarios", respondJson);
}

function processGetUsersWEB(result,response){
  processGet(result, response, formater.users, "No existen usuarios", renderDatos); 
}

function processGetInterest(result, response){
  processGet(result, response, formater.intereses, "No existen intereses", respondJson);
}


function processPostInterest(result,response){
  processPOST(result, response, "Interes creado con exito", "Error al crear interes");
}

function processPostUser(result,response){
  //hay que devolver usuario entero aca...
  processPOST(result, response, "Usuario creado con exito","Error al crear usuario");
}

function processPUT(result){
  //a llenar
}

function processDelete(result,response){

  console.log(result);
  if (result.rowCount == 0){
    resultado = {status:400, result: "No existe el usuario a eliminar"};
  } else {
    resultado = { status:201, result: "Elimino"};
  }
  resultado.result = result.rows[0];
  response.send(resultado.status,resultado.result);

}

//GET a /users
function getAllUsersAPI(request, response){
  queryDatabase(queryCreator.buscarUsers, processGetUsers, response);
  
}
//GET a /users/id
function getUserAPI(request,response){
  var id = request.params.id;
  queryDatabase(queryCreator.fBuscarUser(id), processGetUser, response);
}

//POST a /users
//201
function altaUserAPI(request,response){
  console.log(request.body.user);
  var user = request.body.user;
  //crea bien
  queryDatabase(queryCreator.fAltaUser(user), processPostUser, response);
  //getid
  //queryDatabase(queryCreator.fBuscarUser(id),processGetUser, response);
}

//PUT a /users/id
/*function modifyUserAPI(request,response){
  queryDatabase(queryModify,??,respondJson??,response);
}*/

//PUT a /users/id/photo
/*function putFotoAPI(request,response){
  queryDatabase(queryActualizarFoto, ?? , respond??, response);
}*/

//DELTE a /users/id
function deleteUserAPI(request,response){
  queryDatabase(queryCreator.fDeleteUser(request.params.id), processDelete,  response);
}

//GET a /interests
function getInteresesAPI(request,response){
  queryDatabase(queryCreator.buscarIntereses, processGetInterest, response);
}

//POST a /interest
//201
function postInteresAPI(request,response){
  console.log(request.body.interest);
  var interes = request.body.interest;
  queryDatabase(queryCreator.fAgregarInteres(interes), processPostInterest, response)
}

//asi hago la APIII!!!!I!I!I!I!I!
router.route('/users').get(getAllUsersAPI).post(altaUserAPI);
router.route('/users/:id').get(getUserAPI).delete(deleteUserAPI);//.put(modifyUserAPI);
router.route('/interests').get(getInteresesAPI).post(postInteresAPI);
//router.route('/users/:id/photo').put(putFotoAPI);
app.use(express.static(__dirname + '/public'),router);