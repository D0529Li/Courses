import requests
import nacl.secret
import nacl.pwhash
import nacl.utils
import binascii
import base64
import json

# reference: https://readthedocs.org/projects/lmctvpynacl/downloads/pdf/use-autodoc-for-apis/

psp = b"musical volume"
salt = binascii.unhexlify("12c06b9b5d1b15a30a7790d23b1f7fdf4ed089ae952ccd2cba8f579afb09d155")
ops = 524288
mem = 16777216
kdf = nacl.pwhash.kdf_scryptsalsa208sha256
message = b"LYC LYC LYC"

key = kdf(nacl.secret.SecretBox.KEY_SIZE, psp, salt, opslimit=ops, memlimit=mem)
box = nacl.secret.SecretBox(key)
nonce = nacl.utils.random(nacl.secret.SecretBox.NONCE_SIZE)

encrypted = box.encrypt(message, nonce)

data = {
    "api_token": "cae9435dd42963557517552f361a97b63972c648a57af4167dfc4f42b51eb503",
    "recipient": "Fluffy",
    "msg": base64.b64encode(encrypted)
}
response = requests.post("https://hash-browns.cs.uwaterloo.ca/api/psp/send", data=data)

print(response)

#####################################

data = {
      "api_token": "cae9435dd42963557517552f361a97b63972c648a57af4167dfc4f42b51eb503"
}

response = requests.post("https://hash-browns.cs.uwaterloo.ca/api/psp/inbox", data=data)
print(box.decrypt(base64.b64decode(json.loads(response.text)[0]["msg"])))
