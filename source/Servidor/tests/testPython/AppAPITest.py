from Clients.clientApp import ClientApp

mail= "pepep123@pas.com"
passw = "secret"

myClient = ClientApp(mail,passw)

r = myClient.signup()
print("SignedUPed", r.status_code, r.text)

r = myClient.login()
print("Responded", r.status_code, r.text)

# r = myClient.get_token()
# print("Responded", r.status, r.read())

# print "Token: " + myClient.token

# r = myClient.get_new_matches()
# print("Matches", r.status, r.read())

#r = myClient.uniexistant_call()
#print("Unexistant", r.status)
#r.read()

# r = myClient.get_data()
# print("Data self", r.status, r.read())

# r = myClient.post_photo("aasaa")
# print("post photo?", r.status, r.read())

# r = myClient.get_photo()
# print("photo: ", r.status, r.read())
