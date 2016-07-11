from Clients.clientApp import ClientApp
import time
import requests

mail1= "dehotmail.com"
passw = "123456"

mail2="copitogmail.com"
passw2="123456"

myClient = ClientApp(mail1,passw)
secondClient = ClientApp(mail2,passw2)

myClient.login()
secondClient.login()
myClient.sendMessage(mail2,"asdasd")

while 1:
	secondClient.getNewMsgs(mail1)
	time.sleep(2)

