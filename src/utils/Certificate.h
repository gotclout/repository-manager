#ifndef __CERTIFICATE__
#define __CERTIFICATE__

#include "RepositoryManagerUtils.h"

/**
 * A structure used for representing certificates
 */
class Certificate
{
  private:

  vector<string>

    /** A certificate is a string of clauses stored here **/
    clauses,

    /** Clauses are transformed into prolog/binder statements stored here **/
    transformedCls;


  /** The time fram for which a Certificate is valid **/
  long long startTimeSec, stopTimeSec;

  string
    /** Certifactes are valid in a period indicating start and end times **/
    startTime, stopTime,

    /** Certificates are associated with a signature stored here **/
    signature,

    /** The public key of the signing entity is stored here **/
    pubKey,

    /** The binder/prolog representation of the public key is stored here **/
    keyDigest,

    /** Certifcate clauses as a single string is stored here **/
    certStr;

  protected:

  public:

  /** The default constructor for a certificate **/
  Certificate() { certStr = ""; pubKey = ""; keyDigest = ""; certStr = ""; };

  /**
   * Retrieves the container of clauses for this certificate
   */
  vector<string>& getClauses() { return clauses; };

  /**
   * Retrieves the container of transformed clauses for this certificate
   */
  vector<string>& getTransformedCluses() { return transformedCls; };

  /**
   * Adds a clause to the clauses container
   */
  void addClause(string c) { clauses.push_back(c); };

  /**
   * Retrieves the specified transformed clause
   */
  string getTransformedCls(size_t i)
  {
    return i >= transformedCls.size() ? "" : transformedCls[i];
  };

  /**
   * Retrieves the number of transformed clauses
   */
  size_t numTransformedCls() { return transformedCls.size(); };

  /**
   * Adds a string to the transformed clause container
   */
  void addTransformedCls(const string & tc) { transformedCls.push_back(tc); };

  /**
   * Retrieves the certificate as a string
   */
  const string& getCertStr() const { return certStr; };

  /**
   *
   */
  void appendToPubKey(string s) { pubKey += s; };

  /**
   *
   */
  void appendToSignature(string s) { signature += s; };

  /**
   *
   */
  void appendToStartTime(string s) { startTime += s; };

  /**
   *
   */
  void appendToStopTime(string s) { stopTime += s; };

  /**
   *
   */
  void setStartTime(string s) { startTime = s; };

  /**
   *
   */
  void setStopTime(string s) { stopTime = s; };

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

  /**
   *
   */
  const long long getStartTimeSec() const { return startTimeSec; };

  /**
   *
   */
  const long long getStopTimeSec() const { return stopTimeSec; };

  /**
   *
   */
  const string& getStartTime() const { return startTime; };

  /**
   *
   */
  const string& getStopTime() const { return stopTime; };

  /**
   *
   */
  const string& getPubKey() const { return pubKey; };

  /**
   *
   */
  const string& getSignature() const { return signature; };

  /**
   *
   */
  const string& getKeyDigest() const { return keyDigest; };

  /**
   *
   */
  void setKeyDigest(const string & kd) { keyDigest = kd; };

};
#endif//__CERTIFICATE__

