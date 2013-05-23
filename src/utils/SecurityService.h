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

#include "RepositoryManagerUtils.h"

using namespace std;

/**
 * A structure for representing prolog rule expressions
 */
struct Rule
{
  /** A rule atom consists of a body and head **/
  string head, body, atom;

  /** The body consists of predicates/clauses **/
  vector<string> clauses;

  /**
   * Retrieves the clauses from a predicate
   */
  vector<string> getClauses()
  {
    if(clauses.size() < 1)
    {
      string tmp = body;
      bool end = false;
      size_t pos = tmp.find("),");
      while(!end)
      {
        if(pos != string::npos)
        {
          clauses.push_back(tmp.substr(0, pos + 1));
          tmp = tmp.substr(pos + 2, tmp.length() - pos - 2);
        }
        else
        {
          clauses.push_back(tmp.substr(0, tmp.length() - 1));
          end = true;
        }
        pos = tmp.find("),");
      }
    }

    return clauses;
  };

  /**
   * A rule constructor
   */
  Rule(const string & r)
  {
    size_t imp = r.find(":-");
    atom = r;
    head = r.substr(0, imp);
    body = r.substr(imp + 2, r.length() - 1);
    getClauses();
  };

  /**
   * Retrieves the string representation of a rule
   */
  string toString()
  {
    string retVal = "Rendering Rule For Atom:\n" + atom
      + "\nHead: " + head + "\n" + "Body: " + body + "\n";
    for(size_t i = 0; i < clauses.size(); i++)
      retVal += clauses[i] + "\n";

    return retVal;
  };

};

/**
 * A structur for representing Binder Policies
 */
struct Policy
{
  string
    /** Policy Key **/
    pubKey,

    /** Poicy File String **/
    policyStr,

    /** MD5 Key digest **/
    keyDigest;

  /** Policy facts as binder/prolog statemnts **/
  vector<string> facts;

  /** A list of rules **/
  vector<Rule> rules;

  /**
   * Default policy constructor
   */
  Policy(const string & pPolicyStr)
  {
    if(pPolicyStr != "")
    {
      policyStr = pPolicyStr;
      getAtoms();
    }
  };

  /**
   * Retrieves the string value of the policy
   */
  string toString()
  {
    string retVal = "Rendering Policy\n";
    size_t i = 0;

    retVal += "Rules\n";
    for(; i < rules.size(); i++)
      retVal += rules[i].atom + "\n";
    retVal+= "\nFacts\n";
    for(i = 0; i < facts.size(); i++)
      retVal += facts[i] + "\n";

    return retVal;
  };

  /**
   * Output stream operator overload
   */
  friend ostream& operator << (ostream & o, Policy & p)
  {
    o << p.toString();
    return o;
  }

  /**
   * Retrieves the substring from the end of the delimiter in the source string
   */
  string getsub(const string & source, string delim)
  {
    string retVal = "";

    if(source != "" && delim != "")
    {
      size_t pos = source.find(delim),
             dif = source.length() - delim.length();
      if(pos != string::npos)
        retVal = source.substr(pos + delim.length(), dif);
    }
    return retVal;
  };

  /**
   * Retreives all atoms of the policy
   */
  void getAtoms()
  {
    size_t pos = 0, idx = 0, imp = 0;
    string tmp = policyStr;

    pos = policyStr.find_first_of("(");

    while(!isspace(policyStr[pos])) pos--;

    tmp = policyStr.substr(pos, policyStr.length() - pos - 1);

    while((idx = tmp.find_first_of(".")) != string::npos)
    {
      string atom = trim(tmp.substr(0, idx + 1));

      if((imp = atom.find(":-")) != string::npos)
      {
        Rule r(atom);
        rules.push_back(r);
      }
      else
      {
        facts.push_back(atom);
      }

      tmp = getsub(tmp, ".");
    }
  };
};

/**
 * A structure used for representing certificates
 *
 */
struct Certificate
{
  vector<string>

    /** A certificate is a string of clauses stored here **/
    clauses,

    /** Clauses are transformed into prolog/binder statements stored here **/
    transformedCls;


  /** The time fram for which a Certificate is valid **/
  long startTimeSec, stopTimeSec;

  string
    /** Certifactes are valid in a period indicating start and end times **/
    startTime, stopTime,

    /** Certificates are associated with a signature stored here **/
    signature,

    /** The public key of the signing entity is stored here **/
    pubKey,

    /** The binder/prolog representation of the punlic key is stored here **/
    keyDigest,

    /** Certifcate clauses as a single string is stored here **/
    certStr;

  /** The default constructor for a certificate **/
  Certificate() { certStr = ""; };

  /**
   * Retrieves the string representation of a certificate
   *
   */
  string toString()
  {
    stringstream sstr;

    if(certStr == "")
      setCertificateStr();

    sstr << "Certificate String: " << endl << certStr << endl
      << "From: " << endl << startTime << endl << "To: " << endl
      << stopTime << endl << "Public Key: " << endl << pubKey << endl
      << "Signature: " << endl << signature
      << endl;

    return sstr.str();
  };

  /**
   * Output stream operator overload
   */
  friend ostream& operator << (ostream & o, Certificate & c)
  {
    o << c.toString();
    return o;
  };

