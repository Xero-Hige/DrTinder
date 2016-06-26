var using_url_mdf =  $("[data-link]").attr('data-link');

var httpResquest = function(method, url, callback_fn, error_fn, data_to_send=""){
	
	var xhttp = new XMLHttpRequest();
		xhttp.onreadystatechange = function() {
			if (xhttp.readyState == 4){
				if ( xhttp.status == 200){
					callback_fn(xhttp.responseText);
				}
			if (xhttp.status >= 400) {
					error_fn(xhttp.responseText);
				}
			}  
		};

	xhttp.open(method, url, true);

	xhttp.setRequestHeader('Content-Type', 'application/json');
	
	xhttp.send(data_to_send);
}

var userModal= function(obj){

	var modal = $('#profile_user');
	$('#profile_mail').text(obj.mail);
	$('#profile_name').val(obj.name);
	$('#profile_alias').val(obj.alias);
	$('#profile_id').text(obj.id);
	$('#profile_age').val(obj.age);
	$('#profile_latitude').val(obj.latitude);
	$('#profile_longitude').val(obj.longitude);
	$('#profile_sex option[value="'+obj.sex+'"]').attr('selected', 'selected');
	$('#profile_interests').val(obj.interests).trigger('chosen:updated');
	$('#profile_photo').hide();
	modal.modal('show');
	
	var setPhoto = function(resp){
		var path = "data:image/jpg;base64," + JSON.parse(resp).photo_profile;
		$('#profile_photo').attr('src',path);
		$('#profile_photo').show();
	}
	var noPhoto = function(){

	}

	httpResquest('GET',using_url_mdf + '/users/' + obj.id + '/photo',setPhoto,noPhoto);
	//var path = "data:image/jpg;base64," + user.link_to_photo; 
}

$('.edit-user').click(function(e){
	//find
	var user = {};
	var row = $( e.target ).closest('tr');
	user.name = row.find('.name').text();
	user.alias = row.find('.alias').text();
	user.age = row.find('.age').text();
	user.mail = row.find('.email').text();
	user.id = row.attr('id');
	user.sex = row.find('.sex').text();
	user.link_to_photo = row.find('.photo').find('span').attr('data-value');
	
	row.find('li[data-loc]').each(function (){
		if ($(this).attr('data-loc') == 'latitude')
			user.latitude = $(this).text();
		else
			user.longitude = $(this).text();
	});

	var interest_key="";
	user.interests = [];
	row.find('.interests').children('li').each(function(){
		if (! $(this).hasClass('dropdown-header'))
			user.interests.push($(this).text());
	});

	userModal(user);
});
for (var i = 0; i < spans.length; i++){
	spans[i].addEventListener('click',showImg);
}

function succesRequest(resp){
	$('#profile_user').modal('hide');
	location.reload();
}

function failRequest(resp){
	$('#profile_user').modal('hide');
	$('#error_msg .modal-body').text(resp);
	$('#error_msg').modal('show');
}

function putUser(){
	var user = {}, location = {}, user_format = {};
	var choices = document.querySelectorAll('#profile_user .chosen-choices .search-choice span');
	
	var interests = [];
	var categories = document.querySelectorAll('#profile_interests optgroup');
	
	//por cada eleccion de interes
	for ( i = 0; i < choices.length; i++){
		var value = choices[i].textContent;

		var category = "";
		//por optgroup
		for ( var j = 0; j < categories.length; j++){
			var options = categories[j].children;
	
			//recorro las opciones
			for (var k = 0; k <options.length; k++ ){
				if (value == options[k].textContent){
					category = categories[j].getAttribute('label');
				}
			}
			if (category!="")
				break;
		}
	
		var interest = {
			category: category,
			value: value
		}

		interests.push(interest);
	}
	

	user.age = parseInt($('#profile_age').val());
	user.email = $('#profile_mail').text();
	user.name = $('#profile_name').val();
	user.alias = $('#profile_alias').val();
	user.id = parseInt($('#profile_id').text());
	location.latitude = parseFloat($('#profile_latitude').val());
	location.longitude = parseFloat($('#profile_longitude').val());
	user.location = location;
	user.interests = interests;
	user.sex = $('#profile_sex').val();
	user.photo_profile = $('#profile_photo').attr('src').replace(/^data:image\/(png|jpg|jpeg);base64,/, '');

	user_format.user = user;
	user_format.metadata = {version: "1.0"};
	
	httpResquest('PUT',using_url_mdf + "/users/" + user.id, succesRequest, failRequest, JSON.stringify(user_format));	
}

function changePhoto(){
  	var file    = document.getElementById('profile_photo_file').files[0];
  	var reader  = new FileReader();
  	$('#profile_photo').hide();
  	console.log('changing');
  	reader.onloadend = function () {
    	var photo_b64 = reader.result;
		$('#profile_photo').attr('src',photo_b64);
		$('#profile_photo').show();
  	};

  	if (file) {
    	reader.readAsDataURL(file);
  	} else {
  		$('#profile_photo').show();
  	}
}

function deleteUser(){
	var id = $('#profile_id').text();
	httpResquest('DELETE',using_url_mdf + "/users/" + id, succesRequest, failRequest);
}

$("#profile_photo_file").change( function(){
	changePhoto();
});
document.getElementById("delete_user").addEventListener('click',deleteUser);
document.getElementById("save_user").addEventListener('click',putUser);