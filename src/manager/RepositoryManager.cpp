#include <iostream>
#include <fstream>
#include <cstdlib>

#include "TransportService.h"
#include "../utils/SecurityService.h"
#include <openssl/buffer.h>

using namespace std;

/**
 * The Repository Manager is a server application that implements
 * an authorization scheme using binder. The authorization protocol is a five
 * step process in which a client sends a containing it's public key. The
 * server then sends a challenge message and if verified, request from the
 * client are processed and corresponding replies are sent.
 *
 * The application should be invoked with arguments
 *
 * @param argc, the number of arguments
 * @param argv[0] program name, argv[1] port, argv[3] policy file, additionally
 *  by providing a 4th argument, a flag will be set causing the Repository
 *  Manager to validate the date times associated with certificates.
 */
int main(int argc, char* argv[])
{
  SecurityService ss;
  if(argc < 3)
  {
    cerr << "Num Args: " << argc << endl
         << "Usage: <prog> <port> <policy>" << endl;
    return 1;
  }
  else
  {
    int port = atoi(argv[1]);

    if(argc == 4)
      VALIDATE_CERT_TIME = true;

    cout << LINE_S << endl << "NAME  : Repository Manager" << endl
         << "PORT  : " << port << endl << "POLICY: " << argv[2] << endl
         << LINE_S << endl;

    srand(time(0));
    TransportService ts(port, argv[2]);
    ts.startListening();
  }

  return 0;
}
