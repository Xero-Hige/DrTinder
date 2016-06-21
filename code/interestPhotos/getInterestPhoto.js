var fs = require('fs');

// function to encode file data to base64 encoded string
function base64_encode(file) {
    
    try{
      // read binary data
      var bitmap = fs.readFileSync(file);
      // convert binary data to base64 encoded string
      return new Buffer(bitmap).toString('base64');

    }catch (error){
      return "";
    }
}

function getInterstPhoto(category) {
  var file = "code/interestPhotos/"+category+".jpg";
  return base64_encode(file);
}

module.exports = function(request,response){
  var res = getInterstPhoto(request.params.category);
  if (res == ""){
    response.status(400).send("No existe la img de esa categoria");
  }else{
    response.status(200).send(res);
  }
}