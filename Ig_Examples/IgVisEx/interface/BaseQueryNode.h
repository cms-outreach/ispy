#ifndef INCLUDE_BaseQueryNode_
#define INCLUDE_BaseQueryNode_
/* ----------------------------Hepvis----------------------------------------*/
/*                                                                           */
/* Class:            BaseQueryNode                                           */
/*                                                                           */
/* Description:      (not quite virtual) base class for querying             */
/*                   nodes via selection                                     */
/*                                                                           */
/* Authors:          G. Alverson, L. Taylor, T.Paul       03 Mar 1999        */
/*                                                                           */
/*---------------------------------------------------------------------------*/
 
#include "Ig_Examples/IgVisEx/interface/config.h"
#include <iostream>

/*! \class BaseQueryNode BaseQueryNode.h HEPVis/BaseQueryNode.h
 *  \brief BaseQueryNode is a (not quite) virtual base class for
 *  nodes which need to be queried interactively.
 *
 */ 
class BaseQueryNode {
public:
/*! Supply an extraction operator to an output stream. When
 *  a class multiply inherits from BaseQueryNode, it always has an
 *  output method.
 */ 
	friend std::ostream& operator << (std::ostream& o, const BaseQueryNode& b);
protected:
	//! called by the extractor
	virtual void print(std::ostream& o) const;
};

//===================== Inline defs ==============================
inline std::ostream& operator<< (std::ostream& o, const BaseQueryNode& b) {
  b.print(o);
  return o;
}

inline void BaseQueryNode::print(std::ostream& o) const {
  o << "Info request unimplemented for this class" << std::endl; }

#endif  //INCLUDE_BaseQueryNode_
 
