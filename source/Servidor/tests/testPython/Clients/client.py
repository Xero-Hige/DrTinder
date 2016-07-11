import requests
#HttpClient persistant
GET="GET"
PUT="PUT"
POST="POST"
DELETE="DELETE"
SHOW_MAX_BODY = 200

class Client:
	def __init__(self,base_url):
		self.base_url = base_url

	def makeUrl(self, lista, params={}):
		new_url = self.base_url
		for i in range(len(lista)):
			new_url = new_url +"/"+ lista[i] 
		params_url = ""
		for key in params:
			params_url = params_url + key+ "=" + params[key] + "&"
		if params_url != "":
			new_url = new_url + "?" + params_url
		return new_url


	def makeRequest(self,method, url, body="", headers={}):

		print "\n========REQUEST============================"
		print("Method: " + str(method))
		print("Url: " + url)
		if body != "":
			print("In Body: " + body)
		if (headers != {}) :
			print("With Header: " + str(headers))


		if method == GET:
			r = requests.get(url,data=body,headers=headers)
		if method == POST:
			r = requests.post(url,data=body,headers=headers)
		if method == PUT:
			r = requests.put(url,data=body,headers=headers)
		if method == DELETE:
			r = requests.delete(url,data=body,headers=headers)


		print "========RESPONSE============================"
		print "Status: " + str(r.status_code)
		if len(r.text) <= SHOW_MAX_BODY:
			print "Body: " + r.text
		else:
			print "Body: " + r.text[:SHOW_MAX_BODY -1 ] + "..."
		print "============================================"
		
		return r