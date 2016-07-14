var using_url =  $("[data-link]").attr('data-link');

function valid(text,type){
	var regex;
	switch (type){
		case 'novacio':
		return text !== '';
		break;
		case 'email':
		regex = /(?:[a-z0-9!#$%&'*+/=?^_`{|}~-]+(?:\.[a-z0-9!#$%&'*+/=?^_`{|}~-]+)*|"(?:[\x01-\x08\x0b\x0c\x0e-\x1f\x21\x23-\x5b\x5d-\x7f]|\\[\x01-\x09\x0b\x0c\x0e-\x7f])*")@(?:(?:[a-z0-9](?:[a-z0-9-]*[a-z0-9])?\.)+[a-z0-9](?:[a-z0-9-]*[a-z0-9])?|\[(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?|[a-z0-9-]*[a-z0-9]:(?:[\x01-\x08\x0b\x0c\x0e-\x1f\x21-\x5a\x53-\x7f]|\\[\x01-\x09\x0b\x0c\x0e-\x7f])+)\])/;
		break;
		case 'age':
		regex = /^[0-9]+$/;
		break;
		case 'sex':
		regex = /^(man|woman)$/;
		break;
		case 'float':
		regex = /^[-+]?[0-9]*\.?[0-9]+$/;
		break;
		default:
		return true;
	}
	return regex.test(text);
}

function validate(text, validation_types){
	for (var i = 0; i < validation_types.length; i ++){
		if (!valid(text, validation_types[i])){
			return false;
		}	
	}
	return true;
}

function getCategory(interest){
	interest = interest.previousSibling;
	while ( interest.className != 'group-result' ){
		interest = interest.previousSibling;
	}
	return interest.textContent;

}

function postUser(user){
	var xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function() {
		if (xhttp.readyState == 4){
			if ( xhttp.status == 201){
				var postedUser = JSON.parse(xhttp.responseText)["user"];
				putPhoto(postedUser["id"], user.user.photo_profile);
			}
			if (xhttp.status >= 400) {
				$("#err_msg").text("ERR "+ xhttp.responseText);
			}
		}  
	};
	//http://
	user["metadata"] = { version:"0.1" };
	xhttp.open("POST", using_url + "/users", true);
	xhttp.setRequestHeader('Content-Type', 'application/json');
	xhttp.send(JSON.stringify(user));
}

function putPhoto(id,photo){
	var xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function() {
		if (xhttp.readyState == 4){
			if ( xhttp.status == 200){
				$("#new_user").modal("hide");
				location.reload();
			}
			if (xhttp.status >= 400) {
				$("#err_msg").text("ERR "+ xhttp.responseText);
			}
		}  
	};
	//http://

	var send = {photo: photo};
	xhttp.open("PUT", using_url + "/users/" + id + "/photo", true);
	xhttp.setRequestHeader('Content-Type', 'application/json');
	xhttp.send(JSON.stringify(send));
}

function decideToPost(user,keys_mal){
	if (keys_mal.length > 0){
		var keys_error = '';
		for (i = 0; i < keys_mal.length; i++){
			keys_error += keys_mal[i] + " ";
		}
		$('#err_msg').text("Error en formato de : " + keys_error);
	}else{
		document.getElementById('err_msg').textContent = '';
		postUser({user: user});

	}
}

function _finishUser(user, keys_mal) {
  var file    = document.querySelector('#photo_file').files[0];
  var reader  = new FileReader();

  reader.onloadend = function () {
    var photo_b64 = reader.result;
	photo_b64 = photo_b64.replace(/^data:image\/(png|jpg|jpeg);base64,/, '');
	console.log(photo_b64.substring(0,20));
	user['photo_profile'] = photo_b64;
	decideToPost(user,keys_mal);
  };

  if (file) {
    reader.readAsDataURL(file);
  } else {
    keys_mal.push('photo_profile');
    decideToPost(user,keys_mal);
  }
}

function _createUser(basic_info, location_list, interests_values){
	var user = {};
	var err = false;
	var msg = "";
	var keys_mal = []
	//alias,name,etc
	for (var i = 0; i < basic_info.length; i++){
		var key = basic_info[i].querySelector('span').textContent;
		var value;
		if (key == 'sex'){
			value = basic_info[i].querySelector('select').value
		}else{
			value = basic_info[i].querySelector('input').value;
		}

		if ( key == 'age'){
			user[key] = parseInt(value);
		}else{
			user[key] = value;
		}
		

		if (!(validate(value,['novacio',key]))){
			keys_mal.push(key);
		}
	}
	//location
	var location = {}
	for (i = 0; i < location_list.length; i++){
		var x = location_list[i].getAttribute('placeholder');
		var value = location_list[i].value;
		location[x] = parseFloat(value);
		if (!(validate(value,['novacio','float']))){
			keys_mal.push(x);
		}
	}
	user['location'] = location;
	//interests
	var interests = [];
	if (interests_values.length == 0){
		keys_mal.push("intereses");
	}
	for ( i = 0; i < interests_values.length; i++){
		var value = interests_values[i].textContent;
		var category = getCategory(interests_values[i]);
		var interest = {
			category: category,
			value: value
		}
		interests.push(interest);
	}
	user['interests'] = interests;
	//photo
	_finishUser(user,keys_mal); 	
}

function crearUsuario(){
	var interests_values = document.querySelectorAll('#new_user .chosen-results .result-selected');
	var basic_info = document.getElementById('new_user').querySelectorAll('.basic_info');
	var location = document.getElementById('location_crear').querySelectorAll('input');
	_createUser(basic_info, location, interests_values);	
}



function showImg(e){
	var link;
	var xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function() {
		if (xhttp.readyState == 4){
			if ( xhttp.status == 200){
				var path = "data:image/jpg;base64," + JSON.parse(xhttp.responseText).photo_profile; 
				$('#img_a_buscar').attr("src", path);
				$('#img_user').modal("show");			
			}
			if (xhttp.status >= 400) {
				var path = "https://image.freepik.com/free-icon/male-user-shadow_318-34042.png";
				$('#img_a_buscar').attr("src", path);
				$('#img_user').modal("show");
			}
		}  
	};
	var url = e.target.getAttribute('data-value');
	var id = $(e.target).closest('tr').attr('id');
	xhttp.open("GET", using_url + '/users/' + id + '/photo', true);
	xhttp.setRequestHeader('Content-Type', 'application/json');
	xhttp.send();
	
}

//inicializo cosas
$(".chosen-select").chosen({no_results_text: "Nothing found!", allow_single_deselect: true, disable_search_threshold: 5, width:"100%" });
$('.chosen-drop').css({minWidth: '100%', width: 'auto'});

var spans = document.querySelectorAll('span[data-value]');
for (var i = 0; i < spans.length; i++){
	spans[i].addEventListener('click',showImg);
}

document.getElementById("crear_user").addEventListener('click',crearUsuario);