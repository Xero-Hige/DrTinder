from client import *
import json

users_url = "users"
photo_url = "photo"
interests_url = "interests"

class ClientShared(Client):
	def __init__(self):
		Client.__init__(self,"http://dr-tinder.herokuapp.com")

	def request(self,method, url, body=""):
		return self.makeRequest(method,url,body,{'content-type': 'application/json'})

	def newUser(self,json_user):
		url = self.makeUrl([users_url])
		return self.request(POST,url,json.dumps(json_user))

	def modifyUser(self,json_user, id):
		url = self.makeUrl([users_url, str(id)])
		return self.request(PUT,url,json.dumps(json_user))

	def postPhoto(self,photo_64, id):
		url = self.makeUrl([users_url, str(id) ,photo_url])
		return self.request(PUT, url, json.dumps({'photo':photo_64}))

	def delete(self,id):
		url = self.makeUrl([users_url, str(id)])
		return self.request(DELETE, url)

	def getUser(self,id):
		url = self.makeUrl([users_url, str(id)])
		return self.request(GET,url)

	def getInterests(self):
		url = self.makeUrl([interests_url])
		return self.request(GET,url)

	def newInterest(self,obj_category_value):
		url = self.makeUrl([interests_url])
		return self.request(POST,url,json.dumps({'interest': obj_category_value, 'metadata': { 'count':1, 'version':'0.1' } }))
