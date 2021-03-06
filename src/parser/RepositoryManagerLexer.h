/** \file
 *  This C header file was generated by $ANTLR version 3.4
 *
 *     -  From the grammar source file : /home/stbadmin/repository-manager/src/parser/RepositoryManager.g
 *     -                            On : 2016-01-20 16:04:36
 *     -                 for the lexer : RepositoryManagerLexerLexer
 *
 * Editing it, at least manually, is not wise.
 *
 * C language generator and runtime by Jim Idle, jimi|hereisanat|idle|dotgoeshere|ws.
 *
 *
 * The lexer 
RepositoryManagerLexer

has the callable functions (rules) shown below,
 * which will invoke the code for the associated rule in the source grammar
 * assuming that the input stream is pointing to a token/text stream that could begin
 * this rule.
 *
 * For instance if you call the first (topmost) rule in a parser grammar, you will
 * get the results of a full parse, but calling a rule half way through the grammar will
 * allow you to pass part of a full token stream to the parser, such as for syntax checking
 * in editors and so on.
 *
 * The parser entry points are called indirectly (by function pointer to function) via
 * a parser context typedef pRepositoryManagerLexer, which is returned from a call to RepositoryManagerLexerNew().
 *
 * As this is a generated lexer, it is unlikely you will call it 'manually'. However
 * the methods are provided anyway.
 *
 * The methods in pRepositoryManagerLexer are  as follows:
 *
 *  - 
 void
      pRepositoryManagerLexer->T__39(pRepositoryManagerLexer)
 *  - 
 void
      pRepositoryManagerLexer->T__40(pRepositoryManagerLexer)
 *  - 
 void
      pRepositoryManagerLexer->T__41(pRepositoryManagerLexer)
 *  - 
 void
      pRepositoryManagerLexer->T__42(pRepositoryManagerLexer)
 *  - 
 void
      pRepositoryManagerLexer->T__43(pRepositoryManagerLexer)
 *  - 
 void
      pRepositoryManagerLexer->T__44(pRepositoryManagerLexer)
 *  - 
 void
      pRepositoryManagerLexer->T__45(pRepositoryManagerLexer)
 *  - 
 void
      pRepositoryManagerLexer->T__46(pRepositoryManagerLexer)
 *  - 
 void
      pRepositoryManagerLexer->WS(pRepositoryManagerLexer)
 *  - 
 void
      pRepositoryManagerLexer->NUM(pRepositoryManagerLexer)
 *  - 
 void
      pRepositoryManagerLexer->TEXT(pRepositoryManagerLexer)
 *  - 
 void
      pRepositoryManagerLexer->CREDENTIAL_HEADER(pRepositoryManagerLexer)
 *  - 
 void
      pRepositoryManagerLexer->CREDENTIAL_END(pRepositoryManagerLexer)
 *  - 
 void
      pRepositoryManagerLexer->PUBLIC_KEY_HEADER(pRepositoryManagerLexer)
 *  - 
 void
      pRepositoryManagerLexer->PUBLIC_KEY_END(pRepositoryManagerLexer)
 *  - 
 void
      pRepositoryManagerLexer->SIGNATURE_HEADER(pRepositoryManagerLexer)
 *  - 
 void
      pRepositoryManagerLexer->SIGNATURE_END(pRepositoryManagerLexer)
 *  - 
 void
      pRepositoryManagerLexer->VALIDITY_HEADER(pRepositoryManagerLexer)
 *  - 
 void
      pRepositoryManagerLexer->VALIDITY_END(pRepositoryManagerLexer)
 *  - 
 void
      pRepositoryManagerLexer->CERT_HEADER(pRepositoryManagerLexer)
 *  - 
 void
      pRepositoryManagerLexer->CERT_END(pRepositoryManagerLexer)
 *  - 
 void
      pRepositoryManagerLexer->CHALLENGE_HEADER(pRepositoryManagerLexer)
 *  - 
 void
      pRepositoryManagerLexer->CHALLENGE_END(pRepositoryManagerLexer)
 *  - 
 void
      pRepositoryManagerLexer->RESPONSE_HEADER(pRepositoryManagerLexer)
 *  - 
 void
      pRepositoryManagerLexer->RESPONSE_END(pRepositoryManagerLexer)
 *  - 
 void
      pRepositoryManagerLexer->REQUEST_HEADER(pRepositoryManagerLexer)
 *  - 
 void
      pRepositoryManagerLexer->REQUEST_END(pRepositoryManagerLexer)
 *  - 
 void
      pRepositoryManagerLexer->REPLY_HEADER(pRepositoryManagerLexer)
 *  - 
 void
      pRepositoryManagerLexer->REPLY_END(pRepositoryManagerLexer)
 *  - 
 void
      pRepositoryManagerLexer->NOT_BEFORE(pRepositoryManagerLexer)
 *  - 
 void
      pRepositoryManagerLexer->NOT_AFTER(pRepositoryManagerLexer)
 *  - 
 void
      pRepositoryManagerLexer->Tokens(pRepositoryManagerLexer)
 *
 * The return type for any particular rule is of course determined by the source
 * grammar file.
 */
