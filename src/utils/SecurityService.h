#ifndef __SecurityService__
#define __SecurityService__

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>

#include <string>
#include <vector>
#include <sstream>

#include <openssl/objects.h>
#include <openssl/buffer.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/md5.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/err.h>

#include "Mutex.h"
#include "Policy.h"
#include "Credential.h"
#include "RepositoryManagerUtils.h"

using namespace std;

static bool qStats;

/**
 * Thread process for executing XSB queries
 */
static void* executeQueryProc(void* pQuery)
{
  char* query = (char*) pQuery;
  stringstream ss;
  string res = "";
  FILE* f = (FILE*) popen(query, "r");
  if(f)
  {
    cls(ss);
    char line[MAX_LINE];

    while(fgets(line, sizeof(line), f))
      ss << line;

    pclose(f);
    res = ss.str();
    size_t pos = res.find_last_of("\n");

    if(pos != string::npos)
      res = res.substr(pos - 3, 3);

    if(res == "yes")
      qStats = true;

    cls(ss);
  }
  else
    cerr << "could not execute query: " << query << endl;

  return 0;
  //pthread_exit(0);
};

/**
 * The security service provides low level access to operations that are
 * to be performed for verification and authentication including b64 conversion
 * MD5, and signature verification using openssl APIs. Additionally, it
 * executes queries using the XSB command line interface.
 */
class SecurityService
{
  private:

    /**
     * The security services maintains a string representation of the servers
     * policy
     */
    string policyStr;

    /** The security maintains an instance of the server policy structure **/
    Policy* policy;

    /** The credential pointer for a security service instance **/
    Credential* credPtr;

    /** Only one thread should try executing XSB queries at a time **/
    Mutex mutex;
  protected:

  public:

    /**
     * The default constructor
     *
     */
    SecurityService()
    {
      policyStr = "";
      policy = 0;
      credPtr = 0;
      qStats = 0;

      OpenSSL_add_all_algorithms();

      if (!EVP_get_cipherbyname(SN_des_ede3_cbc))
      {
        EVP_add_cipher(EVP_des_ede3_cbc());
        EVP_add_cipher_alias(SN_des_ede3_cbc,"DES3");
        EVP_add_cipher_alias(SN_des_ede3_cbc,"des3");
      }
    };

    /**
     * The destructor
     *
     */
    ~SecurityService()
    {
      if(policy)
        delete policy;
      policy = 0;

      EVP_cleanup();
    };

    /**
     * Initialize the policy
     */
    void setPolicy(const string & pPolicyStr)
    {
      policyStr = pPolicyStr;
      policy = new Policy(policyStr);
    };

    /**
     * Base 64 encodes a string
     */
    char* encodeB64(const unsigned char* input, int length)
    {
      char* buff = 0;
      BIO* bmem, *b64;
      BUF_MEM *bptr;
      int ret = 0;

      b64 = BIO_new(BIO_f_base64());
      bmem = BIO_new(BIO_s_mem());
      b64 = BIO_push(b64, bmem);
      ret = BIO_write(b64, input, length);
      if(BIO_flush(b64))
      {
        BIO_get_mem_ptr(b64, &bptr);
        buff = new char [(bptr->length)+1];
        memset(buff, 0, bptr->length + 1);
        memcpy(buff, bptr->data, bptr->length);
        buff[bptr->length] = 0;
      }

      BIO_free_all(b64);

      return buff;
    };

    /**
     * Decodes a Bse 64 string
     */
    unsigned char * decodeB64(const unsigned char * input, int & length)
    {
      BIO *bmem, *b64;
      unsigned char* buff = 0;
      int ret = 0;

      b64 = BIO_new(BIO_f_base64());
      bmem = BIO_new(BIO_s_mem());
      ret = BIO_write(bmem, input, length);
      bmem = BIO_push(b64, bmem);
      buff = new unsigned char[3*length];
      memset(buff, 0, 3*length);
      length = BIO_read(bmem, buff, length);
      if(length < 1)
      {
        if(!BIO_should_retry(bmem))
          ERR_print_errors_fp(stderr);
      }

      BIO_free_all(bmem);

      return buff;
    };

    /**
     * Generates a random challenge message
     */
    char* generateChallenge()
    {
      char* buffer = new char [MAX_CHALLENGE_BUF + 1];
      memset(buffer, 0, MAX_CHALLENGE_BUF + 1);
      int nextRand;

      for(int i = 0; i < MAX_CHALLENGE_BUF; i++)
      {
        nextRand = rand();
        if(nextRand % 5 == 0)
          buffer[i] = '@';
        else if(nextRand % 3 == 0)
          buffer[i] = '!';
        else if(nextRand % 2 == 0)
          buffer[i] = '?';
        else
          buffer[i] = '*';
      }

      return buffer;
    }

