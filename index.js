var pg = require('pg');
var express = require('express');
var app = express();
var router = express.Router();

app.set('port', (process.env.PORT || 5000));

// views is directory for all template files, dir base cuando renderea
app.set('views', __dirname + '/views');
app.set('view engine', 'ejs');

//cuando en la app voy a / realizo esto  QUIEN ES RESPONSE??
app.get('/', function(request, response) {
    response.render('pages/index');
});

//muestra puerto en donde estoy corriendo
app.listen(app.get('port'), function() {
  console.log('Node app is running on port', app.get('port'));
});

app.get('/db', function (request, response) {
  //conexion a database
  buscarUsers(function(busqueda){
    if (busqueda.status == 500){ 
      console.error(err); 
      response.send("Error " + busqueda.result);
    }
    else {
      console.log('busco y da la data');
      response.render('pages/db', {results: busqueda.result.users});
    }
  });

});

//funcion con solo request y response
function getAllUsersAPI(request, response){
	buscarUsers(function(busqueda){

    if (busqueda.status == 500){
      response.send(500,busqueda.result);
    }

    else {
      console.log(busqueda.result);
      response.status(200).jsonp(busqueda.result);
    }

  });
  
}

//seguir este formato para todo, hacer funcion que busca, pone o bla para 
function buscarUsers(callback){
  var respones = {status: '500', result:''};
  pg.connect(process.env.DATABASE_URL, function(err, client, done) {
      client.query('SELECT array_to_json(array_agg(row_to_json(test_table))) AS users FROM test_table', function(err, result){
        done();
        if (err){
           responses = {status:'500', result:err };
        }
        else{
          responses = { status:'200', result: result.rows[0]};
        }
        callback(responses);
      });
  }); 
}

//asi hago la APIII!!!!I!I!I!I!I!
router.route('/users').get(getAllUsersAPI);
//router.route('/users/:id').get(getUserAPI).put(addUserAPI).delete(deleteUserAPI);
app.use(express.static(__dirname + '/public'),router);