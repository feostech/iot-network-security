
#OPEN SSL SETUP ON MQTT:

##Installation mosquitto in terminal-1:

```
sudo apt-add-repository ppa:mosquitto-dev/mosquitto-ppa
sudo apt-get install mosquitto
sudo apt-get install mosquitto-clients
```

```
Mosquitto or mosquitto -v
  
Starts locally by using the default config.
```

First, I encountred with the Error which was saying that. The port is already in use but no problem. Configure the
Mosquitto.conf and restart the mosquitto then it solved.

```
systemctl restart mosquitto. services 
```

```
mosquitto or mosquitto -v  

To check whether that error was coming or not.
```
OPEN THE TERMINAL-2:
#installation of ssl(ca) file by below command:

##To Generate our own ca certificate and key:
```
openssl req -new -x509 -days 365 -extensions v3_ca -keyout my-ca.key -out my-ca.crt 
```
After install the ca certificate and key to give the following terms

Enter PEM pass phrase:                                                          
Verifying - Enter PEM pass phrase: 

Country Name (2 letter code) [AU]:IN
State or Province Name (full name) [Some-State]:TN 
Locality Name (eg, city) []:Chennai
Organization Name (eg, company) [Internet Widgits Pty Ltd]:My IoT Company
Organizational Unit Name (eg, section) []:Certificate Authority   
Common Name (e.g. server FQDN or YOUR name) []:  
Email Address []:

##Now generate a private key for use by our MQTT server
```
openssl genrsa -out server.key 2048 
```
##Generate a signing request to send to the certificate authority
```
openssl req -out server.csr -key server.key -new 
```
Reenter the terms:

Country Name (2 letter code) [AU]:IN                                            
State or Province Name (full name) [Some-State]:Tn                        
Locality Name (eg, city) []:Chennai                                               
Organization Name (eg, company) [Internet Widgits Pty Ltd]:My IoT Company       
Organizational Unit Name (eg, section) []:MQTT                                  
Common Name (e.g. server FQDN or YOUR name) []:mqtt.example.net                 
Email Address []:  

A challenge password []: (Nill)                                                       
An optional company name []: (Nill)

##Send the CSR to the CA, they will sign it, or sign it with your own CA: 
```
openssl x509 -req -in server.csr -CA my-ca.crt -CAkey my-ca.key -CAcreateserial -out server.crt -days 180 
```
Enter pass phrase for my-ca.key: 
##Certificates are generated, now edit your config file: 
```
vim mosquitto.config

Give the below commands to vim mosquitto.config

port 8883                                                                       
cafile my-ca.crt                                                                
certfile server.crt                                                             
keyfile server.key

   If you want clients to provide a certificate before connecting:               
   require_certificate true  
```

## Run mosquitto using that config on your server, then to publish: 
```
mosquitto_pub --cafile my-ca.crt -h mqtt.example.net -t hello -m world 
```
##Configuration Methods:

To create the configuration by giving the following Command 
```
Cat mosquitto.conf
```
   
Give the below format :

Listener 8883
Allow_anonymous true
Cafile/home/path/folder-name/my-ca.crt
Certfile/home/path/folder-name/server.crt 
Keyfile/home/path/folder-name/server.key 
Require_certificate false
 
Check the above folders by giving the command:
```
ls
```

After the checkup give the publish command:
mosquitto_pub --cafile my-ca.crt -h "mqtt-ip-address" -t "topic" -m "hello"
  
OPEN THE NEW TERMINAL-3:
give the mosquitto config by giving the below command:
```
mosquitto -c mosquitto.conf 
```


