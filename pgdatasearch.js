 module.exports = function(){

 	function requestDataBase (queryprocess){
		pg.connect(process.env.DATABASE_URL,queryprocess);
	}
	
	function queryUsers(callback){
		client.query('SELECT array_to_json(array_agg(row_to_json(test_table))) AS users FROM test_table', function(err, result){
	        done();
	        var respones = {status: '500', result:''};
	        if (err){
	           responses = {status:'500', result:err };
	        }
	        else{
	          console.log (result);
	          responses = { status:'200', result: result.rows[0]};
	        }
	        callback(responses);
	      });
	}

	function queryUserId(id,callback){
		client.query()
	}

	

	function buscarUsers(callback){
	  requestDataBase(queryUsers(callback));  
	}

	function buscarUserPorId(id, callback){
		requestDataBase(queryUserId(id,callback));
	}
	
	function modificarUsuario(user_modificado, callback){
		requestDataBase(queryModifyUser(user_modificado, callback));
	}

	function agregarUsuario(jsonUser,callback){
		requestDataBase(queryput(jsonUser,callback));
	}

	function sacarUsuario(id,callback){
		requestDataBase(queryDelete(id,callback));
	}


	function buscarIntereses(callback){
		requestDataBase(queryIntereses(callback));
	}

	function agregarInteres(interes,callback){
		requestDataBase(queryPutInteres(interes,callback));
	}

	function buscarFoto(id,callback){
		requestDataBase(queryFoto(id, callback));
	}

	function actualizarFoto(id,callback){
		requestDataBase(queryModifyUserFoto(id, callback));
	}

	return {
		buscarUsers: buscarUsers,
		buscarUserPorId: buscarUserPorId,
		buscarIntereses: buscarIntereses,
		agregarInteres: agregarInteres,
		agregarUsuario: agregarUsuario,
		modificarUsuario: modificarUsuario,
		sacarUsuario: sacarUsuario,
		buscarFoto: buscarFoto,
		actualizarFoto: actualizarFoto 

	}


 }();