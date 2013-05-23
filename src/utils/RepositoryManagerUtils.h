#ifndef __RepositoryManagerUtils__
#define __RepositoryManagerUtils__

#define cls(sstr) \
  sstr.str(""); \
  sstr.clear();

#include <pthread.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include "Timer.h"
//#include <time.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>

#include <stdio.h>
#include <string.h>
#include <pthread.h>

#define MAX_CHALLENGE_BUF 32
#define MAX_READ 128
#define MAX_LINE 128
#define MAX_SEED_BUF 64
#define KEY_LEN 1024
#define MAX_CRED_LEN 51200
#define MAX_CONNECT 64
#define MAX_BUFF 51200

using namespace std;

const static string PK_HEADER_S = "-----BEGIN PUBLIC KEY-----";
const static string PK_END_S = "-----END PUBLIC KEY-----";
const static string SIG_S = "-----BEGIN SIGNATURE-----";
const static string END_SIG_S = "-----END SIGNATURE-----";
const static string CLAUSE_S = "CLAUSE";
const static string START_TIME_S = "START_TIME";
const static string STOP_TIME_S = "STOP_TIME";
const static string CERT_S = "CERT";
const static string END_CERT_S = "END_CERT";
const static string CREDENTIAL_S = "CREDENTIAL";
const static string END_S = "END";
const static char* LINE_S = "-------------------------------";
const static string BEGIN_CHALLENGE_S = "-----BEGIN MPKI CHALLENGE-----";
const static string END_CHALLENGE_S = "-----END MPKI CHALLENGE-----";
const static string BEGIN_RESPONSE_S = "-----BEGIN MPKI CHALLENGE RESPONSE-----";
const static string END_RESPONSE_S = "-----END MPKI CHALLENGE RESPONSE-----";
const static string BEGIN_CLIENT_REQUEST_S = "-----BEGIN REPOSITORY CLIENT REQUEST-----";
const static string END_CLIENT_REQUEST_S = "-----END REPOSITORY CLIENT REQUEST-----";
const static string BEGIN_SERVER_RESPONSE_S = "-----BEGIN REPOSITORY SERVER REPLY-----";
const static string END_SERVER_RESPONSE_S = "-----END REPOSITORY SERVER REPLY-----";
const static string RSA_S = "rsa_";
const static string GRANT_S = "granted";
const static string DENY_S = "denied";
const static string TMP_FILE_S = "rlf_tmp";
const static string FIN_S = "exit";

static bool VALIDATE_CERT_TIME = false;

/**
 * Trim white space and comments from strings
 */
string trim(const string pStr)
{
  stringstream ss;
  string retVal;
  size_t pos, idx;

  for(size_t i = 0; i < pStr.length(); i++)
  {
    if(pStr[i] != ' ' && pStr[i] != '\n' && pStr[i] != '\t' && pStr[i] != '\r')
      ss << pStr[i];
  }

  retVal = ss.str();

  if((pos = retVal.find("/*")) != string::npos)
  {
    idx = retVal.find("*/");
    retVal = retVal.substr(idx + 2, retVal.length() - 1);
  }

  return retVal;
}


/**
 * Convert a date time predicate to utc sec
 */
long getUTCTimeSec(string & pTimeStr)
{
  long retVal = 0;
  time_t curTime = time(NULL);
  struct tm* now = gmtime(&curTime);

  size_t pos = pTimeStr.find("(") + 1,
         idx = pTimeStr.find(",");

  stringstream ss;

  ss << pTimeStr.substr(pos, idx - pos);
  ss >> now->tm_year;
  now->tm_year -= 1900;
  cls(ss);

  idx++;
  pos = idx;
  idx = pTimeStr.find(",", idx);
  ss << pTimeStr.substr(pos, idx - pos);
  ss >> now->tm_mon;
  now->tm_mon -= 1;
  cls(ss);

  idx++;
  pos = idx;
  idx = pTimeStr.find(",", idx);
  ss << pTimeStr.substr(pos, idx - pos);
  ss >> now->tm_mday;
  cls(ss);

  idx++;
  pos = idx;
  idx = pTimeStr.find(",", idx);
  ss << pTimeStr.substr(pos, idx - pos);
  ss >> now->tm_hour;
  cls(ss);

  idx++;
  pos = idx;
  idx = pTimeStr.find(",", idx);
  ss << pTimeStr.substr(pos, idx - pos);
  ss >> now->tm_min;
  cls(ss);

  idx++;
  pos = idx;
  idx = pTimeStr.find(")", idx);
  ss << pTimeStr.substr(pos, idx - pos);
  ss >> now->tm_sec;
  cls(ss);

  time_t tmp = mktime(now);

  retVal = (long) tmp;

  return retVal;
};

/**
 *
 */
class RepositoryManagerUtils
{
  private:

  protected:

  public:
};

#endif//__RepositoryManagerUtils__
