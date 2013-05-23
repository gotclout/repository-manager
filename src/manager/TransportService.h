#ifndef __TransportService__
#define __TransportService__

#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <iostream>
#include <sstream>
#include <string>

#include "AstWalker.h"
#include "../utils/fifoqueue.h"
#include "../utils/RepositoryManagerUtils.h"
#include "../parser/RepositoryManagerLexer.h"
#include "../parser/RepositoryManagerParser.h"

static SecurityService ss;

class TransportService
{
  private:

    /** The name of the policy file for the repository Manager **/
    string policyFileName;

    /** String containing the policy **/
    string policy;

    /** List of client credentials **/
    vector<Credential*> clients;

    /** Map of challenge strings to clients **/
    map<string, int> challengeClientMap;

    int
      ///
      port,
      //
      socketId,
      //
      clientSocketId,
      //
      socketErr;

    stringstream
      //
      socketMsg,
      //
      printStr;

    //
    struct sockaddr_in socketAddr;

    bool
      //
      clientAuthenticated,
      //
      processingRequests,
      //
      socketInit,
      //
      socketListening;

    //
    pRepositoryManagerParser parser;

    //
    pRepositoryManagerLexer lex;

    //
    pANTLR3_INPUT_STREAM input;

    //
    pANTLR3_COMMON_TOKEN_STREAM tokens;

    //
    pANTLR3_COMMON_TOKEN ts;

    //The antlr parse tree
    pANTLR3_BASE_TREE tt;

    //
    pANTLR3_STRING tts;

    //
    AstWalker* tWalker;

  public:

    /**
     * Reads the server policy into a buffer
     */
    bool readPolicy()
    {
      bool retVal = false;
      ifstream in;

      if(policyFileName != "")
      {
        in.open(policyFileName.c_str());
        if(in.is_open())
        {
          in.seekg(0, ios::end);
          int len = in.tellg();
          if(len > 0)
          {
            char* buff = new char[len + 1];
            memset(buff, 0, len + 1);
            in.seekg(ios::beg);
            in.read(buff, len);
            policy = buff;
            delete [] buff;
            buff = 0;
            retVal = true;
          }

          in.close();
          in.clear();
        }
      }

      return retVal;
    };

    /**
     * Default constructor for the transport service
     */
    TransportService(int pPort, char pPolicy [])
    {
      policy = "";
      policyFileName = (string) pPolicy;
      if(!readPolicy())
        cerr << "Error: Could not read server policy" << endl;
      port = pPort;
      socketId = socket(AF_INET, SOCK_STREAM, 0);
      socketMsg << "";
      socketListening = false;
      clientAuthenticated = false;
      processingRequests = false;

      if(socketId > 0)
      {
        size_t addrLen = sizeof(socketAddr);
        memset((char*) &socketAddr, 0, addrLen);
        socketAddr.sin_family = AF_INET;
        socketAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        socketAddr.sin_port = htons(port);

        socketErr = bind(socketId, (struct sockaddr*) &socketAddr, addrLen);

        if(!socketErr)
        {
          socketErr = listen(socketId, MAX_CONNECT);
          socketInit = (!socketErr) ? true : false;
          if(socketInit)
          {
            cout << "Port: " << socketAddr.sin_port << endl;
            cout << "IP: " << socketAddr.sin_addr.s_addr << endl;
          }
        }
      }
    };

    /**
     * The destructor
     */
    ~TransportService()
    {
      if(clients.size() > 0)
        clients.clear();
      char tmpPolicy[TMP_FILE_S.length() + 3];
      memset(tmpPolicy, 0, TMP_FILE_S.length() + 3);
      strcpy(tmpPolicy, TMP_FILE_S.c_str());
      strcat(tmpPolicy, ".P");
      remove(tmpPolicy);
    };

