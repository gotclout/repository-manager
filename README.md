repository-manager
==================

A simple repository manager experiment

Compilation Instructions:

To compile my application, execute that runTest.sh bash script located in the
repository-manager directory and follow the instructions. Select 'c' from the
menu to compile. The executable bin/ReposioryManager will be created.

Execution Execution Instructions:

To run the application, execute the runTest.sh bash script located in the
repository manager directory and follow the instructions. Select 'r' from the
menu to compile. When prompted enter the port number and policy file that the
server should accept as input.

I have implemented additional functionality that verifies the validity of
certificates against the current UTC prior to verifying a certificate.

Notes:

Because this application uses XSB from the command line, the executable must
have sufficient privlege to write a temporary file to disk and remove it. The
writeBinderFile operation will create a temporary file containing the server
policy and any transformed clauses that are loaded into XSB prior to executing
query requests.

The server will close connections and exit if the client enters "exit" as a
request, or the server receives an empty request.