    /**
     * Constructs an XSB query string
     */
    string generateQuery(string & msg)
    {
      string retVal = "";

      if(msg != "")
      {
        size_t pos = 0, idx;
        idx = msg.find(END_CLIENT_REQUEST_S.c_str());
        pos = msg.find_first_of("request");
        if(idx != string::npos && pos != string::npos)
        {
          msg = msg.substr(pos, idx - pos);
          vector<string> args;
          string tmp = "";
          pos = msg.find_first_of("(");

          while(pos++ != msg.length() && pos != string::npos)
          {
            if(msg[pos] == ',' && tmp != "" || msg[pos] == ')')
            {
              args.push_back(tmp);
              tmp = "";
            }
            else if(msg[pos] != '(' && msg[pos] != ')')
              tmp += msg[pos];
          }

          retVal += "allow(" + credPtr->getKeyDigest() + ",";
          for(size_t i = 0; i < args.size(); i++)
          {
            retVal += args[i];
            if(i + 1 != args.size())
              retVal += ",";
          }
          retVal += ").";
        }
        else
          cerr << "Error: Could not construct query string" << endl;
      }
      return retVal;
    }

    /**
     * Verifies a credential
     */
    bool verifyCredential(Credential* & pCred)
    {
      bool retVal = true;
      Certificate* certPtr = 0;
      size_t j = 0, i = 0;

      if(pCred == 0)
        cerr << "Error: Received Null Credential" << endl << endl;
      else
      {
        credPtr = pCred;
        pCred->setKeyDigest(getKeyDigest(pCred->getPubKey()));
        for(i = 0; i < credPtr->numCertificates(); i++)
        {
          certPtr = credPtr->getCertificates()[i];
          cout << "Verifying Certificate..." << endl << certPtr->getCertStr() << endl
               << endl;

          bool vTime = true;

          if(VALIDATE_CERT_TIME)
          {
            time_t curTime = time(NULL);
            struct tm* now = gmtime(&curTime);
            time_t tmp = mktime(now);
            vTime = tmp >= certPtr->getStartTimeSec() &&
                    tmp < certPtr->getStopTimeSec();
          }

          if(VALIDATE_CERT_TIME && !vTime)
          {
            cerr << "Error: Certificate is not valid at this time" << endl;
            retVal = false;
          }
          else if(verifySig(certPtr->getPubKey(),
                            certPtr->getSignature(),
                            certPtr->getCertStr()))
          {
            cout << "Certificate Verified" << endl << endl;
            certPtr->setKeyDigest(getKeyDigest(certPtr->getPubKey()));
            policy->setKeyDigest(certPtr->getKeyDigest());
            vector<string> clauses = certPtr->getClauses();
            cout << "Transforming Clauses..." << endl << endl;
            for(j = 0; j < clauses.size(); j++)
            {
              string clause = clauses[j];
              string binderClause = transformClause(clause);
              certPtr->addTransformedCls(binderClause);
              cout << "Clause:           " << clause << endl
                   << "Binder Statement: " << binderClause << endl << endl;
            }
          }
          else retVal = false;
        }
      }

      return retVal;
    };

    /**
     * Verifies the signature of a credential or certificate
     */
    bool verifySig(const string & pKey, const string & pB64DgstSigned,
                   const string & pMsg)
    {
      int ret = 0,
          sigLen = 0;
      unsigned char *decSig = 0,
                    *md5Digest = 0;
      bool retVal = true;

      if(pKey == "")
      {
        cerr << "Error: Key must have a value" << endl;
        retVal = false;
      }
      else if(pB64DgstSigned == "")
      {
        cerr << "Error: Signature must have a value" << endl;
        retVal = false;
      }
      else if(pMsg == "")
      {
        cerr << "Error: Message must have a value" << endl;
        retVal = false;
      }
      else
      {
        md5Digest = getMsgDigest(pMsg);
        BIO* in =
          BIO_new_mem_buf((void*) pKey.c_str(), pKey.size());

        if(in)
        {
          ret = BIO_flush(in);
          RSA* pubKey = (RSA*) PEM_read_bio_RSA_PUBKEY(in, 0, 0, NULL);
          if(pubKey)
          {
            sigLen = pB64DgstSigned.size();
            decSig = decodeB64((const unsigned char*)pB64DgstSigned.c_str(), sigLen);

            ret = RSA_verify(
                NID_md5, md5Digest, MD5_DIGEST_LENGTH, decSig, RSA_size(pubKey), pubKey);

            if(!ret)
            {
              ERR_print_errors_fp(stderr);
              retVal = false;
            }

            RSA_free(pubKey);
            if(decSig)
              delete [] decSig;

            decSig = 0;
          }
          else
            ERR_print_errors_fp(stderr);

          BIO_free_all(in);
        }
        else
          ERR_print_errors_fp(stderr);

        if(md5Digest)
          delete [] md5Digest;
      }

      return retVal;
    };

