#ifndef IG_TEST_IGGI_CONTROLLER_H
#define IG_TEST_IGGI_CONTROLLER_H

//<<<<<< INCLUDES                                                       >>>>>>

#include <QDialog>
#include "ISpyItem.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class ISpyController : public QDialog
{
public:
	ISpyController (QDialog* parent = 0) : QDialog (parent) {};
	virtual ~ISpyController () {};
	
	virtual void setControlledItem (ISpyItem* item) = 0;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>
#endif // IG_TEST_IGGI_CONTROLLER_H