    /**
     * Authenticates a client by generating a challenge message to be signed
     * by the client using its private key
     */
    bool authenticateClient(Credential* & pCred)
    {
      bool retVal = false;
      char* challenge = ss.generateChallenge();
      string anotherChallenge = (string) challenge;
      int numRead = 0, bytesRead = 0, outLen = 0;

      if(challenge)
      {
        stringstream tss;
        tss << BEGIN_CHALLENGE_S << endl
            << challenge         << endl
            << END_CHALLENGE_S   << endl;
        size_t mLen = tss.str().size();
        outLen = write(clientSocketId, tss.str().c_str(), mLen);

        unsigned char* msgDigest = ss.getMsgDigest(anotherChallenge);
        char* msgBuff = new char [MAX_BUFF + 1], *pos = 0;
        pos = msgBuff;
        memset(msgBuff, 0, MAX_BUFF + 1);
        cls(tss);
        do
        {
          pos = (char*)(msgBuff + bytesRead);
          numRead = read(clientSocketId, pos, MAX_READ);
          bytesRead += numRead;
          pos = 0;
          if(strstr(msgBuff, END_RESPONSE_S.c_str()))
            numRead = 0;
        }
        while(bytesRead + MAX_READ < MAX_BUFF && numRead > 0);

        string response = (string) msgBuff;
        retVal = ss.verifySig(pCred->pubKey, response, anotherChallenge);

        //clean up
        if(msgBuff)
          delete [] msgBuff;
        if(msgDigest)
          delete [] msgDigest;
        if(challenge)
          delete [] challenge;
        msgDigest = 0;
        challenge = 0;
        msgBuff = 0;
        pos = 0;
      }

      return retVal;
    };

    /**
     * Process a client request generating a binder statement and executing
     * the query in xsb
     */
    void processRequest()
    {
      int numRead = 0, bytesRead = 0, outLen = 0;
      size_t  mLen = 0;
      stringstream tss;
      char* msgBuff = 0, *pos = 0;

      msgBuff = new char [MAX_BUFF + 1];
      pos = msgBuff;
      memset(msgBuff, 0, MAX_BUFF + 1);
      cls(tss);
      string request = "";

      do
      {
        pos = (char*)(msgBuff + bytesRead);
        numRead = read(clientSocketId, pos, MAX_READ);
        bytesRead += numRead;
        pos = 0;
        if(strstr(msgBuff, END_CLIENT_REQUEST_S.c_str()))
          numRead = 0;
      }
      while(bytesRead + MAX_READ < MAX_BUFF && numRead > 0);

      request = (string) msgBuff;

      if(request.find(FIN_S) != string::npos)
      {
        cout << "Done Processing Request" << endl
             << "Closing Connection..." << endl << endl;
        processingRequests = false;
      }
      else if(request == "")
      {
        cerr << "Error: Received Empty Request" << endl;
        processingRequests = false;
      }

      if(processingRequests)
      {
        cout << "Received Request:" << endl << request << endl;
        string query = ss.generateQuery(request);
        tss << BEGIN_SERVER_RESPONSE_S << endl;
        if(query != "")
        {
          cout << "Generated Query:" << endl << query << endl;

          cls(tss);
          tss << BEGIN_SERVER_RESPONSE_S << endl;
          if(ss.executeQuery(query))
            tss << GRANT_S;
          else
            tss << DENY_S;
        }
        else
            tss << DENY_S;
        tss << endl << END_SERVER_RESPONSE_S << endl;
        mLen = tss.str().length();
        outLen = write(clientSocketId, tss.str().c_str(), mLen);
      }
      else
      {
        socketListening = false;
        close(clientSocketId);
        cout << "Connection closed, server shut down" << endl;
      }

      if(msgBuff)
        delete [] msgBuff;
      msgBuff = 0;
    };

    /**
     * Starts the server listener on a tcp socket
     */
    void startListening()
    {
      int numRead = 0,
          bytesRead = 0;
      char* msgBuf = 0;
      struct sockaddr clientIp;
      socklen_t clientIpLen = sizeof(clientIp);

      if(socketInit)
      {
        socketListening = true;

        while(socketListening)
        {
          if(processingRequests)
            processRequest();
          else
          {
            clientSocketId = accept(socketId, &clientIp, &clientIpLen);

            if(clientSocketId)
            {
              msgBuf = new char[MAX_BUFF + 1];
              memset((void*)msgBuf, 0, MAX_BUFF + 1);
              numRead = read(clientSocketId, msgBuf, MAX_BUFF);

              if(bytesRead < 0)
              {
                cerr << "Error: Reading Message Buffer Returned " << bytesRead << endl;
                socketErr = -1;
              }
              else
              {
                socketMsg << msgBuf;
                processMsg();
              }

              cls(socketMsg);
              delete [] msgBuf;
              msgBuf = 0;
              bytesRead = numRead = 0;
            }
            else
              socketListening = false;
          }
        }
      }
    };

