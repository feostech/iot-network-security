#include <ESP8266WiFi.h>
#include <ESP8266WebServerSecure.h>

const char* ssid = "ssid";
const char* password = "ssid-password";

BearSSL::ESP8266WebServerSecure server(443);

static const char serverCert[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDeTCCAmGgAwIBAgIULIlFhSIGMblnBADOKROIvP9x+3UwDQYJKoZIhvcNAQEL
BQAwTDELMAkGA1UEBhMCSU4xCzAJBgNVBAgMAlROMRowGAYDVQQKDBFGRU9TIFRF
Q0hOT0xPR0lFUzEUMBIGA1UEAwwLMTkyLjE2OC4xLjcwHhcNMjQwMjEyMTQyNTMy
WhcNMzUwNTAxMTQyNTMyWjBMMQswCQYDVQQGEwJJTjELMAkGA1UECAwCVE4xGjAY
BgNVBAoMEUZFT1MgVEVDSE5PTE9HSUVTMRQwEgYDVQQDDAsxOTIuMTY4LjEuNzCC
ASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAOVRgnuXWMcI0vYKhzVXMm3r
EudS0q4+o5WO1wrMAYjT7xNMJx1zii5fh/T8NDTKtB+f7WI2kROjDuTfdbCl/BXW
T6GAMSUtF2CAsI2S+JPwymOHUsQmbXZsyoSl12oF1YpDrv0bdL42CsPhyMYa5CJs
whyVDxiKTn2n2iATYXgAQm1qoxj4tTv+91NxRmlqmcd5qUoSmvtdPzwWgtkgAxJ4
YC6/8BBDA1o+VwVCaEP6uLdlMFFYD89jmrK3rIU9Um4qIaolprd+tfnNqVLl52W9
HQDEZ5SljW+bNi0cw3WEcGWQAC4Do6sbIMLpSBeoci9JXGX4lKfkqp+8XRzqQ4UC
AwEAAaNTMFEwHQYDVR0OBBYEFJ0+vDVDUoNHQ0Zd++asKues1XMgMB8GA1UdIwQY
MBaAFJ0+vDVDUoNHQ0Zd++asKues1XMgMA8GA1UdEwEB/wQFMAMBAf8wDQYJKoZI
hvcNAQELBQADggEBAD+NX3mDZP1li7gObFNIZO1A/mUqxyFXTLw5KponTvvtvvzz
66jAkxSBRlsvhywDt+3MGuiXA49hpsnepicXknAiHJUw4CRarxT2J6dvGYW6eNpT
Dcp97dR8RFPfH/GjTaMdQph5r6h3PXhRQfRs+h/kaWEFAHGsxvQlpc52rukzh+yY
+1Tj3btESRX0WL6FTC3Ayh6Vt46UnbbAVq6VWd10vcaj0c7BMQZG+nlx7Dbb2cCC
X/WwOBtomuilpE4T9n4Ix97xGi36TqWrs/OdX8eHL3W/kX31vxMcoON6ANvkzOTH
vLNWJsM16tvglw5uzpYSOzN0V3k6xgoiLv5/Qzs=
-----END CERTIFICATE-----
)EOF";

