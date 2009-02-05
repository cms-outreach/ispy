// IgQtVectorTwig.h: interface for the IgQtVectorTwig class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IGQTVECTORTWIG_H__F7705EC7_D0F7_406B_B7FD_CEE1B093E741__INCLUDED_)
#define AFX_IGQTVECTORTWIG_H__F7705EC7_D0F7_406B_B7FD_CEE1B093E741__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IgQtTwig.h"
#include <vector>

/*! /class IgQtVectorTwig IgQtVectorTwig.h
 *  Template class for placing a vector of interactive items in a Twig
 *
 */


template<class T> class IG_VIS_API IgQtVectorTwig : public IgQtTwig  
{
public:
	//! c-tors same as for IgQtTwig
	IgQtVectorTwig(std::vector<T> *aVector, IgQtTwig *parent, SbString name, bool visibility = true, bool cascade = true);
	//! alternate c-tor
	IgQtVectorTwig(std::vector<T> *aVector, SbString name, bool visibility = true, bool cascade = true);
	//! alternate c-tor
//	explicit IgQtVectorTwig(bString name, bool visibility = true, bool cascade = true);
	//! d-tor
	virtual ~IgQtVectorTwig();
	//! insert a node representing the vector object. May supply group which contains node.
	void insert(SoNode *node, SoNode *group=NULL);
protected:
	//! redirects << for the twig back to \b T
	virtual void printInfo(std::ostream& ostr, const SoNode *node) const;
	//! if the vector isn't passed in the constructor
	void setVector(std::vector<T> *aVector);
private:
	//! internal pointer to vector 
	std::vector<T> *p_vector;

};
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

template <class T> IgQtVectorTwig<T>::IgQtVectorTwig(std::vector<T> *aVector, IgQtTwig *parent, 
												 SbString name, bool visibility, 
												 bool cascade) : 
	IgQtTwig(parent, name, visibility, cascade), p_vector(aVector)
{

}
template <class T> IgQtVectorTwig<T>::IgQtVectorTwig(std::vector<T> *aVector, SbString name, 
												 bool visibility, 
												 bool cascade) :
		IgQtTwig(name, visibility, cascade), p_vector(aVector)
{
}
template <class T> IgQtVectorTwig<T>::~IgQtVectorTwig()
{

}
template <class T> void IgQtVectorTwig<T>::printInfo(std::ostream& ostr, const SoNode* node) const 
{
	
	int picked_point_ndx = getPickIndex(node);
	if (picked_point_ndx != -1) {
		ostr << (*p_vector)[picked_point_ndx];
	}
}
template <class T> void IgQtVectorTwig<T>::insert(SoNode *node, SoNode *group) 
{
	if (group != NULL) {
		addIvNode(group); }
	else {
		addIvNode(node);
	}
}
template <class T> 	void IgQtVectorTwig<T>::setVector(std::vector<T> *aVector)
{
	p_vector = aVector;
}

#endif // !defined(AFX_IGQTVECTORTWIG_H__F7705EC7_D0F7_406B_B7FD_CEE1B093E741__INCLUDED_)
