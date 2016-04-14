
function showImg(e){
	var data = e.target.getAttribute("data-value");
	var path = "data:image/jpg;base64," + data; 
    $('#img_a_buscar').attr("src", path);
    $('#img_user').modal("show");
}

var spans = document.querySelectorAll('span[data-value]');
for (var i = 0; i < spans.length; i++){
	spans[i].addEventListener('click',showImg);
}
