#ifndef IG_CMSCAN_IG_QT_TWIG_CALLBACK_STRUCT_H
# define IG_CMSCAN_IG_QT_TWIG_CALLBACK_STRUCT_H

#include "Ig_Modules/IgVis/interface/config.h"

class IgQtTwig;
class SoSceneManager;

/*!  \class IgQtTwigCallbackStruct IgQtTwigCallbackStruct.h Ig_Modules/IgVis/interface/IgQtTwigCallbackStruct.h
 *
 *  \brief Provide a callback structure for use with X/OIV callbacks.
 *
 *  \author G. Alverson, T. Paul, L. Taylor
 *  \date Prehistoric
 */

class IG_VIS_API IgQtTwigCallbackStruct
{
public:
	//! supplying a twig allows us to call a non-static method
    IgQtTwigCallbackStruct(IgQtTwig *aTwig, SoSceneManager *aManager = NULL);

    IgQtTwig *		theTwig;
    SoSceneManager *	theManager;
};

inline
IgQtTwigCallbackStruct::IgQtTwigCallbackStruct (IgQtTwig *aTwig,
						SoSceneManager *aManager)
    : theTwig (aTwig),
      theManager (aManager)
{}

#endif // IG_CMSCAN_IG_QT_TWIG_CALLBACK_STRUCT_H
