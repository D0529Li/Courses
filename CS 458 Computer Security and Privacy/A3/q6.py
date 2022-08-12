import requests
import nacl
from nacl.encoding import Base64Encoder
from nacl.signing import SigningKey
from nacl.public import PrivateKey, Box
import json
import base64

# reference: https://readthedocs.org/projects/lmctvpynacl/downloads/pdf/use-autodoc-for-apis/

signing_key = SigningKey.generate()
verify_key = signing_key.verify_key
verify_key_b64 = verify_key.encode(encoder=Base64Encoder)

data = {
    "api_token": "cae9435dd42963557517552f361a97b63972c648a57af4167dfc4f42b51eb503",
    "pubkey": verify_key_b64
}
response = requests.post("https://hash-browns.cs.uwaterloo.ca/api/prekey/set-identity-key", data=data)

print(response)

sk = PrivateKey.generate()
pk = sk.public_key

signed = signing_key.sign(pk.encode(encoder=nacl.encoding.RawEncoder))

data = {
    "api_token": "cae9435dd42963557517552f361a97b63972c648a57af4167dfc4f42b51eb503",
    "pubkey": base64.b64encode(signed)
}

response = requests.post("https://hash-browns.cs.uwaterloo.ca/api/prekey/set-signed-prekey", data=data)

print(response)

##################################
# get identity key
data = {
    "api_token": "cae9435dd42963557517552f361a97b63972c648a57af4167dfc4f42b51eb503",
    "user": "Fluffy"
}

response = requests.post("https://hash-browns.cs.uwaterloo.ca/api/prekey/get-identity-key", data=data)

pk_encoded = json.loads(response.text)['pubkey']
verify_key = nacl.signing.VerifyKey(pk_encoded, encoder=Base64Encoder)

# get signed prekey
data = {
    "api_token": "cae9435dd42963557517552f361a97b63972c648a57af4167dfc4f42b51eb503",
    "user": "Fluffy"
}

response = requests.post("https://hash-browns.cs.uwaterloo.ca/api/prekey/get-signed-prekey", data=data)

signed_pk = verify_key.verify(base64.b64decode(json.loads(response.text)['pubkey']))
pk_fluffy = nacl.public.PublicKey(signed_pk, encoder=nacl.encoding.RawEncoder)

message = b"lyc lyc lyc"
box = Box(sk, pk_fluffy)

encrypted = box.encrypt(message)

data = {
    "api_token": "cae9435dd42963557517552f361a97b63972c648a57af4167dfc4f42b51eb503",
    "recipient": "Fluffy",
    "msg": base64.b64encode(encrypted)
}

response = requests.post("https://hash-browns.cs.uwaterloo.ca/api/prekey/send", data=data)

print(response)

############################################

data = {
      "api_token": "cae9435dd42963557517552f361a97b63972c648a57af4167dfc4f42b51eb503"
}

response = requests.post("https://hash-browns.cs.uwaterloo.ca/api/prekey/inbox", data=data)
print(box.decrypt(base64.b64decode(json.loads(response.text)[0]["msg"])))
