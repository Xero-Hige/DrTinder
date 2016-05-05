var exports = module.exports;
var Validator = require('jsonschema').Validator;
var v = new Validator();


var metaSinCountSchema = {
	"id": "/metaSCSchema",
	"type": "object",
	"properties":{
		"version":{"type":"string", "required": "true"}
	}

}

var metaConCountSchema = {
	"id": "/metaCCSchema",
	"type": "object",
	"properties":{
		"version":{"type":"string"},
		"count": {"type": "integer"}
	},
	"required":["version","count"]

}

var locationSchema = {
	"id": "/Location",
	"type": "object",
	"properties": {
		"longitude": {"type": "number"},
		"latitude": {"type": "number"},
	},
	"required": ["longitude","latitude"]
};

var interestSchema = {
	"id": "/Interest",
	"type": "object",
	"properties":{
		"category": {"type":"string"},
		"value": {"type":"string"}
	},
	"required": ["category","value"]	
}

var _userSchema= {
  	"id": "/UserBase",
  	"type": "object",
  	"properties": {
  		"id":{"type":"integer"},
  		"name": {"type": "string"},
  		"alias": {"type": "string"},
  		"sex":{"type": "string"},
  		"email":{"type": "string"},
  		"photo_profile":{"type": "string"},
  		"age":{"type": "integer"},
  		"interests": {
        	"type": "array",
        	"items": {"$ref": "/Interest"},
        	"minItems": 1
      	},
  		"location":{"$ref": "/Location"},
  	},
  	"required":["id","name","alias","sex","email","photo_profile","age","interests","location"]
};

var userSchema = {
	"id": "/User",
	"type":"object",
	"properties": {
		"user":{"$ref":"/UserBase","required":"true"}
	}
}

var usersSchema = {
	"id": "/Users",
	"type": "object",
	"properties":{
		"users":{
			"type": "array",
			"items": {"$ref": "/User"}
		},
		"metadata":{"$ref": "/metaCCSchema"}
	},
	"required":["users","metadata"]
}

var interestsSchema = {
	"id": "/Interests",
	"type": "object",
	"properties":{
		"interests":{
			"type": "array",
			"items": {"$ref":"/Interest"}
		},
		"metadata":{"$ref": "/metaCCSchema"}
	},
	"required": ["interests","metadata"]
}

var userMetaSchema = {
	"id":"/UseraMeta",
	"type":"object",
	"properties": {
		"user":{"$ref":"/UserBase"},
		"metadata":{"$ref":"/metaSCSchema"}
	},
	"required":["user", "metadata"]
}

var interestMetaSchema = {
	"id": "/InteresMeta",
	"type": "object",
	"properties": {
		"interest": {"$ref":"/Interest"},
		"metadata": {"$ref":"/metaCCSchema"}
	},
	"required":["interest","metadata"]

}

v.addSchema(metaConCountSchema,"/metaCCSchema");
v.addSchema(metaSinCountSchema,"/metaSCSchema");
v.addSchema(locationSchema,"/Location");
v.addSchema(_userSchema, "/UserBase");
v.addSchema(interestSchema, "/Interest");
v.addSchema(userSchema, "/User");

function errors(object,schema){
	var err = v.validate(object,schema).errors;
	var msgs = [];
	for (var i = 0; i < err.length; i++){
		var err_msg = "";
		err_msg+= err[i].property.replace("instance.","") + " ";
		err_msg+= err[i].message;
		msgs.push(err_msg);
	}
	return msgs;
}

exports.validateUsers = function(users) {
	return errors(users, usersSchema);
}
exports.validateInterests = function(interests){
	return errors(interests,interests);
}
exports.validateInterestSinMeta = function(interest){
	return errors(interest,interestSchema);
}
exports.validateInterest = function(interest){
	return errors(interest,interestMetaSchema);
}
exports.validateUser = function (user) {
	return errors(user,userMetaSchema);
}



/*
var prueba_interes = {"category":"sport","value":"surf"};
var prueba_interes2 = {"category":"sport"};
var prueba_interes3 = {"value":"surf"};
var prueba_interests = {"interests": [prueba_interes,prueba_interes],"metadata":
	{ "version":"1.0",
	   "count":2	
		}
	}
var prueba_interesM = {"interest":{"category":"sport","value":"surf"},
	"metadata":{
		"version":"1.0",
		"count":1
	}};
var prueba_location = {"latitude":-1.456,"longitude":1.4623};
var prueba_location2 = {"latitude":"-1.456","longitude":"1.456"};
var prueba_user = {"user":{"id":"21","name":"Karina","alias":"Kalista","photo_profile":"foto_mujer","age":24,"sex":"woman","email":"escudo@gmail.com","interests":[{"category":"music","value":"rock"},{"category":"music/band","value":"pearl jam"},{"category":"sport","value":"tennis"}],"location":{"latitude":-3.23,"longitude":1.1416}}}
var prueba_userM = {"user":{"id":21,"name":"Karina","alias":"Kalista","photo_profile":"foto_mujer","age":24,"sex":"woman","email":"escudo@gmail.com","interests":[{"category":"music","value":"rock"},{"category":"music/band","value":"pearl jam"},{"category":"sport","value":"tennis"}],"location":{"latitude":-3.23,"longitude":1.1416}},
"metadata":{
	"version":"1.0"
}
}
var prueba_users = {"users":[prueba_user,prueba_user],
	"metadata":{
		"version":"1.0",
		"count":2
	}}

console.log(1);
console.log(validateInterest(prueba_interes));
console.log(2);
console.log(validateInterests(prueba_interests));
console.log(3)
console.log(validateUser(prueba_user));
console.log(4);
console.log(validateUsers(prueba_users));
console.log(5);
console.log(validateUser(prueba_userM));
console.log(6);
console.log(validateInterest(prueba_interesM));
console.log(7);
console.log(validateInterest(prueba_interes2));*/