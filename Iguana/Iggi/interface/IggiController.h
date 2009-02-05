#ifndef IG_TEST_IGGI_CONTROLLER_H
#define IG_TEST_IGGI_CONTROLLER_H

//<<<<<< INCLUDES                                                       >>>>>>

#include <QDialog>
#include "IggiItem.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IggiController : public QDialog
{
public:
	IggiController (QDialog* parent = 0) : QDialog (parent) {};
	virtual ~IggiController () {};
	
	virtual void setControlledItem (IggiItem* item) = 0;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>
#endif // IG_TEST_IGGI_CONTROLLER_H
