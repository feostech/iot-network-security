#!/bin/bash

ESP8266_URL="https://arduino.esp8266.com/stable/package_esp8266com_index.json"
ARDUINO_CLI=$HOME/bin/arduino-cli

arduino_cli_setup ()
{
    sudo apt-get --yes install curl &&
    pushd $HOME &&
    curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh &&
    echo 'export PATH=$HOME/bin:$PATH' >> ~/.bashrc &&
    $ARDUINO_CLI config init
}

# Check if arduino-cli tool is installed
$ARDUINO_CLI version
if [ $? -ne 0 ]; then
    arduino_cli_setup
fi

# update the indices
$ARDUINO_CLI core update-index --additional-urls=$ESP8266_URL &&

#install the core for your board
$ARDUINO_CLI core install esp8266:esp8266 --additional-urls=$ESP8266_URL &&

#verify we have installed the core properly by running
$ARDUINO_CLI core list &&

echo "Arduino setup completed!"

