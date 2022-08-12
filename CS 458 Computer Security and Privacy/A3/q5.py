import requests
from nacl.public import PrivateKey, PublicKey, Box
import json
import base64
from nacl.hash import blake2b
from nacl.encoding import Base64Encoder


# reference: https://readthedocs.org/projects/lmctvpynacl/downloads/pdf/use-autodoc-for-apis/

data = {
    "api_token": "cae9435dd42963557517552f361a97b63972c648a57af4167dfc4f42b51eb503",
    "user": "Fluffy"
}
response = requests.post("https://hash-browns.cs.uwaterloo.ca/api/pke/get-key", data=data)

hex_key = json.loads(response.text)['pubkey']
key = base64.b64decode(hex_key)
print(blake2b(key))

##############################################

sk = PrivateKey.generate()
pk = sk.public_key

data = {
    "api_token": "cae9435dd42963557517552f361a97b63972c648a57af4167dfc4f42b51eb503",
    "pubkey": pk.encode(encoder=Base64Encoder)
}
response = requests.post("https://hash-browns.cs.uwaterloo.ca/api/pke/set-key", data=data)

print(response)

pk_fluffy = PublicKey(hex_key, encoder=Base64Encoder)
box = Box(sk, pk_fluffy)
message = b"Lyc Lyc Lyc"
encrypted = box.encrypt(message)

data = {
    "api_token": "cae9435dd42963557517552f361a97b63972c648a57af4167dfc4f42b51eb503",
    "recipient": "Fluffy",
    "msg": base64.b64encode(encrypted)
}
response = requests.post("https://hash-browns.cs.uwaterloo.ca/api/pke/send", data=data)

print(response)

###########################################

data = {
      "api_token": "cae9435dd42963557517552f361a97b63972c648a57af4167dfc4f42b51eb503"
}

response = requests.post("https://hash-browns.cs.uwaterloo.ca/api/pke/inbox", data=data)

print(box.decrypt(base64.b64decode(json.loads(response.text)[0]["msg"])))