    /**
     * Retrieves the MD5 digest for a message
     */
    unsigned char* getMsgDigest(const string & pMsg)
    {
      const unsigned char* msg = (const unsigned char*)pMsg.c_str();
      return getMsgDigest(msg, strlen(pMsg.c_str()));
    };

    /**
     * Retrieves the MD5 digest for a message
     */
    unsigned char* getMsgDigest(const unsigned char* msg, size_t len)
    {
      unsigned char* md5Digest = 0;

      if(msg)
      {
        md5Digest = new unsigned char [MD5_DIGEST_LENGTH + 1];
        memset(md5Digest, 0, MD5_DIGEST_LENGTH + 1);
        MD5(msg, len, md5Digest);
      }

      return md5Digest;
    };

    /**
     * Retrieves an MD5 Digest of a string in binder form
     */
    string getKeyDigest(const string & pKey)
    {
      string retVal = "";
      int keyLen = pKey.length();
      unsigned char* decKey = decodeB64((const unsigned char*)pKey.c_str(), keyLen);
      unsigned char* md5Digest =
        getMsgDigest((const unsigned char*)decKey, (size_t)keyLen);

      char x[2 * MD5_DIGEST_LENGTH + 1];
      char* tmp = x;
      for(int i = 0; i < MD5_DIGEST_LENGTH; i++)
        sprintf(tmp + (i * 2), "%02x", md5Digest[i]);

      string val(x);
      retVal = "rsa_";
      retVal += val;
      delete [] decKey;
      delete [] md5Digest;

      return retVal;
    };

    /**
     * Transforms a clause into binder form (APPENDIX B of binder Paper)
     *
     */
    string transformClause(string & pClause)
    {
      string retVal = "",
             subject = "",
             verb = "",
             prefix = "says(" + policy->getKeyDigest(),
             predicate = "";

      size_t pos = pClause.find(":-");
      bool isRule = pos != string::npos;

      if(isRule)
      {
        Rule r(pClause);
        bool quoted = r.getBody().find(r.getHead()) != string::npos;
        if(quoted)
          retVal = prefix + "," + r.getHead() + "):-" + r.getBody();
        else
        {
          retVal += prefix + "," + r.getHead() + "):-";
          vector<string> clauses = r.getClauses();

          for(size_t i = 0; i < clauses.size(); i++)
          {
            retVal += prefix + "," + clauses[i] + ")";
            if(i + 1 == clauses.size())
              retVal += ".";
            else
              retVal += ",";
          }
        }
        Rule anotherRule(retVal);
        policy->addRule(retVal);
      }
      else
      {
        retVal = prefix + "," + pClause.substr(0, pClause.length() - 1) + ").";
        policy->addFact(retVal);
      }

      return retVal;
    };


    /**
     * Executes an XSB query
     *
     */
    bool executeQuery(const string & pQuery)
    {
      bool retVal = false;
      qStats = false;

      if(pQuery == "")
        cerr << "Error: Cannot execute empty query" << endl;
      else
      {
        stringstream ss;
        ss << "xsb --quietload --nobanner --noprompt -e "
           << "\"[" << TMP_FILE_S << "]. " << pQuery << " halt.\"";
        char q [ss.str().length() + 1];
        memset(q, 0, ss.str().length() +1);
        memcpy(q, ss.str().c_str(), ss.str().length());
        cout << "Executing query: " << q << endl;
        pthread_t queryProcThread;

        if(pthread_create(&queryProcThread, 0, executeQueryProc, (void*)&q) == 0)
        {
          mutex.lock();
          Timer t;
          double elapsed = 0;
          t.start();
          while(!qStats && (elapsed += t.getElapsedSecs()) < 1) //{;}
          {
            ;//cout << "Elapsed: " << elapsed << '\n';
          }
          t.stop();
          //if for some reason this hasn't returned yet kill it
          pthread_detach(queryProcThread);
          pthread_kill(queryProcThread, SIGKILL);
          mutex.unlock();
        }

        retVal = qStats;
      }

      return retVal;
    };

    /**
     * Writes a policy and transformed clauses to a file
     */
    bool writeBinderFile()
    {
      bool retVal = true;
      ofstream out;
      Certificate* certPtr = 0;
      string fName = TMP_FILE_S + ".P";
      out.open(fName.c_str());

      if(out.is_open() && credPtr)
      {
        out << policy->getPolicyStr() << endl << "/* Begin Transformed Rules */"
          << endl;

        for(size_t i = 0; i < credPtr->numCertificates(); i++)
        {
          certPtr = credPtr->getCertificate(i);
          for(size_t j = 0; j < certPtr->numTransformedCls(); j++)
            out << certPtr->getTransformedCls(j) << endl;
        }

        out.close();
        out.clear();
      }
      else
      {
        cerr << "Error: Could not open file [" << fName << "] for writing" << endl;
        retVal = false;
      }

      return retVal;
    };
};
#endif //__SecurityService__

