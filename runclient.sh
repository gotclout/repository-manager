#!/bin/bash

echo "Enter 1 for credential 1 or 2 for credential 2"
echo -n
read credential;
echo

CREDENTIAL="./client_credential1.msg.txt"

if [ "$credential" == 2 ]; then
  CREDENTIAL="./client_credential2.msg.txt"
fi

java -jar RepositoryManagerClient.jar "127.0.0.1" "1983" $CREDENTIAL "./client.rsa.der"
