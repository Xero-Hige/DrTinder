var exports = module.exports;

function respondSimple(resultado, respuesta) {
  respuesta.send(resultado.status, resultado.result);
}

exports.respondSimple = respondSimple;

//mal nombre TODO
exports.respondRenderDatos = function(resultado, response) {
  if (resultado.status >= 400) {
    respondSimple(resultado, respuesta);
  } else {

    response.render('pages/db', {
      results: resultado.result.users,
      intereses: resultado.result.intereses
    });
  }
}

exports.respondJson = function(resultado, respuesta) {
  if (resultado.status >= 400) {
    respondSimple(resultado, respuesta);
  } else {
    //console.log(resultado.result);
    respuesta.status(resultado.status).jsonp(resultado.result);
  }
}