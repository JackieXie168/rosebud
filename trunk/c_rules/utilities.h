/**
 * \file utilities.h
 *
 * Copyright (c) 2007 Carnegie Mellon University.
 * All rights reserved.

 * Permission to use this software and its documentation for any purpose is
 * hereby granted, provided that the above copyright notice appear and that
 * both that copyright notice and this permission notice appear in supporting
 * documentation, and that the name of CMU not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.
 *
 * CMU DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
 * IMPLIED WSTRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL CMU BE
 * LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, RISING OUT OF OR
 * IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */


#ifndef UTILITIES_H
#define UTILITIES_H

#include <list>
#include <string>
#include "rose.h"
#include "utilities-inline.h"
#include "categories.h"
#include "value.h"


/**
 * Do not use this function directly, use the findParentOfType macro
 */
const SgNode *findParentNodeOfType(const SgNode *node, VariantT t);

bool isCallOfFunctionNamed(const SgFunctionRefExp *node, const std::string &name);
const SgExpression *removeImplicitPromotions( const SgExpression *e );

/**
 * \return reference to ith argument of function reference. Dives through
 * typecasts.
 * \return NULL if no such parm
 */
const SgExpression* getFnArg(const SgFunctionRefExp* node, unsigned int i);
const SgExpression* getFnArg(const SgFunctionCallExp* fnCall, unsigned int i);

/**
 * Fills list with all nodes in enclosing function
 */
const Rose_STL_Container<SgNode*> getNodesInFn( const SgNode* node);

/**
 * \return iterator of next node that refers to same variable as ref.
 * \return nodes.end() if unsuccessful
 */
Rose_STL_Container<SgNode *>::const_iterator nextVarRef(const Rose_STL_Container<SgNode *>& nodes, Rose_STL_Container<SgNode *>::const_iterator i, const SgInitializedName* ref);

/**
 * \return true if function( ref) appears in code somewhere after ref
 */
bool isVarUsedByFunction(const char* function, const SgVarRefExp* ref);

const SgStatement * findInBlockByOffset(const SgNode *node, int delta);
const SgExpression* removeCasts(const SgExpression * expr);

const SgExpression* getAllocFunctionExpr(const SgFunctionRefExp *node);
SgValueExp* computeValueTree(SgValueExp* node);
int getScanfFormatString(const SgFunctionRefExp *node);
int getPrintfFormatString(const SgFunctionRefExp *node);

/**
 * Returns the number of bytes that an object of this type should take up
 */
size_t sizeOfType(const SgType *type);

/**
 * Use this to report errors and warnings
 */
void print_error(const SgNode* node, const char* rule, const char* desc,bool warning = false);

/****************************************************************************
 * Variable assignment functions                                            *
 ****************************************************************************/
bool varWrittenTo(const SgNode* var);
const SgInitializedName *getVarAssignedTo(const SgFunctionRefExp *fnRef, const SgVarRefExp **varRef_p);
bool valueVerified(const SgExpression *expr);
bool isCheckForZero(const SgStatement *stat, const SgVarRefExp *varRef);
/**
 * \return True if node is inside an expression that tests its value to see if
 * it is NULL
 */
bool isTestForNullOp(const SgNode* node);

/**
 * Checks to see if node is an assignment with var as the lhs and not in
 * the rhs
 */
bool isAssignToVar( const SgNode *node, const SgInitializedName *var);


/**
 * Given a node in a block, returns the node at the top of the block.
 *
 * \note This looks for a lot of blocks, but maybe not all of them?
 */
const SgNode *popBlock(const SgNode *node);


/**
 * Class to visit nodes following node in control flow
 */
class CFGVisitor {
private:
  /** Nodes that have already been visited */
  std::set<VirtualCFG::CFGNode> visited;
public:
  CFGVisitor() {}
  virtual ~CFGVisitor() {}
  /** Do something with this node, then visit its successors, in a bfs order */
  void visit(const VirtualCFG::CFGNode& node);
  /** Visit node's successors */
  void visitOthers(const VirtualCFG::CFGNode& node);
  /**
   * Do something with this node
   * /return true if visiting should continue
   */
  virtual bool doSomething(const SgNode* node) = 0; // returns true if visit should continue
};


/**
 * /param ref  Reference to a variable
 * /return list of succeeding variable references
 */
Rose_STL_Container<const SgVarRefExp*> next_var_references(const SgVarRefExp* ref);


/// C++ utiltiies

/* Returns a declaration of the given class */
const SgClassDeclaration *getClassDeclaration(const SgType* t);

/* Returns true if class is std */
bool isClassDeclaredInStdNamespace(const SgClassDeclaration *decl);

/* Returns true if class inherits from std::exception */
bool inheritsFromStdException(const SgClassDeclaration *cdecl);


#endif
