#ifndef __CREDENTIAL__
#define __CREDENTIAL__

#include "Certificate.h"
#include "Rule.h"

using std::string;

/**
 * A representation of a client credentail
 */
class Credential
{
  private:

  string
    /** A credential has a public key **/
    pubKey,
    /** The binder/prolog representation of the key digest **/
    keyDigest;

  /** A credential has one or more certificates **/
  vector<Certificate*> certificates;

  protected:

  public:

  /**
   * The default constructor for a Credential
   */
  Credential()
  {
    pubKey = "";
  };

  /**
   * Retrieve the Key Digest
   */
  const string& getKeyDigest() const { return keyDigest; };

  /**
   * Set the Key Digest
   */
  void setKeyDigest(const string & k) { keyDigest = k; };

  /**
   * Retreive the public key
   */
  const string& getPubKey() const { return pubKey; };

  /**
   * Append a public key
   */
  void appendToPubKey(string s) { pubKey += s; };

  /**
   * Sets the credentials pub key
   */
  inline void setPubKey(const string & pk) { pubKey = pk; };

  /**
   * Retrieves the credentials certificates
   */
  vector<Certificate*>& getCertificates() { return certificates; };

  /**
   * Adds a certificate to the credential
   */
  void addCertificate(Certificate* & c) { if(c) { certificates.push_back(c);} };

  /**
   * Retrieves the specified certificate
   */
  Certificate* getCertificate(size_t i)
  {
    return i >= certificates.size() ? 0 : certificates[i];
  };

  /**
   * Retrieves the number of certificates
   */
  size_t numCertificates() { return certificates.size(); };

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
#endif//__CREDENTAIL__

