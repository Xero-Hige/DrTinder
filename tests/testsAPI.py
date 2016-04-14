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
with open("pipa.jpg", "rb") as image_file:
    foto_mujer = base64.b64encode(image_file.read())

with open("pepe.jpg","rb") as image:
	foto_varon = base64.b64encode(image.read())

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
# r = modifyFoto(photo,12)
# mostar(r)

# print "Alta user varon"
# stringUser = '{"user":{"name":"Alejando","alias":"Alex","photo_profile":"'+foto_varon+'","age":"26","sex":"man","email":"elale@gmail.com","interests":[{"category":"music","value":"rock"},{"category":"music/band","value":"pearl jam"},{"category":"sport","value":"tennis"}],"location":{"latitude":-3.23,"longitude":3.1416}}}'
# user = json.loads(stringUser)
# r = postUser(user);
# mostar(r)

# print "altaInters"
# stringInteres = '{"interest":{"category":"sport","value":"rugby"}}'
# interest = json.loads(stringInteres)
# r = postInterest(interest)
# mostar(r)

# # print "deleteUserInexistente"
# # r = deleteUser(10)
# # mostar(r)

# print "modifyUser"
# pedro = '{"user":{"id":1,"name":"Pedro","alias":"Copo","photo_profile":"'+foto_varon+'","age":"21","sex":"man","email":"copito@gmail.com","interests":[{"category":"music","value":"rock"},{"category":"music/band","value":"pearl jam"},{"category":"sport","value":"tennis"}],"location":{"latitude":-9.87,"longitude":1.234}}}'
# user_2 = json.loads(pedro)
# r = modifyUser(user_2,1)
# mostar(r)


#tests
#GETS NORAMLES
#GET USER INEXISTENTE
#POST NORMALES
#POST CON PARAMETROS MAL
#PUT NORMALES
#PUT CON PARAMAETROS MAL
#DELTE, que no exista mas el user
#DELTE USUARIO INEXISTENTE


