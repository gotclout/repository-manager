#ifndef __POLICY__
#define __POLICY__

#include "Rule.h"
#include "RepositoryManagerUtils.h"

/**
 * A structure for representing Binder Policies
 */
class Policy
{
  private:

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

  protected:

  public:

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
   * Adds a rule to the rules container
   */
  void addRule(const string & r) { rules.push_back(r); };

  /**
   * Retrieves the key digest for the policy
   */
  const string& getKeyDigest() const { return keyDigest; };

  /**
   * Sets the key digest value for this policy
   */
  void setKeyDigest(const string & kd) { keyDigest = kd; };

  /**
   * Retrieve the string representation of the policy
   */
  const string& getPolicyStr() const { return policyStr; };

  /**
   * Retrieve the container of known facts
   */
  vector<string>& getFacts() { return facts; };

  /**
   * Add a fact to the facts container
   */
  void addFact(const string & f) { facts.push_back(f); };

  /**
   * Retrieves the string value of the policy
   */
  string toString()
  {
    string retVal = "Rendering Policy\n";
    size_t i = 0;

    retVal += "Rules\n";
    for(; i < rules.size(); i++)
      retVal += rules[i].getAtom() + "\n";
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

#endif//__POLICY__

