var ver = document.querySelectorAll('[data-img]');
for (var i = 0; i < ver.length; i++){
	ver[i].addEventListener('click',function(e){
		var name = e.target.getAttribute('data-img');
		console.log(e.target);
		$('#img_a_buscar').attr("src", "url(/db/photos/" + name + ")");
		$('#img_user').modal("show");
	});
}