  /**
   * Conactenates the certificate clauses into a single string
   *
   */
  void setCertificateStr()
  {
    if(certStr == "")
    {
      for(size_t i = 0; i < clauses.size(); i++)
        certStr += clauses[i];
      certStr += startTime + stopTime;
    }

    if(startTime != "")
      startTimeSec = getUTCTimeSec(startTime);
    else
      startTimeSec = 0;
    if(stopTime != "")
      stopTimeSec = getUTCTimeSec(stopTime);
    else
      stopTimeSec = 0;
  };
};

/**
 * A representation of a client credentail
 */
struct Credential
{
  string
    /** A credential has a public key **/
    pubKey,
    /** The binder/prolog representation of the key digest **/
    keyDigest;

  /** A credential has one or more certificates **/
  vector<Certificate*> certificates;

  /**
   * The default constructor for a Credential
   */
  Credential()
  {
    pubKey = "";
  };

  /**
   * Retrieves the string representation of a Credential
   */
  string toString()
  {
    stringstream sstr;

    sstr << "Redering Credential" << endl << "Public Key" << endl << pubKey
      << endl << "Num Certificates: " << certificates.size() << endl;
    for(size_t i = 0; i < certificates.size(); i++)
    {
      sstr << "Rendering Certificate " << i + 1 << endl;
      sstr << *certificates[i] << endl;
    }

    return sstr.str();
  };

  /**
   * Outputstream operator overload
   */
  friend ostream& operator << (ostream & o, Credential & c)
  {
    o << c.toString();
    return o;
  };

  /**
   * Finalize all unset values of the Credentials Certificates
   */
  void finalize()
  {
    for(size_t i = 0; i < certificates.size(); i++)
      certificates[i]->setCertificateStr();
  };

  /**
   * A destructor for a Credential
   */
  ~Credential()
  {
    for(size_t i = 0; i < certificates.size(); i++)
    {
      Certificate* cert = certificates[i];
      delete cert;
      cert = 0;
    }
  };
};


static bool qStats;

/**
 *
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
    cerr << "could not execute query" << endl;

  pthread_exit(0);
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

    /** Query result status **/

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

          retVal += "allow(" + credPtr->keyDigest + ",";
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
        pCred->keyDigest = getKeyDigest(pCred->pubKey);
        for(i = 0; i < credPtr->certificates.size(); i++)
        {
          certPtr = credPtr->certificates[i];
          cout << "Verifying Certificate..." << endl << certPtr->certStr << endl
            << endl;

          bool vTime = true;

          if(VALIDATE_CERT_TIME)
          {
            time_t curTime = time(NULL);
            struct tm* now = gmtime(&curTime);
            time_t tmp = mktime(now);
            vTime = tmp >= certPtr->startTimeSec && tmp <= certPtr->stopTimeSec;
          }

          if(VALIDATE_CERT_TIME && !vTime)
          {
            cerr << "Error: Certificate is not valid at this time" << endl;
            retVal = false;
          }
          else if(verifySig(certPtr->pubKey, certPtr->signature, certPtr->certStr ))
          {
            cout << "Certificate Verified" << endl << endl;
            certPtr->keyDigest = getKeyDigest(certPtr->pubKey);
            policy->keyDigest = certPtr->keyDigest;
            vector<string> clauses = certPtr->clauses;
            cout << "Transforming Clauses..." << endl << endl;
            for(j = 0; j < clauses.size(); j++)
            {
              string clause = clauses[j];
              string binderClause = transformClause(clause);
              certPtr->transformedCls.push_back(binderClause);
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
    bool verifySig(string pKey, string & pB64DgstSigned, string & pMsg)
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

      char* x = new char[2 * MD5_DIGEST_LENGTH + 1];
      char* tmp = x;
      for(int i = 0; i < MD5_DIGEST_LENGTH; i++)
        sprintf(tmp + (i * 2), "%02x", md5Digest[i]);

      string val(x);
      retVal = "rsa_";
      retVal += val;
      delete [] decKey;
      delete [] x;
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
             prefix = "says(" + policy->keyDigest,
             predicate = "";

      size_t pos = pClause.find(":-");
      bool isRule = pos != string::npos;

      if(isRule)
      {
        Rule r(pClause);
        bool quoted = r.body.find(r.head) != string::npos;
        if(quoted)
          retVal = prefix + "," + r.head + "):-" + r.body;
        else
        {
          retVal += prefix + "," + r.head + "):-";
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
        policy->rules.push_back(retVal);
      }
      else
      {
        retVal = prefix + "," + pClause.substr(0, pClause.length() - 1) + ").";
        policy->facts.push_back(retVal);
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
          Timer t;
          t.start();
          while(t.elapsed() < 500000){;}
          t.stop();
          //if for some reason this hasn't returned yet kill it
          pthread_detach(queryProcThread);
          pthread_kill(queryProcThread, SIGKILL);
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
        out << policy->policyStr << endl << "/* Begin Transformed Rules */"
          << endl;

        for(size_t i = 0; i < credPtr->certificates.size(); i++)
        {
          certPtr = credPtr->certificates[i];
          for(size_t j = 0; j < certPtr->transformedCls.size(); j++)
            out << certPtr->transformedCls[j] << endl;
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
