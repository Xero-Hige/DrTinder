from client import Client

mail= "pepep123@pas.com"
passw = "secret"


myClient = Client(mail,passw)

#r = myClient.signup()
#print("Responded", r.status)
# r.read()
r = myClient.login()
print("Responded", r.read())

r = myClient.get_token()
print("Responded", r.read())

print myClient.token
r = myClient.get_new_matches()
print("Matches", r.read())

# r = myClient.uniexistant_call()
# print("Unexistant", r.status)
# r.read()

r = myClient.get_data()
print("Data self", r.read())

r= myClient.post_photo("aasaa")
print("post photo?", r.status)

r = myClient.get_photo()
print("photo: ", r.read())

myClient.s.close()