#!/bin/bash

# Install mosquitto
sudo apt-add-repository ppa:mosquitto-dev/mosquitto-ppa
sudo apt-get install mosquitto
sudo apt-get install mosquitto-clients
 
# Generate CA certificate and key
openssl req -new -x509 -days 365 -extensions v3_ca -keyout my-ca.key -out my-ca.crt 

# Generate private key for MQTT server
openssl genrsa -out server.key 2048 

# Generate signing request for CA
openssl req -out server.csr -key server.key -new 

# Sign the CSR with CA
openssl x509 -req -in server.csr -CA my-ca.crt -CAkey my-ca.key -CAcreateserial -out server.crt -days 180 