// [The "BSD license"]
// Copyright (c) 2005-2009 Jim Idle, Temporal Wave LLC
// http://www.temporal-wave.com
// http://www.linkedin.com/in/jimidle
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. The name of the author may not be used to endorse or promote products
//    derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef	_RepositoryManagerLexer_H
#define _RepositoryManagerLexer_H
/* =============================================================================
 * Standard antlr3 C runtime definitions
 */
#include    <antlr3.h>

/* End of standard antlr 3 runtime definitions
 * =============================================================================
 */

#ifdef __cplusplus
extern "C" {
#endif

// Forward declare the context typedef so that we can use it before it is
// properly defined. Delegators and delegates (from import statements) are
// interdependent and their context structures contain pointers to each other
// C only allows such things to be declared if you pre-declare the typedef.
//
typedef struct RepositoryManagerLexer_Ctx_struct RepositoryManagerLexer, * pRepositoryManagerLexer;






#ifdef	ANTLR3_WINDOWS
// Disable: Unreferenced parameter,							- Rules with parameters that are not used
//          constant conditional,							- ANTLR realizes that a prediction is always true (synpred usually)
//          initialized but unused variable					- tree rewrite variables declared but not needed
//          Unreferenced local variable						- lexer rule declares but does not always use _type
//          potentially unitialized variable used			- retval always returned from a rule
//			unreferenced local function has been removed	- susually getTokenNames or freeScope, they can go without warnigns
//
// These are only really displayed at warning level /W4 but that is the code ideal I am aiming at
// and the codegen must generate some of these warnings by necessity, apart from 4100, which is
// usually generated when a parser rule is given a parameter that it does not use. Mostly though
// this is a matter of orthogonality hence I disable that one.
//
#pragma warning( disable : 4100 )
#pragma warning( disable : 4101 )
#pragma warning( disable : 4127 )
#pragma warning( disable : 4189 )
#pragma warning( disable : 4505 )
#pragma warning( disable : 4701 )
#endif

/** Context tracking structure for 
RepositoryManagerLexer

 */
struct RepositoryManagerLexer_Ctx_struct
{
    /** Built in ANTLR3 context tracker contains all the generic elements
     *  required for context tracking.
     */
    pANTLR3_LEXER    pLexer;

     void
     (*mT__39)	(struct RepositoryManagerLexer_Ctx_struct * ctx);

     void
     (*mT__40)	(struct RepositoryManagerLexer_Ctx_struct * ctx);

     void
     (*mT__41)	(struct RepositoryManagerLexer_Ctx_struct * ctx);

     void
     (*mT__42)	(struct RepositoryManagerLexer_Ctx_struct * ctx);

     void
     (*mT__43)	(struct RepositoryManagerLexer_Ctx_struct * ctx);

     void
     (*mT__44)	(struct RepositoryManagerLexer_Ctx_struct * ctx);

     void
     (*mT__45)	(struct RepositoryManagerLexer_Ctx_struct * ctx);

     void
     (*mT__46)	(struct RepositoryManagerLexer_Ctx_struct * ctx);

     void
     (*mWS)	(struct RepositoryManagerLexer_Ctx_struct * ctx);

     void
     (*mNUM)	(struct RepositoryManagerLexer_Ctx_struct * ctx);

     void
     (*mTEXT)	(struct RepositoryManagerLexer_Ctx_struct * ctx);

     void
     (*mCREDENTIAL_HEADER)	(struct RepositoryManagerLexer_Ctx_struct * ctx);

     void
     (*mCREDENTIAL_END)	(struct RepositoryManagerLexer_Ctx_struct * ctx);

     void
     (*mPUBLIC_KEY_HEADER)	(struct RepositoryManagerLexer_Ctx_struct * ctx);

     void
     (*mPUBLIC_KEY_END)	(struct RepositoryManagerLexer_Ctx_struct * ctx);

     void
     (*mSIGNATURE_HEADER)	(struct RepositoryManagerLexer_Ctx_struct * ctx);

     void
     (*mSIGNATURE_END)	(struct RepositoryManagerLexer_Ctx_struct * ctx);

     void
     (*mVALIDITY_HEADER)	(struct RepositoryManagerLexer_Ctx_struct * ctx);

     void
     (*mVALIDITY_END)	(struct RepositoryManagerLexer_Ctx_struct * ctx);

     void
     (*mCERT_HEADER)	(struct RepositoryManagerLexer_Ctx_struct * ctx);

     void
     (*mCERT_END)	(struct RepositoryManagerLexer_Ctx_struct * ctx);

     void
     (*mCHALLENGE_HEADER)	(struct RepositoryManagerLexer_Ctx_struct * ctx);

     void
     (*mCHALLENGE_END)	(struct RepositoryManagerLexer_Ctx_struct * ctx);

     void
     (*mRESPONSE_HEADER)	(struct RepositoryManagerLexer_Ctx_struct * ctx);

     void
     (*mRESPONSE_END)	(struct RepositoryManagerLexer_Ctx_struct * ctx);

     void
     (*mREQUEST_HEADER)	(struct RepositoryManagerLexer_Ctx_struct * ctx);

     void
     (*mREQUEST_END)	(struct RepositoryManagerLexer_Ctx_struct * ctx);

     void
     (*mREPLY_HEADER)	(struct RepositoryManagerLexer_Ctx_struct * ctx);

     void
     (*mREPLY_END)	(struct RepositoryManagerLexer_Ctx_struct * ctx);

     void
     (*mNOT_BEFORE)	(struct RepositoryManagerLexer_Ctx_struct * ctx);

     void
     (*mNOT_AFTER)	(struct RepositoryManagerLexer_Ctx_struct * ctx);

     void
     (*mTokens)	(struct RepositoryManagerLexer_Ctx_struct * ctx);
    const char * (*getGrammarFileName)();
    void            (*reset)  (struct RepositoryManagerLexer_Ctx_struct * ctx);
    void	    (*free)   (struct RepositoryManagerLexer_Ctx_struct * ctx);
};

// Function protoypes for the constructor functions that external translation units
// such as delegators and delegates may wish to call.
//
ANTLR3_API pRepositoryManagerLexer RepositoryManagerLexerNew         (
pANTLR3_INPUT_STREAM
 instream);
ANTLR3_API pRepositoryManagerLexer RepositoryManagerLexerNewSSD      (
pANTLR3_INPUT_STREAM
 instream, pANTLR3_RECOGNIZER_SHARED_STATE state);

/** Symbolic definitions of all the tokens that the 
lexer
 will work with.
 * \{
 *
 * Antlr will define EOF, but we can't use that as it it is too common in
 * in C header files and that would be confusing. There is no way to filter this out at the moment
 * so we just undef it here for now. That isn't the value we get back from C recognizers
 * anyway. We are looking for ANTLR3_TOKEN_EOF.
 */
#ifdef	EOF
#undef	EOF
#endif
#ifdef	Tokens
#undef	Tokens
#endif
#define EOF      -1
#define T__39      39
#define T__40      40
#define T__41      41
#define T__42      42
#define T__43      43
#define T__44      44
#define T__45      45
#define T__46      46
#define CERT      4
#define CERT_END      5
#define CERT_HEADER      6
#define CHALLENGE_END      7
#define CHALLENGE_HEADER      8
#define CLAUSE      9
#define CREDENTIAL      10
#define CREDENTIAL_END      11
#define CREDENTIAL_HEADER      12
#define END      13
#define END_CERT      14
#define END_SIG      15
#define NOT_AFTER      16
#define NOT_BEFORE      17
#define NUM      18
#define PK      19
#define PUBLIC_KEY_END      20
#define PUBLIC_KEY_HEADER      21
#define REPLY_END      22
#define REPLY_HEADER      23
#define REQUEST_END      24
#define REQUEST_HEADER      25
#define RESPONSE_END      26
#define RESPONSE_HEADER      27
#define SIG      28
#define SIGNATURE_END      29
#define SIGNATURE_HEADER      30
#define START_TIME      31
#define STOP_TIME      32
#define TEXT      33
#define VALIDITY      34
#define VALIDITY_END      35
#define VALIDITY_HEADER      36
#define VALUE      37
#define WS      38
#ifdef	EOF
#undef	EOF
#define	EOF	ANTLR3_TOKEN_EOF
#endif

#ifndef TOKENSOURCE
#define TOKENSOURCE(lxr) lxr->pLexer->rec->state->tokSource
#endif

/* End of token definitions for RepositoryManagerLexer
 * =============================================================================
 */
/** } */

#ifdef __cplusplus
}
#endif

#endif

/* END - Note:Keep extra line feed to satisfy UNIX systems */
