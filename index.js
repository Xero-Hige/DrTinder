var express = require('express');
var app = express();
var router = express.Router();
var bodyParser = require('body-parser');
var validateInterest = require('./code/validator.js').validateInterest;
var validateInterests = require('./code/validator.js').validateInterests;
var validateUser = require('./code/validator.js').validateUser;
var validateUsers = require('./code/validator.js').validateUsers;
var interestPhoto = require('./code/interestPhotos/getInterestPhoto.js');
var env = require('node-env-file');

//funciones a usar en API y la pagina
var _APIdir = './code/ApiMethods/';
var _render = './code/WebMethods/RENDER/datos_tabla.js';
var _get = _APIdir + 'GET/';
var _post = _APIdir + 'POST/';
var _put = _APIdir + 'PUT/';
var _del = _APIdir + 'DELETE/';
var _user = 'user.js';
var _interest = 'interest.js';
var _userS = 'users.js';
var _interestS = 'interests.js';
var _lastUser = 'lastUser.js';
var _photo = 'photo.js';
var lastUser = require( _get + _lastUser);
var getUsersAPI = require( _get + _userS);
var getUserAPI = require( _get + _user);
var getInterestsAPI = require( _get + _interestS);
var postUserAPI = require( _post + _user);
var postInteresAPI = require( _post + _interest);
var modifyUserAPI = require( _put + _user);
var putFotoAPI = require(_put + _photo);
var deleteUserAPI = require( _del + _user);
var getPhoto = require( _get + _photo);
var renderDatos = require ( _render );
try{
	env(__dirname + '/.env');
}catch(error){
	console.log("no .env file");
}
//para recibir json
app.use(bodyParser.json({limit: '50mb'}));
var port = (process.env.PORT || 5000);
//set port
app.set('port', port);

var url_localhost = "http://localhost:" + port;
var url_web = "http://dr-tinder.herokuapp.com";

var using_url;

if (process.argv[2] == '-l' || process.argv[2] == '--local'){
	using_url = url_localhost;
}else{
	using_url = url_web;
}
// views is directory for all template files, base cuando renderea
app.set('views', __dirname + '/views');
app.set('view engine', 'ejs');

//cuando en la app voy a / realizo esto
app.get('/', function(request, response) {
  response.render('pages/index');
});

//muestra puerto en donde estoy corriendo
var server = app.listen(app.get('port'), function() {
  console.log('Node app is running on port', app.get('port'));
});

//seteo para acceder a la carpeta public directo desde /db
app.use('/db', express.static(__dirname + '/public'));

//cuando accedo a /db
app.get('/db', function(request, response) {
  renderDatos(request, response, using_url);
});

//set API
router.route('/users').get(getUsersAPI).post(postUserAPI);
router.route('/users/:id').get(getUserAPI).delete(deleteUserAPI).put(modifyUserAPI);
router.route('/interests').get(getInterestsAPI).post(postInteresAPI);
router.route('/users/:id/photo').put(putFotoAPI).get(getPhoto);
//mio propio
router.route('/last').get(lastUser);
router.route('/interests/:category/photo').get(interestPhoto);

app.use(express.static(__dirname + '/public'), router);

var closeApp = function(){
	server.close();
	console.log("Exiting");
	process.exit(0);
}

//catches ctrl+c event
process.on('SIGINT', closeApp);