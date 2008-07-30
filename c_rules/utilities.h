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
#include "categories.h"
#include "value.h"

/**
 * Iterate over all initializers "i" in a list of variables
 * nodes should be something like SgVariableDeclaration->get_variables()
 */
#define FOREACH_INITNAME(nodes,i) \
	for(SgInitializedNamePtrList::const_iterator (i) = (nodes).begin(); (i) < (nodes).end(); (i)++)

/** Iterate over all nodes of type type below node, name the collection nodes
 * and name the iteroator i
 * 
 * \param[in] node Root node of the tree
 * \param[out] nodes What to call the collection
 * \param[out] i What to call the iterator
 * \param[in] type Only iterate over nodes of this type
 */
#define FOREACH_SUBNODE(node, nodes, i, type) \
	Rose_STL_Container<SgNode *> (nodes) = NodeQuery::querySubTree( const_cast<SgNode*>((const SgNode *) (node)), (type) ); \
	Rose_STL_Container<SgNode *>::iterator (i) = (nodes).begin(); \
	for (; (i) != (nodes).end(); ++(i) )


/**
 * \return value indicates whether there was a default case.
 */
bool getCaseValues( const SgBasicBlock *body, std::vector<int> &values );

/**
 * Questions having to do with function declarations.
 * \note first arg is arg 1, not arg 0.
 */
const SgType *getArgType( const SgFunctionDeclaration *fdec, int n );
int argCount( const SgFunctionDeclaration *fdec );
size_t CountLinesInFunction( const SgFunctionDeclaration *funcDecl );
bool isSingleArgFunctionDeclaration( const SgNode *node );

/**
 * Questions about function calls.
 */
const SgFunctionSymbol *isCallOfFunctionNamed( const SgNode *node, const std::string &name );

/**
 * The two function templates below simply execute the same operations as the
 * two non-template functions above, but will accept a sequence of names
 * instead of a single name.
 */
template <typename In>
const SgFunctionSymbol *isCallOfFunctionNamedAnyOf( const SgNode *node, In b, In e ) {
	while( b != e )
		if( SgFunctionSymbol *f = isCallOfFunctionNamed( *b++ ) )
			return f;
	return 0;
}

/**
 * Miscellaneous questions and manipulations.
 */
bool isCompilerGeneratedNode( const SgNode *node );
const SgExpression *removeImplicitPromotions( const SgExpression *e );

class IsFunctionDeclarationNamed : public std::unary_function<SgFunctionDeclaration *,bool> {
  public:
	IsFunctionDeclarationNamed( const std::string &id )
		: id_(id) {}
	bool operator ()( SgNode *node ) const {
		if( SgFunctionDeclaration *fd = isSgFunctionDeclaration( node ) ) {
			std::string fid = fd->get_name().getString();
			return fid == id_;
		}
		return false;
	}
  private:
	std::string id_;
};

/**
 * \todo do a version with a vector of nodeTypes, or just overload
 */
std::pair<const SgNode *,size_t> findParentNodeOfType( const SgNode *start, int nodeType );

void print_error(const SgNode* node, const char* rule, const char* desc,bool warning = false);

/**
 * \return True if node is inside an expression that tests its value to see if
 * it is NULL
 */
bool isTestForNullOp(const SgNode* node);

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
 * \return a variable's declaration, given a reference to that var
 */
const SgInitializedName* getRefDecl(const SgVarRefExp* ref);

/**
 * \return iterator of next node that refers to same variable as ref.
 * \return nodes.end() if unsuccessful
 */
Rose_STL_Container<SgNode *>::const_iterator nextVarRef(const Rose_STL_Container<SgNode *>& nodes, Rose_STL_Container<SgNode *>::const_iterator i, const SgInitializedName* ref);

/**
 * \return true if function( ref) appears in code somewhere after ref
 */
bool isVarUsedByFunction(const char* function, const SgVarRefExp* ref);

/**
 * \return true if the variable has global scope
 */
bool isGlobalVar(const SgInitializedName *var);
bool isStaticVar(const SgInitializedName *var);

bool isAnyCharType(const SgType *type);

const SgStatement * findInBlockByOffset(const SgNode *node, int delta);
bool getSizetVal(const SgExpression *node, size_t *value);
bool getIntegerVal(const SgExpression *node, intmax_t *n);
bool getFloatingVal(const SgExpression *node, long double *n);
bool isVal(const SgExpression *node, const intmax_t n);
bool isZeroVal(const SgExpression *node);
bool isMinVal(const SgExpression *node);
const SgExpression* removeCasts(const SgExpression * expr);
const SgType *stripModifiers(const SgType *type);
const SgType *stripTypedefs(const SgType *type);
const SgInitializedName *getVarAssignedTo(const SgFunctionRefExp *fnRef, const SgVarRefExp **varRef_p);
const SgExpression* getAllocFunctionExpr(const SgFunctionRefExp *node);
SgValueExp* computeValueTree(SgValueExp* node);
int getScanfFormatString(const SgFunctionRefExp *node);
int getPrintfFormatString(const SgFunctionRefExp *node);
bool varWrittenTo(const SgNode* var);

class NextVisitor : public AstPrePostProcessing {
public:
	// Visits nodes that will be executed after this one
	void traverse_next(const SgNode* node);
protected:
	virtual void preOrderVisit(SgNode *node);
	virtual void postOrderVisit(SgNode *node);
	virtual void visit_next(SgNode* node);

	// This is the node passed to traverse_next, we only want nodes
	// that follow it.
	const SgNode* sentinel_;

	// A stack of nodes before our sentinel that we might visit
	// because they are in the same for/while loop as our sentinel
	Rose_STL_Container< Rose_STL_Container< SgNode*> > stack_;

	// true if we've passed our sentinel yet
	bool after_;

	// If non-NULL, skip nodes until we encounter this one
	const SgNode* skip_;
};

/**
 * Checks to see if node is an assignment with var as the lhs and not in
 * the rhs
 */
bool isAssignToVar( const SgNode *node, const SgVarRefExp *var);

class NextValueReferred : public NextVisitor {
public:
	// Returns next instance where ref's value is used, or NULL if none
	const SgVarRefExp* next_value_referred(const SgVarRefExp* ref);
protected:
	const SgVarRefExp* next_ref_;
	const SgInitializedName* var_;
	virtual void visit_next(SgNode* node);
};

bool isCheckForZero(const SgStatement *stat, const SgVarRefExp *varRef);
size_t sizeOfType(const SgType *type);
bool valueVerified(const SgExpression *expr);
bool isConstType(const SgType *t);
bool isVolatileType(const SgType *t);

#endif
