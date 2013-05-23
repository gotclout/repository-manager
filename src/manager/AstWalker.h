#ifndef __AstWalker__
#define __AstWalker__

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "../utils/SecurityService.h"
#include "../utils/RepositoryManagerUtils.h"
#include "../parser/RepositoryManagerParser.h"

using namespace std;

/**
 * The AstWalker Class is used to construct Credentials by extracting data from
 * an antlr constructed Abstract Syntax Tree
 */
class AstWalker
{
  private:

    /** number child nodes of the tree root **/
    int numChildren;

    /** a string stream for printing **/
    stringstream sstr;

    /** the label of a node **/
    string label;

    /** an antlr string **/
    pANTLR3_STRING antStr;

    /** the base of the antlr tree or root node **/
    pANTLR3_BASE_TREE tree;

  protected:


  public:

    /**
     * Default constructor
     */
    AstWalker(pANTLR3_BASE_TREE pTree)
    {
      tree = pTree;
      numChildren = (ANTLR3_UINT32) tree->getChildCount(tree);
    };

    /**
     * Retrieves the string value of the first child node of the tree at the
     * specified index
     */
    string getChildStr(const int & pIdx)
    {
      cls(sstr);
      pANTLR3_BASE_TREE child =
        (pANTLR3_BASE_TREE) tree->getChild(tree, (ANTLR3_UINT32) pIdx);
      if(child)
      {
        antStr = (pANTLR3_STRING) child->toString(child);
        sstr << antStr->chars;
      }
      else
        cerr << "Error: NULL Child in AST" << endl;

      return sstr.str();
    }

    /**
     * Retreives the string value of a clause from the child node of the tree
     * at the specified index
     */
    string getClauseStr(int & pIdx)
    {
      string retVal = "";

      do
        retVal = retVal + getChildStr(pIdx);
      while(getChildStr(pIdx++) != ".");

      return retVal;
    };

    /**
     * Constructs a certificate
     */
    Certificate* makeCertificate(Credential* & pCredential, int & pIdx)
    {
      Certificate* retVal;
      string tmp = "";

      if(!pCredential)
        retVal = 0;
      else
      {
        retVal = new Certificate();

        while(pIdx < numChildren && getChildStr(pIdx++) != END_CERT_S)
        {
          label = sstr.str();

          if(label == CLAUSE_S)
          {
            while(getChildStr(pIdx) != START_TIME_S)
              retVal->clauses.push_back(getClauseStr(pIdx));
          }
          else if(label == START_TIME_S)
          {
            retVal->startTime += getClauseStr(pIdx);
          }
          else if(label == STOP_TIME_S)
          {
            retVal->stopTime += getClauseStr(pIdx);
          }
          else if(label == PK_HEADER_S)
          {
            retVal->pubKey += label;
            while(getChildStr(pIdx++) != SIG_S)
              retVal->pubKey += "\n" + sstr.str();
            pIdx--;
          }
          else if(label == SIG_S)
          {
            retVal->signature += label;
            while(getChildStr(pIdx++) != END_SIG_S)
              retVal->signature += "\n" + sstr.str();
            retVal->signature += "\n" + END_SIG_S;
          }
          else
            pIdx++;
        }
      }

      return retVal;
    };

    /**
     * Constructs a credential
     */
    Credential* makeCredential(Credential* & pCredential, int & pIdx)
    {
      pCredential = new Credential();

      while(pIdx < numChildren && label != END_S)
      {
        getChildStr(pIdx++);
        label = sstr.str();

        if(label == PK_HEADER_S)
        {
          pCredential->pubKey += label;
          while(getChildStr(pIdx) != CERT_S)
            pCredential->pubKey += "\n" + getChildStr(pIdx++);
        }
        else if(label == CERT_S)
        {
          Certificate* cert = makeCertificate(pCredential, pIdx);
          if(cert)
            pCredential->certificates.push_back(cert);
          getChildStr(--pIdx);
          label = sstr.str();
        }
      }

      return pCredential;
    };

    /**
     * Retrieves a credential
     */
    Credential* getCredential()
    {
      Credential* retVal = 0;
      int childIdx = 0;

      antStr = (pANTLR3_STRING) tree->toString(tree);
      sstr << antStr->chars;

      if(sstr.str() == CREDENTIAL_S)
      {
        retVal = makeCredential(retVal, childIdx);
        retVal->finalize();
      }

      return retVal;
    };
};

#endif//__AstWalker__
