import requests
import json

#LOCAL
local = "http://localhost:5000/"
#NO LOCAL
noLocal = "http://dr-tinder.herokuapp.com/"

usando = local

def getUser(id):
	r = requests.get(usando +'users/' + str(id) )
	return r

def getUsers():
	r = requests.get(usando + 'users')
	return r

def getInterests():
	r = requests.get(usando + 'interests')
	return r

def postUser():
	r = requests.post( usando + 'users',) #En que formato se lo paso? Body,query??
	return r

def postInterest(param):
	r = requests.post(usando + 'interests',)#idem
	return r

def deleteUser(id):
	r = requests.delete(usando + 'users/' + str(id) )
	return r

print deleteUser(5).text
print getUser(1).text
print getUsers().text
print getInterests().text

#tests
#GETS NORAMLES
#GET USER INEXISTENTE
#POST NORMALES
#POST CON PARAMETROS MAL
#PUT NORMALES
#PUT CON PARAMAETROS MAL
#DELTE, que no exista mas el user
#DELTE USUARIO INEXISTENTE


