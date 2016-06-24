import requests
import sys

if ( len(sys.argv) == 2  ):
	requests.delete("http://dr-tinder.herokuapp.com/users/" + str(sys.argv[1]))
else:
	print "No id por parametro"

