/*Procesar result
-response para dar la respuesta
-succes es la funcion que devuelve el resultado correcto
-callback es la fucnion que decide como dar el resultado al que lo pidio
-err_msg el error a dar en caso de que no hubo respuesta desada*/

module.exports = function(result, response, success, callback, err_msg) {
  var resultado;
  if (result.rowCount == 0) {
    resultado = {
      status: 400,
      result: err_msg
    };
  } else {
    resultado = success(result);
  }
  callback(resultado, response);
}