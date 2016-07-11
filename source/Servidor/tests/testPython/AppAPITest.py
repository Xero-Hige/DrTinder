from Clients.clientApp import ClientApp
import unittest
import base64

mail1= "dehotmail.com"
passw = "123456"

mail2="copitogmail.com"
passw2="123456"

myClient = ClientApp(mail1,passw)
secondClient = ClientApp(mail2,passw2)

myClient.signup()

class MyTest(unittest.TestCase):

	def test_CantDeleteTwice(self):
		myClient.login()
		r = myClient.delete()
		self.assertEquals(r.status_code,200)
		r = myClient.delete()
		#token expired -> 403
		self.assertEquals(r.status_code,403)
		myClient.signup()	

	def test_ChangedPhoto(self):
		myClient.login()
		with open("download.jpg", "rb") as image_file:
			foto_mujer = base64.b64encode(image_file.read())

		myClient.postPhoto(foto_mujer)
		r = myClient.getPhoto()
		self.assertTrue(r.text.find(foto_mujer)	>= 0)

	def test_CallUnexistantUri(self):
		myClient.login()
		r = myClient.uniexistantCall()
		self.assertTrue(r.status_code,501)

	def test_GetMatches(self):
		myClient.login()
		r = myClient.getNewMatches()
		self.assertTrue(r.status_code,200)
		self.assertTrue(1)

	def test_ModifiedDataBadData(self):
		data = '"Nombre","25","Alias22","pepep123@pas.com","man","Pickachu","sport::tennis"';
		myClient.login()
		r = myClient.modifyData(data)
		self.assertEqual(r.status_code,400)
		r = myClient.getData()
		self.assertEqual(r.status_code,200)

	def test_ModifiedDataGoodData(self):
		data = '"NONONON","25","man","sport::tennis"';
		myClient.login()
		myClient.modifyData(data)
		r = myClient.getData()
		self.assertEqual(r.status_code,200)
		self.assertNotEqual(r.text.find("NONONON"),-1)

	def test_getUserOfSS(self):
		user="asd@asd.com"
		myClient.login()
		r = myClient.getData(user)
		self.assertEqual(r.status_code,200)

	def test_loginUserOfSS(self):
		user="copito@gmail.com"
		r = myClient.login(user,"123456")
		self.assertEqual(r.status_code,200)

	def test_sendMsgToExistantUser(self):
		msg="khjcdjutdutgcfju"
		myClient.login()
		myClient.sendMessage(mail2,msg)
		secondClient.login()
		r = secondClient.chatFriend(mail1)
		self.assertNotEqual(r.text.find(msg),-1)


	def test_BothLikeAndMatch(self):
		myClient.login()
		secondClient.login()
		myClient.likeUser(mail2)
		secondClient.likeUser(mail1)
		r = myClient.friends()
		self.assertNotEqual(r.text.find(mail2),-1)
		r = secondClient.getNewMatches()
		self.assertNotEqual(r.text.find(mail1),-1)		


if __name__ == '__main__':
	unittest.main()
	myClient.delete()
