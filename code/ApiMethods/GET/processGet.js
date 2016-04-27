var proccess = require('../../proccess.js');
var respondJson = require('../../processCallbacks.js').respondJson;

module.exports = function (result, response, success, err_msg) {
	proccess(result, response, success, respondJson, err_msg);
}