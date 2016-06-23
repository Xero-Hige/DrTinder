import httplib
#HttpClient persistant
GET="GET"
PUT="PUT"
POST="POST"
DELETE="DELETE"

user_url = "user"
users_url = "users"
token_url = "token"
photo_url = "photo"
chat_url = "chats"
id_url = "id"


class Client:
	def __init__(self,mail,passw):
		self.mail = mail
		self.passw = passw
		self.token = ""
		self.s = httplib.HTTPConnection("127.0.0.1",8000);

	def makeUrl(self, lista, params={}):
		new_url = ""
		for i in range(len(lista)):
			new_url = new_url +"/"+ lista[i] 
		params_url = ""
		for key in params:
			params_url = params_url + key+ "=" + params[key] + "&"
		if params_url != "":
			new_url = new_url + "?" + params_url
		return new_url


	def makeRequest(self,method, url, body="", headers={}):
		print("Method: " + str(method))
		print("Url: " + url)
		if body != "":
			print("In Body: " + body)
		if headers != {}:
			print("With Header:")
			print(headers)

		self.s.request( method, url, body, headers)
		return self.s.getresponse()

	def get_token(self):
		url = self.makeUrl([user_url,token_url,id_url])
		r = self.makeRequest(GET,url)
		self.token = r.read()
		return r

	def get_data(self, mail=""):
		if mail == "":
			mail = self.mail
		url = self.makeUrl([user_url], {"token":self.token, "res_id":mail})
		return self.makeRequest(GET,url)

	def get_new_matches(self):
		url= self.makeUrl([users_url], {"token": self.token})
		return self.makeRequest(GET,url)

	def login(self, passw=""):
		if passw == "":
			passw = self.passw
		url=self.makeUrl([user_url])
		header = {"Authorization":"username=\""+self.mail+"\" pass=\""+ passw +"\""}
		return self.makeRequest(POST,url,"localization=1.536,-1.2356",header)

	def signup(self):
		url=self.makeUrl([users_url])
		header = {"Authorization":"username=\""+self.mail+"\" pass=\""+self.passw+"\""}
		body = "User=\"Nombre\",\"25\",\"Alias\",\""+self.mail+"\",\"man\",\"Pickachu\",\"sport::tennis\""
		return self.makeRequest(POST,url,body,header)

	def get_photo(self, mail):
		params={"token":self.token, "res_id": mail}
		url=self.makeUrl([users_url,photo_url], params)
		return self.makeRequest(GET, url)

	def friends(self):
		url= self.makeUrl([chat_url])
		return self.makeRequest(GET,url,{"token": self.token})

	def chat_with_friend(self,id):
		url = self.makeUrl([chat_url])
		return self.makeRequest(GET,url,{"token": self.token, "res_id": id})

	def modify_data(self, string_put_user_csv):
		body="User="+string_put_user_csv
		params = {"token": self.token}
		url = self.makeUrl([users_url], params)
		return self.makeRequest(PUT,url,body)

	def post_photo(self,phot):
		body = photo
		params = {"token": self.token}
		url=self.makeUrl([users_url,photo_url], params)
		return self.makeRequest(POST,url,body)

	def delete(self):
		url = self.makeUrl([users_url], {"token":self.token})
		return self.makeRequest(DELETE,url)

	def uniexistant_call(self):
		url = self.makeUrl(["noExiste","Nono"])
		return self.makeRequest(GET,url)