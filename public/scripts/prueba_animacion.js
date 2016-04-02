document.getElementById('animado').addEventListener('click',function(){
	var new_user = document.getElementById('new_user');
	new_user.style.display = 'flex';
	new_user.style.zIndex = '10';
});
document.getElementById('new_user').style.display = 'none';
document.getElementById('new_user').addEventListener('click',function(e){
	if (e.target.getAttribute('id') == "new_user"){
		var new_user = document.getElementById('new_user');
		new_user.style.display = 'none';
		new_user.style.zIndex= '-1';
	}
});