from Clients.clientShared import *
import sys, getopt
import unittest

LOC = {'latitude':1.53, 'longitude': -1.53}
INT = [{'category':'sport', 'value':'tennis' },{'category':'music/band','value':'radiohead'}]
photo = "asdasdasdasd"
new_user = {'user':{ 'name':'antonio','age':25,'sex':'man','alias':'Antu','email':'antu123@gmail.com','photo_profile':'','location':LOC,'interests': INT },'metadata':{'version':'0,1'}}
modified_user = {'user':{ 'name':'antonio','age':25,'sex':'man','alias':'Antu','email':'antu123@gmail.com','photo_profile':'','location':LOC,'interests': INT },'metadata':{'version':'0,1'}}

client = ClientShared()

class MyTest(unittest.TestCase):

	def test_get_user_existant(self):
		r  = client.getUser(1)
		self.assertEqual(r.status_code,200)

	def test_get_user_inexistant(self):
		r  = client.getUser(2)
		self.assertEqual(r.status_code,400)

	def test_cannot_create_user_twice_then_delete(self):
		r  = client.newUser(new_user)
		createdUser=json.loads(r.text)
		createdId = createdUser['user']['id']
		self.assertEqual(r.status_code,201)
		r = client.newUser(new_user)
		self.assertTrue(r.status_code >= 400)
		r = client.delete(createdId )
		self.assertEqual(r.status_code,200)

	def test_create_modified(self):
		r  = client.newUser(new_user)
		createdUser=json.loads(r.text)
		createdId = createdUser['user']['id']
		self.assertEqual(r.status_code,201)
		mod_us = modified_user
		mod_us['user']['id'] = createdId
		r = client.modifyUser(modified_user, createdId)
		self.assertEqual(r.status_code,200)
		r = client.delete(createdId )
		self.assertEqual(r.status_code,200)

	def test_post_existant_interest(self):
		r = client.newInterest({'category':'movies','value':'drama'})
		self.assertFalse(r.status_code == 201)

if __name__ == '__main__':
	unittest.main()