static const char serverKey[] PROGMEM = R"EOF(
-----BEGIN PRIVATE KEY-----
MIIEvgIBADANBgkqhkiG9w0BAQEFAASCBKgwggSkAgEAAoIBAQDlUYJ7l1jHCNL2
Coc1VzJt6xLnUtKuPqOVjtcKzAGI0+8TTCcdc4ouX4f0/DQ0yrQfn+1iNpETow7k
33WwpfwV1k+hgDElLRdggLCNkviT8Mpjh1LEJm12bMqEpddqBdWKQ679G3S+NgrD
4cjGGuQibMIclQ8Yik59p9ogE2F4AEJtaqMY+LU7/vdTcUZpapnHealKEpr7XT88
FoLZIAMSeGAuv/AQQwNaPlcFQmhD+ri3ZTBRWA/PY5qyt6yFPVJuKiGqJaa3frX5
zalS5edlvR0AxGeUpY1vmzYtHMN1hHBlkAAuA6OrGyDC6UgXqHIvSVxl+JSn5Kqf
vF0c6kOFAgMBAAECggEAVlLhMiIuhrpBI+KhtEgvUSZpNtA8Q+CLluyu8b8LItVZ
DaQgo1sPnNZUaToZcvgNZ0w+Qbb11PUQUcL23DkXlKbY0hxeI7b+MlMvYxP85EAE
CAR0eSdYFHjuoZGGbkBk+sfw0xHnWAUi90sC1+RelcDoL3dDOAM0eePaXjG9gryr
z9hMh5U3QhJ9telBPZsotrsd7yv6W4uIN9QmIGC4eYCmNa87alRw3mq4O2EYPxzp
8BEceqyd2f3d/A6OEbBbYQCvw6A7GRcMGKjSgl28VQ3mCn11CXvk/8BUBTj/EPFC
EvBgsGBjnfCYOgPFetdGv/bp1BmnVwVvdY3eFNlIzwKBgQD3X1VWarz+xbkhL34T
6QIPmp8Sx3Wju4Y+aoSeBFFpFQky5doiYVuZtcZLld64ZsjSmZp6F3T8b1Fd7s4Z
dbhglsLPZYzWdTRirMuS6J2ECkkYV5rJRhTQjhO9aOrncT2fHuCtI1DoYSzJL3gI
15o+iC3JVGoNMPZh2kGg4aYM7wKBgQDtUPsmwTcKJx8WMnz6d+/QKZZBVbAXAy2t
dqkSIs46v+y6dl6H6MGDwHrm5G5FHGSYE/7kEqlutzPiX8K4+DgT/ij+6HzUio14
tPf37SBE/JN2yR4q1w07taB7bByFlYWkuK88Hz+gnKWv/B+KXHwzIaUu1DGluK4p
RyzchuYeywKBgQDjmPkhW2+sUSs2F0Y+92GIDdFSK+ZW0RCAmOyPnpVww4QFKmn6
u+zVjeN2Y9vZeehHrD/vGWnvYFRSwocp8Z/EbrGqtFpSbt/V8foTnqZeeEAsf13Q
1gQMpswOt1t5ujziDDzXz+h/Bfl42Y3tRMGLyyulU1+H54apHXDInxRyTQKBgQDo
vFKSX0qrGlLSYhFPUfBOJXrF+FMJPoS9aXHF53x5AWAYPZ3Xi9y8aTge2EeMX5wH
yRRgeCzR97laeK9u13+nm8p4s2g4yOiVSGUnL9ZF0a6NABAl02GW/Ti10gDmwQyf
zr+MJtkvmnCjdo84Eht7ybNFibRm4gdrU/IbE/D6oQKBgBvx17+sjtbXnfWElmxR
rMnipb/+r8TRIv1K8cIzb9/8skJdougqAts1C09S9oauur/U7bavBVI5I+MbljrG
aN3xUwScFt2LZ0ph/JYAaF+QGtkxP/OsLVhA70E4JD51XgD9iEe97fB1Xn4RMW23
L6Sx4Sx2swjASoUzPyBd6jZj
-----END PRIVATE KEY-----
)EOF";

void handleRoot() {
  server.send(200, "text/plain", " Hello From ESP 8266 ! ");
}
void setup() {
  Serial.begin(115200);  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");

while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.println("Connecting to WiFi...");
  }
    Serial.println("Connected to WiFi");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    
    server.getServer().setRSACert(new BearSSL::X509List(serverCert), new BearSSL::PrivateKey(serverKey));
  // Start the server
    server.on("/", handleRoot);
    server.begin();
    Serial.println("HTTP server started");
}
void loop() {
    server.handleClient();
}
