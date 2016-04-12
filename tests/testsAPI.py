import requests
import json
import base64

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

def modifyUser(user, id):
	r = requests.put(usando + 'users/' + str(id), data= json.dumps(user), headers= headers)
	return r

def modifyFoto(photo,id):
	r = requests.put(usando + 'users/' + str(id) + '/photo', data= json.dumps(photo), headers= headers )
	return r
#print deleteUser(5).text
# print getUser(1).text
# print getUsers().text
# print getInterests().text
with open("nouser.jpg", "rb") as image_file:
    encoded_string = base64.b64encode(image_file.read())


stringUser = '{"user":{"id":1,"name":"Pedro","alias":"Copito","photo_profile":"undefined","age":"21","sex":"man","email":"pepe@usuasdario.com","interests":[{"category":"music","value":"rock"},{"category":"music/band","value":"pearl jam"},{"category":"sport","value":"tennis"}],"location":{"latitude":-1.45356,"longitude":4.51119}}}'
stringInteres = '{"interest":{"category":"music","value":"rock"}}'
justPhoto = '{"photo":"' + encoded_string + '"}';
user = json.loads(stringUser)
interest = json.loads(stringInteres)
photo = json.loads(justPhoto)
ass = modifyFoto(photo,3)
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


