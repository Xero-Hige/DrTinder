from Clients.clientApp import ClientApp
import unittest

mail= "pepep123@pas.com"
passw = "secret"

myClient = ClientApp(mail,passw)

class MyTest(unittest.TestCase):

	def test_1(self):
		r = myClient.signup()
		r = myClient.login()
		r = myClient.getData()
		r = myClient.delete()
		r = myClient.delete()	

	def test_2(self):
		r = myClient.signup()
		r = myClient.login()
		r = myClient.postPhoto("aasaa")
		r = myClient.getPhoto()
		r = myClient.delete()
		self.assertTrue(1)

	def test_3(self):
		r = myClient.signup()
		r = myClient.login()
		r = myClient.uniexistantCall()
		r = myClient.delete()
		self.assertTrue(1)

	def test_4(self):
		r = myClient.signup()
		r = myClient.login()
		r = myClient.getNewMatches()
		r = myClient.delete()
		self.assertTrue(1)

	def test_5(self):
		r = myClient.signup()
		r = myClient.login()
		r = myClient.modifyData('"Nombre","25","Alias22","pepep123@pas.com","man","Pickachu","sport::tennis"')
		r = myClient.getData()
		r = myClient.delete()
		self.assertTrue(1)

if __name__ == '__main__':
	unittest.main()

# print "Token: " + myClient.token
