//////////////////////////////////////////////////////////////////////
//
// IgQtAnim.h: interface for the IgQtAnim class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IGQTANIM_H__2F16A551_9D67_4827_8516_14AFAF2143F7__INCLUDED_)
#define AFX_IGQTANIM_H__2F16A551_9D67_4827_8516_14AFAF2143F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Ig_Modules/IgVis/interface/config.h"
#include "qobject.h"
#include "Inventor/SbString.h"

class SoSFInt32;
class SoCounter;
class SoTimeCounter;
class SoNode;
class SoRotation;
class SoTimeCounter;
class SoComposeRotation;
class SoSensor;
class SoSFBool;

/*!  \class IgQtAnim IgQtAnim.h Ig_Modules/IgVis/interface/IgQtAnim.h
 *   Creates a node which automatically rotates the portion of the scene following it
 *   about all three axes.
 *
 */

class IG_VIS_API IgQtAnim : public QObject  

{
	Q_OBJECT
public:
	//! c-tor
	IgQtAnim(); 
	//! required static entry point for an OIV callback
	static void theIgQtAnimCallback(void *data, SoSensor *sensor);
	//! required static entry point for OIV callback for On/Off 
	static void theIgQtAnimOnOffCallback(void *data, SoSensor *sensor);
/*!  destructor. This only resets the internal counter. Unref the originally
 *  returned node to recover OIV resources.
 */
	virtual ~IgQtAnim(); 
	SoNode *getNode(); //!< returns a pointer to the central rotation
	SoSFBool& on(); //!< returns a reference to on/off field and resets to start value
	static void setAutoPix(bool ); //!< get the global autoPix flag
	static bool getAutoPix();	//!< set the global autoPix flag
signals:
	void newEventRequest(); //!< ready for another event
	void axisReset(int); //!< emitted on change of axis
protected:
	SoSFInt32 *state; //!< keeps track of which axis we're rotating about
	SoComposeRotation *theCRotation; //!< used to compose the overall rotation
	SoRotation* theRotation; //!< the overall rotation
	SoTimeCounter *theCounter; //!< connects the real-time clock to our rotation
	SoCounter *stateCounter; //!< counter which produces the state field mentioned above
//	SbString fieldName; //!< name for the global real-time field
	void doCallback(); //!< does the actual callback work
	void doOnOffCallback(); //!< invoked on state change for on/off
	static int depth; //!< track the current number of instantiations
	static bool autoPix; //!< flag to have all animators produce pictures
};

#endif // !defined(AFX_IGQTANIM_H__2F16A551_9D67_4827_8516_14AFAF2143F7__INCLUDED_)
