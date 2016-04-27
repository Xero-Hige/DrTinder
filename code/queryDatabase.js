var pg = require('pg');
var malFormato = "Error en formato";

/*Para hacer cualequier query le tengo que pasar:
- Un query string
- como procesar el resultado
- la response para que el proceso del resultado pueda dar una respuesta*/

module.exports = function(query, processResult, response) {
  pg.connect(process.env.DATABASE_URL, function(err, client, done) {
    
    if (err) {
    	done();
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



