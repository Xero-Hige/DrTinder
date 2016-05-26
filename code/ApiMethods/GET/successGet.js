var formater = require('../../dataFormater');
var exports = module.exports;

function successGet(result, formater) {
	var data_formateada = formater(result.rows[0]);
	resultado = {
		status: 200,
		result: data_formateada
	};
	return resultado;
}

var successGetUsers = function(result){
	return successGet(result, formater.users);
}

exports.successGetUsers = successGetUsers;

exports.successGetInterests = function(result){
   	return successGet(result, formater.intereses);
}

exports.successGetUser = function(result){
	return successGet(result, formater.user);
}

exports.successGetUsersAndInterests = function(result){
	var formatedData = successGetUsers(result);
    var interests = formater.intereses(result.rows[1]);
   	formatedData.result["intereses"] = interests.interests;
   	return formatedData;
}

exports.successGetPhoto = function(result){
	return successGet(result, formater.photo);
}