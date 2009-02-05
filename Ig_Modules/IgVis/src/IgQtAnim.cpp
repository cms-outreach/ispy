//////////////////////////////////////////////////////////////////////
//
// IgQtAnim.cpp: implementation of the IgQtAnim class.
//
//////////////////////////////////////////////////////////////////////

#include "Ig_Modules/IgVis/interface/IgQtAnim.h"


#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoSFRotation.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/engines/SoOneShot.h>
#include <Inventor/engines/SoCalculator.h>
#include <Inventor/engines/SoTimeCounter.h>
#include <Inventor/engines/SoCounter.h>
#include <Inventor/engines/SoCompose.h> //SoComposeRotation et al.
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/fields/SoSFTrigger.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/SoDB.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/sensors/SoSensor.h>
#include <Inventor/SbString.h>
#include <Inventor/fields/SoSFString.h>


//#ifdef _DEBUG
//#undef THIS_FILE
//static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
//#endif


// static storage
int IgQtAnim::depth = 0;
bool IgQtAnim::autoPix = false;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


IgQtAnim::~IgQtAnim()
{
	//this will re-use the (last) global field
	IgQtAnim::depth--;
	theCounter->unref();

}
void IgQtAnim::theIgQtAnimCallback(void *data, SoSensor * /*sensor*/)
{
	IgQtAnim *cc = static_cast<IgQtAnim*>(data);
	cc->doCallback();
}
void 	IgQtAnim::theIgQtAnimOnOffCallback(void *data, SoSensor * /*sensor*/)
{
	IgQtAnim *cc = static_cast<IgQtAnim*>(data);
	cc->doOnOffCallback();
}
void IgQtAnim::doOnOffCallback()
{
	if (!(theCounter->on).getValue())
	{
	    theCounter->reset = 0;
	    stateCounter->reset = 0;
	    // this should be redundant since we've already reset theCounter
	    theCRotation->axis.setValue(1.F,0.F,0.F);
	    theCRotation->angle.setValue(0.F);
	}
	else
	{
	    theCounter->reset = 0;
	}

}
void IgQtAnim::doCallback()
{
	//select the rotation axis based on the value of the state variable


	if (state->getValue()==1) // rotate about y
	{
		theCRotation->axis.setValue(0.F,1.F,0.F);
	}
	else if (state->getValue()==2) // rotate about z
	{
		theCRotation->axis.setValue(0.F,0.F,1.F);
	}
	else // rotate about x: state 0
	{
		theCRotation->axis.setValue(1.F,0.F,0.F);
	}
	SbBool counterOn = false;
	if (autoPix)
	{
		counterOn = theCounter->on.getValue();
		theCounter->on = false;
//		theCounter->reset = 0;
//		theCounter->syncIn.touch();
		
		// emit
		axisReset(state->getValue());
	}
	if (state->getValue()==1) // was 2 when we included the z rotation
	{
		//		emit 
		newEventRequest();
	}
	if (autoPix)
	{
		theCounter->on = counterOn; // callback gets called at init, so can't just turn the counter on
	}
}
IgQtAnim::IgQtAnim(){
	//  initialise the local variables
	theCRotation = NULL;
	theRotation = NULL;
	theCounter = NULL;
	stateCounter = NULL;
	state = NULL;
	IgQtAnim::depth++;

/*
 

    Counter -> Calculator -> ComposeRotation -> Rotation
       |                                           ^
	   |                                           :
	   | (synch)                                   :   
	   |                                           :
	    -> stateCounter -> Sensor -> Callback.......


    The rotation and the counter on/off field are exposed.



 */
	
		
	
	//      May want to expose some of these fields for user adjustment


	//		set up a timing loop
	theCounter = new SoTimeCounter;
	theCounter->max = 2000; // can only store short ints; use mrad/M_PI
	theCounter->step = 10;  
	theCounter->frequency = 0.05F;//0.15F;
	theCounter->ref(); // ref so the assemblage will not be garbage collected.
	

	// create a calculator to convert from the timer int output to a real angle
	SoCalculator *theCalculator = new SoCalculator;
	theCalculator->a.connectFrom(&theCounter->output);
	theCalculator->expression.set1Value(0,"oa=M_PI*a/1000"); //mrad to rad

	//		connect the angle of rotation to the rotation
	theCRotation = new SoComposeRotation;
	theCRotation->axis.setValue(1.F,0.F,0.F);
	theCRotation->angle.connectFrom(&theCalculator->oa);
	theRotation = new SoRotation;
	theRotation->setName("Animator");
	theRotation->rotation.connectFrom(&theCRotation->rotation);
	
	
	// use the state counter to cycle over the axes: states are 0,1 (omit 2 for now) (min defaults to zero)
	stateCounter = new SoCounter;
	stateCounter->max = 1;
	stateCounter->step = 1;
	stateCounter->trigger.connectFrom(&theCounter->syncOut);
	stateCounter->reset = 0;

	//TODO:
	// we guarantee that we have a unique global field, but as we can't destroy the field,
	// we should limit the number of instantiations
	char tmp_string[5];
	sprintf(tmp_string,"%4.4d",IgQtAnim::depth);
	SbString fieldName("IgQtAnimCtl_state_");
	fieldName += tmp_string;
//	fieldName = SbString("IgQtAnimCtl_state_");
//	fieldName += tmp_string;
	state = static_cast<SoSFInt32*>(SoDB::createGlobalField(SbName(fieldName),SoSFInt32::getClassTypeId()));

	// connect the timer cycle to the state, then
	// connect the state output to the callback sensor. this way when the state changes we get called
	// so that we can change the axis appropriately
	state->connectFrom(&stateCounter->output);
	SoFieldSensor *theSensor = new SoFieldSensor;
	
	
	theSensor->setFunction(IgQtAnim::theIgQtAnimCallback);
	theSensor->setData(static_cast<void*>(this));
	theSensor->setPriority(102); //default +2 for slightly lower priority
	theSensor->attach(state);
	
	SoFieldSensor *theOnOffSensor = new SoFieldSensor;
	theOnOffSensor->setFunction(IgQtAnim::theIgQtAnimOnOffCallback);
	theOnOffSensor->setData(static_cast<void*>(this));
	theOnOffSensor->attach(&theCounter->on);
	
}
SoNode* IgQtAnim::getNode()
{
	return theRotation;
}
SoSFBool& IgQtAnim::on()
{
	return (theCounter->on);
}
void IgQtAnim::setAutoPix(bool on)
{
	IgQtAnim::autoPix = on;
}
bool IgQtAnim::getAutoPix()
{
	return IgQtAnim::autoPix;
}

