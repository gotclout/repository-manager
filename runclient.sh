#!/bin/bash

echo "Enter 1 for credential 1 or 2 for credential 2"
echo -n
read credential
echo

CREDENTIAL="./credentials/client_credential1.msg.txt"

if [ "$credential" = "2" ]; then
  CREDENTIAL="./credentials/client_credential2.msg.txt"
fi

java -jar ./bin/RepositoryManagerClient.jar "127.0.0.1" "1983" $CREDENTIAL "./credentials/client.rsa.der"
