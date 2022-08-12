import requests
from nacl.encoding import Base64Encoder
from nacl.signing import SigningKey

# reference: https://readthedocs.org/projects/lmctvpynacl/downloads/pdf/use-autodoc-for-apis/

signing_key = SigningKey.generate()
verify_key = signing_key.verify_key
verify_key_b64 = verify_key.encode(encoder=Base64Encoder)

data = {
    "api_token": "cae9435dd42963557517552f361a97b63972c648a57af4167dfc4f42b51eb503",
    "pubkey": verify_key_b64
}
response = requests.post("https://hash-browns.cs.uwaterloo.ca/api/signed/set-key", data=data)

print(response)

#####################################

signed_b64 = signing_key.sign(b"LYC LYC LYC", encoder=Base64Encoder)
data = {
      "api_token": "cae9435dd42963557517552f361a97b63972c648a57af4167dfc4f42b51eb503",
      "recipient": "Fluffy",
      "msg": signed_b64
}

response = requests.post("https://hash-browns.cs.uwaterloo.ca/api/signed/send", data=data)
print(response)