    /**
     * Renders the tree generated by the antlr parser
     */
    void printTree(pANTLR3_BASE_TREE & pTree)
    {
      queue<pANTLR3_BASE_TREE> q;
      ANTLR3_UINT32 childCount = (ANTLR3_UINT32) pTree->getChildCount(pTree);
      int count = (int) childCount;
      cout << "Num Children: " << count << endl;
      for(int i = 0; i < count; i++)
      {
        pANTLR3_BASE_TREE child = (pANTLR3_BASE_TREE) pTree->getChild(
            pTree, (ANTLR3_UINT32) i);
        tts = (pANTLR3_STRING) child->toString(child);
        printStr << tts->chars;
        cout << "Child " << i << ":" << endl << printStr.str() << endl;
        cls(printStr);
        ANTLR3_UINT32 numChildren = (ANTLR3_UINT32) child->getChildCount(child);
        int children = (int) numChildren;
        for(int j = 0; j < children; j++)
        {
          pANTLR3_BASE_TREE child2 = (pANTLR3_BASE_TREE)child->getChild(
              child, (ANTLR3_UINT32) j);
          q.enqueue(child2);
        }
      }

      pANTLR3_BASE_TREE c;
      while((c = (pANTLR3_BASE_TREE)q.dequeue()) && c != 0)
      {
        printTree(c);
        c = q.dequeue();
      }
    };

    /**
     * Processes messages from the client
     */
    bool processMsg()
    {
      bool retVal = true;

      if(socketMsg.str() == "")
        retVal = false;
      else
      {
        string msg = socketMsg.str();

        pANTLR3_UINT8 in = (pANTLR3_UINT8) msg.c_str();
        size_t len = msg.length();
        char* cMsg = (char*) msg.c_str();
        input =
          antlr3StringStreamNew(in, ANTLR3_ENC_8BIT, len, (pANTLR3_UINT8)cMsg);
        lex = RepositoryManagerLexerNew(input);
        tokens =
          antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT, TOKENSOURCE(lex));
        parser = RepositoryManagerParserNew(tokens);
        RepositoryManagerParser_credential_return cred =
          parser->credential(parser);
        tt = (pANTLR3_BASE_TREE) cred.tree;

        tWalker = new AstWalker(tt);
        Credential* c = tWalker->getCredential();
        input->close(input);
        lex ->free(lex);
        tokens ->free(tokens);
        parser ->free(parser);
        delete tWalker;

        if(c)
        {
          cout << *c;
          clients.push_back(c);
          ss.setPolicy(policy);
          cout << "Verifying Credential..." << endl << endl;
          if(ss.verifyCredential(c))
          {
            cout << "Verified Credential" << endl
                 << "Authenticating Client..." << endl << endl;
            clientAuthenticated = authenticateClient(c);
            if(clientAuthenticated)
            {
              cout << "Client Authenticated" << endl
                   << "Loading Policy & Certificates..." << endl << endl;
              if(ss.writeBinderFile())
              {
                processingRequests = true;
                cout << "Loaded Policy & Certificates" << endl
                     << "Waiting for request..." << endl << endl;
              }
              else
                cerr << "Error: Failed to load binder policy and certificates"
                     << endl;
            }
            else
            {
              cerr << "Error: Client could not be authenticated" << endl;
              close(clientSocketId);
            }
          }
          else
          {
            cerr << "Error: Client credential could not be verified" << endl;
            close(clientSocketId);
          }
        }
        else
        {
          cerr << "Error: Invalid client credential" << endl;
          close(clientSocketId);
        }
      }

      return retVal;
    }
};
#endif//__TransportService__
