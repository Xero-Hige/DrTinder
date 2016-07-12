from client import *


user_url = "user"
users_url = "users"
token_url = "token"
photo_url = "photo"
chat_url = "chats"
interest_url="interest"
new_url = "new"
matches_url = "matches"
id_url = "id"


class ClientApp(Client):
	def __init__(self,mail,passw):
		Client.__init__(self,"http://127.0.0.1:8000")
		self.mail = mail
		self.token = ""
		self.passw = passw
	
	def getData(self, mail=""):
		if mail == "":
			mail = self.mail
		url = self.makeUrl([user_url], {"token":self.token, "res_id":mail})
		return self.makeRequest(GET,url)

	def getMatches(self):
		url= self.makeUrl([users_url], {"token": self.token})
		return self.makeRequest(GET,url)

	def login(self, mail="",passw=""):
		if mail == "":
			mail = self.mail
		if passw == "":
			passw = self.passw
		url=self.makeUrl([user_url])
		header = {"Authorization":"username=\""+mail+"\" pass=\""+ passw +"\""}
		r = self.makeRequest(POST,url,"localization=1.536,-1.2356",header)
		self.token = r.text
		return r

	def signup(self,user_data=""):
		if user_data == "":
			user_data = "\"GastonSna\",\"23\",\"alias\",\""+self.mail+"\",\"man\",\"Pickachu\",\"sport::tennis||food::meat\""
		url=self.makeUrl([users_url])
		header = {"Authorization":"username=\""+self.mail+"\" pass=\""+self.passw+"\""}
		body = "User=" + user_data
		return self.makeRequest(POST,url,body,header)

	def getPhoto(self, mail=""):
		if mail == "":
			mail=self.mail
		params={"token":self.token, "res_id": mail}
		url=self.makeUrl([users_url,photo_url], params)
		return self.makeRequest(GET, url)

	def friends(self):
		url= self.makeUrl([chat_url], {"token": self.token})
		return self.makeRequest(GET,url)

	def chatFriend(self,id):
		url = self.makeUrl([chat_url],{"token": self.token, "res_id": id})
		return self.makeRequest(GET,url)

	def modifyData(self, string_put_user_csv):
		body="User="+string_put_user_csv
		params = {"token": self.token}
		url = self.makeUrl([users_url], params)
		return self.makeRequest(PUT,url,body)

	def postPhoto(self,photo):
		body = photo
		params = {"token": self.token}
		url=self.makeUrl([users_url,photo_url], params)
		return self.makeRequest(POST,url,body)

	def delete(self):
		url = self.makeUrl([users_url], {"token":self.token})
		return self.makeRequest(DELETE,url)

	def uniexistantCall(self):
		url = self.makeUrl(["noExiste","Nono"])
		return self.makeRequest(GET,url)

	def likeUser(self,user):
		params = {"token": self.token}
		body = "user_id=" + user + "&bool=true" 
		url = self.makeUrl([matches_url], params)
		return self.makeRequest(POST,url,body)

	def dislikeUser(self,user):
		params = {"token": self.token}
		body = "user_id=" + user + "&bool=false" 
		url = self.makeUrl([matches_url], params)
		return self.makeRequest(POST,url,body)

	def getNewMatches(self):
		params = {"token": self.token}
		url = self.makeUrl([matches_url], params)
		return self.makeRequest(GET,url)

	def sendMessage(self,user,msg="hola"):
		params = {"token": self.token}
		url = self.makeUrl([chat_url], params)
		body= "user_id=" + user + "&msg=" + msg 
		return self.makeRequest(POST,url,body)

	def getNewMsgs(self,user):
		params = {"token": self.token, "res_id": user}
		url = self.makeUrl([chat_url, new_url], params)
		return self.makeRequest(GET,url)

	def getInterestPhoto(self,int_id):
		params = {"token": self.token, "res_id": int_id}
		url = self.makeUrl([interest_url], params)
		return self.makeRequest(GET,url)