#ifndef __RULE__
#define __RULE__

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

/**
 * A structure for representing prolog rule expressions
 */
class Rule
{
  private:

  /** A rule atom consists of a body and head **/
  string head, body, atom;

  /** The body consists of predicates/clauses **/
  vector<string> clauses;

  protected:

  public:

  /**
   * Default constructor
   */
  Rule()
  {
    head = body = atom = "";
  };

  /**
   *
   */
  const string& getAtom() const { return atom; };

  /**
   *
   */
  const string& getHead() const { return head; };

  /**
   *
   */
  const string& getBody() const { return body; };

  /**
   *
   */
  void setAtom(const string & a) { atom = a; };

  /**
   *
   */
  void setHead(const string & h) { head = h; };

  /**
   *
   */
  void setBody(const string & b) { body = b; };

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

  friend ostream& operator <<(ostream & o, Rule & r)
  {
    o << r.toString();
    return o;
  };
};
#endif//__RULE__

