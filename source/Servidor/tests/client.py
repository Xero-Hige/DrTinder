import requests

GET=0
PUT=1
POST=2
DELETE=3

user_url = "user"
users_url = "users"
token_url = "token"
photo_url = "photo"
chat_url = "chats"


class Client:
	def __init__(self,url,mail,passw):
		self.mail = mail
		self.url = url
		self.passw = passw
		self.token = ""

	def makeUrl(self, lista):
		new_url = self.url
		for i in range(len(lista)):
			new_url = new_url + "/" + lista[i]
		return new_url

	def makeRequest(self,method, url, params={}, body="", headers={}):
		print("Method: " + str(method))
		print("Url: " + url)
		if params != {}:
			print("With params:")
			print(params)
		if body != "":
			print("In Body: " + body)
		if headers != {}:
			print("With Header:")
			print(headers)

		# if method == GET:
		# 	r = requests.get( url, data=body, params=params)
		# elif method == POST:
		# 	r = requests.post( url, data=body, params=params, headers=headers)
		# elif method == PUT:
		# 	r = requests.put( url, data=body, params=params)
		# elif method == DELETE:
		# 	r = requests.delete( url, data=body, params=params)

		return 1

	def get_token(self):
		url = self.makeUrl([user_url,token_url,self.mail])
		r = self.makeRequest(GET,url)
		self.token = 15#r.text
		return r

	def get_data(self, mail):
		url = self.makeUrl([user_url])
		return self.makeRequest(GET,url,{"token":self.token, "res_id":self.mail})

	def get_new_matches(self):
		url= self.makeUrl([users_url])
		return self.makeRequest(GET,url,{"token": self.token})

	def login(self, passw=""):
		if passw == "":
			passw = self.passw
		url=self.makeUrl([user_url])
		header = {"Authorization":"username=\""+self.mail+"\" pass=\""+ passw +"\""}
		return self.makeRequest(POST,url,{},"1.536,-1.2356",header)

	def signup(self):
		url=self.makeUrl([users_url])
		header = {"Authorization":"username=\""+self.mail+"\" pass=\""+self.passw+"\""}
		body = "User=\"Nombre\",\"25\",\"Alias\",\""+self.mail+"\",\"man\",\"Pickachu\",\"sport::tennis\""
		return self.makeRequest(POST,url,{},body,header)

	def get_photo(self, mail):
		url=self.makeUrl([users_url,photo_url])
		params={"token":self.token, "res_id": mail}
		return self.makeRequest(GET, url, params)

	def friends(self):
		url= self.makeUrl([chat_url])
		return self.makeRequest(GET,url,{"token": self.token})

	def chat_with_friend(self,id):
		url = self.makeUrl([chat_url])
		return self.makeRequest(GET,url,{"token": self.token, "res_id": id})

	def modify_data(self, string_put_user_csv):
		url = self.makeUrl([users_url])
		body="User="+string_put_user_csv
		params = {"token": self.token}
		return self.makeRequest(PUT,url,params,body)

	def delete(self):
		url = self.makeUrl([users_url])
		return self.makeRequest(DELETE,url,{"token":self.token})

	def uniexistant_call(self):
		url = self.makeUrl(["noExiste","Nono"])
		return self.makeRequest(GET,url)