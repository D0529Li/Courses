import requests
import nacl.secret
import binascii
import base64
import json

# reference: https://readthedocs.org/projects/lmctvpynacl/downloads/pdf/use-autodoc-for-apis/

key = "5ce554e06ef81822ad51d7cf95e8cf6156029f6a3933023f6ff886c17b6a1daf"
box = nacl.secret.SecretBox(binascii.unhexlify(key))
message = b"LYC LYC LYC"

encrypted = box.encrypt(message)

data = {
    "api_token": "cae9435dd42963557517552f361a97b63972c648a57af4167dfc4f42b51eb503",
    "recipient": "Fluffy",
    "msg": base64.b64encode(encrypted)
}
response = requests.post(
    "https://hash-browns.cs.uwaterloo.ca/api/psk/send", data=data)

print(response)

#####################################

data = {
    "api_token": "cae9435dd42963557517552f361a97b63972c648a57af4167dfc4f42b51eb503"
}

response = requests.post(
    "https://hash-browns.cs.uwaterloo.ca/api/psk/inbox", data=data)
print(box.decrypt(base64.b64decode(json.loads(response.text)[0]['msg'])))
