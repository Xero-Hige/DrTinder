import base64
import Image
from PIL import Image  

with open("peachFace.jpg", "rb") as image_file:
    encoded_string = base64.b64encode(image_file.read())

decoded_string = base64.b64decode(encoded_string)

filename = 'some_image.jpg'  # I assume you have a way of picking unique filenames
with open(filename, 'wb') as f:
    f.write(decoded_string)

                                                                              
img = Image.open(filename)
img.show() 