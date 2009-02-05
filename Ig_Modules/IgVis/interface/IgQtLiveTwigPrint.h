#ifndef IGQTLIVETWIGPRINT_HPP
#define IGQTLIVETWIGPRINT_HPP


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*!
 *  \class  IgQtLiveTwigPrint IgQtLiveTwigPrint.h Ig_Modules/IgVis/interface/IgQtLiveTwigPrint.h
 *
 *  \brief  A LiveTwig with additional print out upon selection.
 *  \author G. Alverson, Northeastern University Experimental Particle Physics
 */

#include "Ig_Modules/IgVis/interface/IgQtTwig.h"
#include "Inventor/SoPath.h"
#include "Inventor/actions/SoSearchAction.h"
#include <iostream>
#include <map>
#ifdef WIN32
#include "qt_windows.h"
#endif


class SoSwitch;

template<class T> class IG_VIS_API IgQtLiveTwigPrint : public IgQtTwig  
{
public:
#ifndef WIN32
	typedef uintptr_t MAPPTR;
#else
	typedef int MAPPTR;
#endif
	typedef std::map<MAPPTR, const T*> T_Map;
	typedef typename std::map<MAPPTR, const T*>::const_iterator T_Map_iter;
	//! c-tors same as for IgQtTwig
	explicit IgQtLiveTwigPrint(IgQtTwig *parent, SbString name, bool visibility = true, bool cascade = true);
	//! alternate c-tor
	explicit IgQtLiveTwigPrint(SbString name, bool visibility = true, bool cascade = true);
	//! d-tor
	virtual ~IgQtLiveTwigPrint();
	//! insert a pointer to a \b T in our local map, cross-indexed against node. May supply group which contains node.
	void insert(SoNode *node, const T *itptr, SoNode *group=NULL);
	//! to be removed?
	T_Map *getMap() {return &m_map;}
	//! function to return appropriate state based on internal data. Needs to be supplied in a derived class to work.
	virtual int selectState(const T* data);
	//! loops over all children
	void stateLoop();
protected:
	//! redirects << for the twig back to \b T
	virtual void printInfo(std::ostream& ostr, const SoNode *node) const;
	//! for debugging
	void printPath(SoPath *path);
	//! access to the underlying data; don't need both this and getMap
	const T* dataElement(const SoNode* node) const; 
private:
	//! internal map of nodes vs. \b T's
	T_Map m_map;
	int counter; // for testing purposes only

};
#include <utility>
#include "Inventor/nodes/SoSwitch.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
template <class T> int IgQtLiveTwigPrint<T>::selectState(const T* data)
{
	return 0;
}
template <class T> IgQtLiveTwigPrint<T>::IgQtLiveTwigPrint(IgQtTwig *parent, 
												 SbString name, bool visibility, 
												 bool cascade) : 
	IgQtTwig(parent, name, visibility, cascade), counter(0)
{		
}
template <class T> IgQtLiveTwigPrint<T>::IgQtLiveTwigPrint(SbString name, 
												 bool visibility, 
												 bool cascade) :
		IgQtTwig(name, visibility, cascade), counter(0)
{
}
template <class T> IgQtLiveTwigPrint<T>::~IgQtLiveTwigPrint()
{

}
template <class T> void IgQtLiveTwigPrint<T>::printInfo(std::ostream& ostr, const SoNode* node) const {
	const MAPPTR converted_node = reinterpret_cast<MAPPTR>(node);
	T_Map_iter anIterator = m_map.find(converted_node);
	if (m_map.find(converted_node) != m_map.end() ){
		anIterator->second->print(ostr);
	}
	else {
		ostr << "A Twig has been selected but is unknown!" << std::endl;
	}
}
template <class T> const T* IgQtLiveTwigPrint<T>::dataElement(const SoNode* node) const 
{
	const MAPPTR converted_node = reinterpret_cast<MAPPTR>(node);
	T_Map_iter anIterator = m_map.find(converted_node);
	if (m_map.find(converted_node) != m_map.end() )
	{
		return (*anIterator).second;
	}
	else
	{
		return NULL;
	}
}
template <class T> void IgQtLiveTwigPrint<T>::insert(SoNode *node, const T *itptr, SoNode *group) 
{
	// convert the data node pointer to an integer of some sort (MAPPTR)
	const MAPPTR converted_node = reinterpret_cast<MAPPTR>(node);
	m_map.insert(std::make_pair(converted_node, itptr));
	SoSwitch *aSwitch = new SoSwitch;
	aSwitch->whichChild = SO_SWITCH_ALL;
	//test
		char cbuf[14];
		sprintf(cbuf,"Box_Switch_%02i",counter++);
		aSwitch->setName(SbName(cbuf));
	// if additional, higher-level node is not supplied, just use the data-node
	if (group != NULL) {
		aSwitch->addChild(group); }
	else {
		aSwitch->addChild(node);
	}
	addIvNode(aSwitch);
}
template <class T> void IgQtLiveTwigPrint<T>::stateLoop()
{
	SoSearchAction findIt;
	SoSearchAction findSwitch;
	findSwitch.setType(SoSwitch::getClassTypeId());
	findIt.setSearchingAll(true); //seach through off SoSwitch nodes
	int icnt=0;
	for (T_Map_iter it = m_map.begin(); it != m_map.end(); it++, icnt++)
	{
		SoNode *bottomNode = reinterpret_cast<SoNode*>(it->first);
		findIt.setNode(bottomNode);
		findIt.apply(getIvNode());
		SoPath *aPath = findIt.getPath(); // get a path from this Twig separator to the desired data
		if ( aPath != NULL) {
			findSwitch.apply(aPath);  // find the Switch somewhere along the path
			SoPath *sPath = findSwitch.getPath();

			if ( sPath != NULL) {
//TODO: normal technique for finding the path didn't seem to work.Otherwise we'd use it. 
				SoNode *theSuspect = aPath->getNode(1); // just hardwire in the known location
				if (theSuspect->isOfType(SoSwitch::getClassTypeId())) 
				{				
					SoSwitch *theSwitch = static_cast<SoSwitch*>(theSuspect);
					int state = selectState(it->second); // check to see visibility status (based on the data)
					int old_state = theSwitch->whichChild.getValue();
					if (theSwitch->whichChild.getValue() != state) 
					{
						theSwitch->whichChild.setValue(state);
					}
				}
			}
		}
	}
}
template <class T> void IgQtLiveTwigPrint<T>::printPath(SoPath *path)
{
#ifdef WIN32 //debug printout
	char xbuf[128];
	sprintf(xbuf,"Tracing back path...\n");
	OutputDebugString(xbuf);
	int n = path->getLength();
	for (int i=0; i < n; i++) {
		SoNode *xNode = path->getNodeFromTail(i);
		SbName nodeName = xNode->getName();					
		sprintf(xbuf,"Node %i from tail is %s\n",i,nodeName.getString());
		OutputDebugString(xbuf);
	}
#endif
}

#endif 

