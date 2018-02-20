import os

with open('slingshot_29.csv', 'r') as data:
    plaintext = data.read()

plaintext = plaintext.replace(',', '')

file = open('data_29', 'w')
file.write(plaintext)

file.close()            
