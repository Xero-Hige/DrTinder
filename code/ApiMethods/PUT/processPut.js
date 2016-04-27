var proccess = require('../../proccess.js');
var respondSimple = require('../../processCallbacks.js').respondSimple;

var successPut = function (result){

    return {
      status: 200,
      result: "Se modifico con exito"
    };
  
}

module.exports = function (result, response, err_msg) {
  proccess(result, response, successPut, respondSimple, err_msg);
}