import requests

data = {
    "api_token": "cae9435dd42963557517552f361a97b63972c648a57af4167dfc4f42b51eb503",
    "recipient": "Fluffy", "msg": "aGVsbG8gd29ybGQ="
}
header = {"Accept": "application/json",
          "Content-Type": "application/json"}
response = requests.post(
    "https://hash-browns.cs.uwaterloo.ca/api/plain/send", json=data, headers=header)
print(response.status_code)

###################################################

data = {
    "api_token": "cae9435dd42963557517552f361a97b63972c648a57af4167dfc4f42b51eb503"
}

response = requests.post(
    'https://hash-browns.cs.uwaterloo.ca/api/plain/inbox', json=data, headers=header)
print(response.json())
