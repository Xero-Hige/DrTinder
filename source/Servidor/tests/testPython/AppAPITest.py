from Clients.clientApp import ClientApp
import unittest

mail= "pepep123@pas.com"
passw = "secret"

myClient = ClientApp(mail,passw)

class MyTest(unittest.TestCase):

	def test_CantDeleteTwice(self):
		r = myClient.signup()
		self.assertEquals(r.status_code,201)
		myClient.login()
		myClient.getData()
		r = myClient.delete()
		self.assertEquals(r.status_code,200)
		r = myClient.delete()
		#token expired -> 498 (NOT LOGED IN)
		self.assertEquals(r.status_code,498)	

	def test_ChangedPhoto(self):
		myClient.signup()
		myClient.login()
		myClient.postPhoto("aasaa")
		r = myClient.getPhoto()
		self.assertTrue(r.text.find("aasaa")	>= 0)
		r = myClient.delete()
		self.assertTrue(1)

	def test_CallUnexistantUri(self):
		myClient.signup()
		myClient.login()
		r = myClient.uniexistantCall()
		self.assertTrue(r.status_code,501)
		r = myClient.delete()

	def test_GetMatches(self):
		myClient.signup()
		myClient.login()
		r = myClient.getNewMatches()
		self.assertTrue(r.status_code,200)
		r = myClient.delete()
		self.assertTrue(1)

	def test_ModifiedDataBadData(self):
		data = '"Nombre","25","Alias22","pepep123@pas.com","man","Pickachu","sport::tennis"';
		myClient.signup()
		myClient.login()
		r = myClient.modifyData(data)
		self.assertEqual(r.status_code,400)
		r = myClient.getData()
		self.assertEqual(r.status_code,200)
		myClient.delete()

	def test_ModifiedDataGoodData(self):
		data = '"NONONON","25","man","Pickachu","sport::tennis"';
		myClient.signup()
		myClient.login()
		myClient.modifyData(data)
		r = myClient.getData()
		self.assertEqual(r.status_code,200)
		self.assertNotEqual(r.text.find("NONONON"),-1)
		myClient.delete()

	def test_getUserOfSS(self):
		user="asd@asd.com"
		myClient.signup()
		myClient.login()
		r = myClient.getData(user)
		self.assertEqual(r.status_code,200)
		myClient.delete()

	def test_loginUserOfSS(self):
		user="asd@asd.com"
		r = myClient.login(user,"1234")
		self.assertEqual(r.status_code,200)

if __name__ == '__main__':
	unittest.main()
