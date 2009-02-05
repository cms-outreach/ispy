// TestScene.cpp: implementation of the TestScene class.
//
//////////////////////////////////////////////////////////////////////
#include "Ig_Examples/IgVisEx/interface/TestScene.h"
#include "Ig_Examples/IgVisEx/interface/SmartBox.h"
#include "Ig_Modules/IgVis/interface/IgQtLiveTwig.h"
#include "Ig_Modules/IgVis/interface/IgQtVectorTwig.h"
#include "Ig_Modules/IgVis/interface/IgQtTwig2D.h"
#include "Ig_Modules/IgVis/interface/IgQtThresholdSliderImpl.h"
#include "Ig_Modules/IgVis/interface/IgQtSliderImplF.h"

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoMarkerSet.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/SoPath.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/nodes/SoScale.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoFaceSet.h>
#include <Inventor/nodes/SoVertexShape.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoText3.h>

#include <vector>
#include <qtabwidget.h>
#include <qlayout.h>
#include <qgroupbox.h>

int TestTwig::selectState( const TestObject* data )
{
	return data->getData() >= threshold ? SO_SWITCH_ALL : SO_SWITCH_NONE;
}

TestTwig::TestTwig( IgQtTwig* parent, SbString name, bool visibility, bool cascade ) :
    IgQtLiveTwig< TestObject >( parent, name, visibility, cascade ){}

//! alternate c-tor
TestTwig::TestTwig( SbString name, bool visibility, bool cascade ):
    IgQtLiveTwig< TestObject >( name, visibility, cascade ){}

//! d-tor
TestTwig::~TestTwig(){}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


TestScene::TestScene(std::ostream & /*stream*/, bool build) : IgQtTwig("TestTwig"), p_list(0), p_vector(0), p_local(0)
{
	if (build) buildScene();
}
void TestScene::buildScene()
{
//  set the output stream to the status window
//	setOstream(stream);

    // add an object which is both graphic and smart
    getIvNode()->addChild( new SmartBox( 5 ));
	
	// Make some objects which are smart but not graphic. These could represent
	// general physics objects such as hits (SI, chamber or calorimetric), tracks, jets, etc.
	// which have internal data and an extraction operator.
	//
	// Maintain pointers to them in, e.g., a list
    p_list = new std::list<TestObject>;
	const int nBoxes = 50;
	for (int i=0; i < nBoxes; i++) {
		p_list->push_back(TestObject(i));
	}
	const float armLength = 3.0; //length of radius to boxes
	float angle;
	// Create the twig for these objects
	p_local = new TestTwig("SmartObjects");
	SoSFVec3f tinyScale;
	tinyScale.setValue(0.04F,0.04F,0.04F);
	SoScale *tiny = new SoScale;
	tiny->scaleFactor = tinyScale;

	// Now simulate the process of looping over tracks, etc.
	int cnt = 0;
	for (  std::list<TestObject>::const_iterator it = p_list->begin(); it != p_list->end(); it++, cnt++)
	{
		angle = (*it).getData()*2*M_PI/nBoxes;
		SoSeparator *aSeparator = new SoSeparator;
		SoTranslation *aTranslation = new SoTranslation;
		aTranslation->translation.setValue(SbVec3f(armLength*cos(angle),0,
			armLength*sin(angle)));
		SoCube *box = new SoCube;
		char cbuf[7];
		sprintf(cbuf,"Box_%02i",cnt);
		box->setName(SbName(cbuf));
		aSeparator->addChild(aTranslation);
		aSeparator->addChild(tiny);
		aSeparator->addChild(box);
		p_local->insert(box, &(*it), aSeparator); // save the relation between the graphics object and the data pointer in the map	
	}
	this->addChild(p_local);


	// Now put in an object which is indexed
	// 
	// Use a vector just as a change...
    p_vector = new std::vector< TestObject >;
    const int nStars = 12;
    for( int i = 0; i < nStars; i++ )
    {
	p_vector->push_back( TestObject( i ));
    }
    // Create the graphics node for the twig
    SoMarkerSet* markerSet = new SoMarkerSet;
    SoVertexProperty* vertexProperty = new SoVertexProperty;
    SoMFVec3f vertices;
    int nVertices = 0;
    const float radius = 2 * armLength;
    for( std::vector< TestObject >::const_iterator it = p_vector->begin(); it != p_vector->end(); it++ )
    {
	angle = ( *it ).getData() * 2 * M_PI / nStars;
	SbVec3f aPoint( 0.F, radius * cos( angle ), radius * sin( angle ));
	vertices.set1Value( nVertices++, aPoint );
    }
    vertexProperty->vertex = vertices;
    markerSet->vertexProperty = vertexProperty;
    markerSet->markerIndex = SoMarkerSet::STAR_7_7;

	// Create the twig
	IgQtVectorTwig<TestObject> *p_vTwig = new IgQtVectorTwig<TestObject>(p_vector,"Point Objects",false);
	p_vTwig->insert(markerSet);
	// add it to the test scene
	addChild(p_vTwig);
}

TestScene::~TestScene()
{
	if (p_list) delete p_list;
	if (p_vector) delete p_vector;
}

void
TestScene::printInfo( std::ostream & os, const SoNode* node ) const
{
    IgQtTwig::printInfo( os, node );

#ifdef IGUANA_CATCH_DYN_CAST

    try
    {
	const BaseQueryNode* smartNode = dynamic_cast< const BaseQueryNode* >( node );
	if( smartNode != NULL)
	{
	    os << *smartNode;
	}
    }
    catch( bad_cast bc )
    {
	std::cout << "Bad Cast Exception raised"  << std::endl;
    }
    
#else
    
    const BaseQueryNode* smartNode = dynamic_cast< const BaseQueryNode* >( node );
    if (smartNode) os << *smartNode;

#endif

}

void TestScene::thresholdSlot( int theThreshold )
{
    p_local->setThreshold( theThreshold );
    p_local->stateLoop();
}
void TestScene::doubleSlot(double theDouble)
{
    double theValue = theDouble;
}

bool TestScene::setControlWidget(QWidget* qtw)
{
	QFrame *f = new QFrame(qtw);
    IgQtThresholdSliderImpl *theThresholdSlider = new IgQtThresholdSliderImpl(f,"Arb Threshold");
	QBoxLayout *l = new QVBoxLayout(f);
	l->addWidget(theThresholdSlider);
    connect(theThresholdSlider,SIGNAL(newValueSignal(int)),SLOT(thresholdSlot(int)));
	IgQtSliderImplF *sf = new IgQtSliderImplF(f,"Test Values");
	sf->setTitle("Float Values");
	sf->setMinValue(0);
	l->addWidget(sf);
	connect(sf,SIGNAL(newValueSignal(double)),SLOT(doubleSlot(double)));

	QTabWidget *q = dynamic_cast<QTabWidget*>(qtw);
	if (q) q->addTab(f,"Thresh");
	l->addStretch(10); // add some stretch to keep them their natural size

	return true; //yes, we can respond
}









