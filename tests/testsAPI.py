import requests
import json
import base64

headers = {'content-type': 'application/json'}

local = "http://localhost:5000/"

noLocal = "http://dr-tinder.herokuapp.com/"

_http = local

def getUser(id):
	r = requests.get(_http +'users/' + str(id) )
	return r

def getUsers():
	r = requests.get(_http + 'users')
	return r

def getInterests():
	r = requests.get(_http + 'interests')
	return r

def postUser(param):
	print "entro"
	r = requests.post( _http + 'users', data=json.dumps(param), headers=headers) #En que formato se lo paso? Body,query??
	return r

def postInterest(param):
	r = requests.post(_http + 'interests', data=json.dumps(param), headers=headers)#idem
	return r

def deleteUser(id):
	r = requests.delete(_http + 'users/' + str(id) )
	return r

def modifyUser(user, id):
	r = requests.put(_http + 'users/' + str(id), data= json.dumps(user), headers= headers)
	return r

def modifyFoto(photo,id):
	r = requests.put(_http + 'users/' + str(id) + '/photo', data= json.dumps(photo), headers= headers )
	return r
#print deleteUser(5).text
# print getUser(1).text
# print getUsers().text
# print getInterests().text
with open("woman.jpg", "rb") as image_file:
    foto_mujer = base64.b64encode(image_file.read())

# with open("man.jpg","rb") as image:
# 	foto_varon = base64.b64encode(image.read())

# with open("man2.jpg","rb") as img:
# 	foto_varon2 = base64.b64encode(img.read())

def mostar(r):
	print r.status_code
	print r.text

# print "GetUsers"
# r = getUsers()
# mostar(r)

# print "getUserExistemte"
# r = getUser(1)
# mostar(r)

# print "getUserInexistente"
# r = getUser(159)
# mostar(r)

# print "Getintereses"
# r = getInterests()
# mostar(r)

# print "PutFotoEn"
# justPhoto = '{"photo":"'+ foto_mujer +'"}';
# photo = json.loads(justPhoto)
# r = modifyFoto(photo,1)
# mostar(r)

# print "Alta user mujer"
# stringUser = '{"user":{"id":21,"name":"Karina","alias":"Kalista","photo_profile":"'+foto_mujer+'","age":24,"sex":"woman","email":"escudo@gmail.com","interests":[{"category":"music","value":"rock"},{"category":"music/band","value":"pearl jam"},{"category":"sport","value":"tennis"}],"location":{"latitude":-3.23,"longitude":1.1416}},"metadata":{"version":"1.0"}}'
# user = json.loads(stringUser)
# r = postUser(user);
# mostar(r)

# print "altaInters"
# stringInteres = '{"interest":{"category":"sport","value":"surf"}}'
# interest = json.loads(stringInteres)
# r = postInterest(interest)
# mostar(r)

# print "deleteUserInexistente"
# r = deleteUser(10)
# mostar(r)

# print "modifyUser"
# stringUser = '{"user":{"id":21,"name":"Karina","alias":"Kalista","photo_profile":"'+foto_mujer+'","age":24,"sex":"woman","email":"escudo@gmail.com","interests":[{"category":"music","value":"rock"},{"category":"music/band","value":"pearl jam"},{"category":"sport","value":"tennis"}],"location":{"latitude":-3.23,"longitude":1.1416}},"metadata":{"version":"1.0"}}'
# user_2 = json.loads(stringUser)
# r = modifyUser(user_2,21)
# mostar(r)


#tests TODO
#GETS NORAMLES
#GET USER INEXISTENTE
#POST NORMALES
#POST CON PARAMETROS MAL
#PUT NORMALES
#PUT CON PARAMAETROS MAL
#DELTE todo ok
#DELETE y que no exista mas el user
#DELTE USUARIO INEXISTENTE


