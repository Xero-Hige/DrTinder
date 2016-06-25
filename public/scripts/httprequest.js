var httpResquest = function(method,url, callback_fn, error_fn, data_to_send, headers_array){
	
	var xhttp = new XMLHttpRequest();
		xhttp.onreadystatechange = function() {
			if (xhttp.readyState == 4){
				if ( xhttp.status == 200){
					callback_fn();
				}
			if (xhttp.status >= 400) {
					error_fn();
				}
			}  
		};

	xhttp.open(method, url, true);

	headers_array.forEach(function(header){
		xhttp.setRequestHeader('Content-Type', 'application/json');
	})

	xhttp.send(data_to_send);
}