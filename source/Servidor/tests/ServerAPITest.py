import unittest
from client import Client

url = "http://localhost:8000"
mail= "pepep123@pas.com"
passw = "secret"

client = Client(url,mail,passw)

class TestStringMethods(unittest.TestCase):

	def test_signup_get_token_login_delete_user(self):
		print("LOGIN")
		r = client.signup()
		#self.assertTrue(r.status, 201)
		r = client.login()
		#self.assertTrue(r.status, 200)
		r = client.get_token()
		#self.assertTrue(r)
		r = client.delete()
		self.assertTrue(r)

	def test_SignupTokenCantLoginDeleteuser(self):
		print("CANT LOGIN")
		r = client.signup()
		#self.assertTrue(r.status, 201)
		r = client.login("1234")
		#self.assertTrue(r.status, 200)
		r = client.get_token()
		#self.assertTrue(r)
		r = client.delete()
		self.assertTrue(r)

	def test_singTokenModifyThenDelete(self):
		print("MODIFY USER")
		r = client.signup()
		#self.assertTrue(r.status, 201)
		r = client.get_token()
		#self.assertTrue(r.status, 200)
		r = client.modify_data("")
		#self.assertTrue(r.status, 200)
		r = client.delete()
		self.assertTrue(r)

	def test_singTokenMatchesDelete(self):
		print("GET MATCHES")
		r = client.signup()
		#self.assertTrue(r.status, 201)
		r = client.get_token()
		#self.assertTrue(r.status, 200)
		r = client.get_new_matches()
		#self.assertTrue(r.status, 200)
		r = client.delete()
		self.assertTrue(r)

	def test_singTokenUserInfoAndPhotoDelete(self):
		print("TOKEN USER INFO")
		r = client.signup()
		#self.assertTrue(r.status, 201)
		r = client.get_token()
		#self.assertTrue(r.status, 200)
		r = client.get_data(client.mail)
		#self.assertTrue(r.status, 200)
		r = client.get_photo(client.mail);
		#self.assertTrue(r.status, 200)
		r = client.delete()
		self.assertTrue(r)


	def test_UnexistantCall(self):
		print("UNEXISTANT CALL")
		r = client.uniexistant_call()
		self.assertEqual(1,1)
		#self.assertEqual(r.status, 501);

  
if __name__ == '__main__':
	unittest.main()

