var express = require('express');
var app = express();
var router = express.Router();
var bodyParser = require('body-parser');
var functions = require('./resolveQuerys');

//funciones a usar
var getUsersAPI = functions.getUsers;
var getUserAPI = functions.getUser;
var getInterestsAPI = functions.getInterests;
var postUserAPI = functions.postUser;
var postInteresAPI = functions.postInteres ;
var modifyUserAPI = functions.modifyUser;
var putFotoAPI = functions.putFoto;
var deleteUserAPI = functions.deleteUser;
var renderDatos = functions.renderDatos;


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

app.use('/db', express.static(__dirname + '/public'));
app.get('/db', function(request, response) {
  renderDatos(request, response);
});

router.route('/users').get(getUsersAPI).post(postUserAPI);
router.route('/users/:id').get(getUserAPI).delete(deleteUserAPI).put(modifyUserAPI);
router.route('/interests').get(getInterestsAPI).post(postInteresAPI);
router.route('/last').get(functions.last);
router.route('/users/:id/photo').put(putFotoAPI);
app.use(express.static(__dirname + '/public'), router);