
f1 = open('ciphertext1', 'rb')
ct1 = f1.read()
print(len(ct1))
f2 = open('ciphertext2', 'rb')
ct2 = f2.read()
print(len(ct2))
diff = [(ord(ct2[i]) - ord(ct1[i])) % 256 for i in range(len(ct1))]
diff_str = ''
for i in diff:
    diff_str += chr(i)
f3 = open('p2p1', 'wb')
# print(diff_str)
f3.write(diff_str)
f3.close()
