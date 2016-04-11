import requests
import json

headers = {'content-type': 'application/json'}
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

def postUser(param):
	print "entro"
	r = requests.post( usando + 'users', data=json.dumps(param), headers=headers) #En que formato se lo paso? Body,query??
	return r

def postInterest(param):
	r = requests.post(usando + 'interests', data=json.dumps(param), headers=headers)#idem
	return r

def deleteUser(id):
	r = requests.delete(usando + 'users/' + str(id) )
	return r

#print deleteUser(5).text
# print getUser(1).text
# print getUsers().text
# print getInterests().text
stringUser = '{"user":{"name":"Juana","alias":"Junana","age":"23","sex":"woman","email":"asd@usuasdario.com","interests":[{"category":"music/band","value":"radiohead"},{"category":"music/band","value":"pearl jam"},{"category":"outdoors","value":"running"}],"location":{"latitude":-121.45356,"longitude":46.51119}}}'
stringInteres = '{"interest":{"category":"music","value":"rock"}}'
user = json.loads(stringUser)
interest = json.loads(stringInteres)
ass = postUser(user)
#rI = postInterest(interest)
print ass.text


#tests
#GETS NORAMLES
#GET USER INEXISTENTE
#POST NORMALES
#POST CON PARAMETROS MAL
#PUT NORMALES
#PUT CON PARAMAETROS MAL
#DELTE, que no exista mas el user
#DELTE USUARIO INEXISTENTE


