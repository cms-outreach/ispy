#if !defined(AFX_IGQTTEXTSTREAM_H__A68743E5_584B_43CC_9D1A_6DA55A5B14F5__INCLUDED_)
#define AFX_IGQTTEXTSTREAM_H__A68743E5_584B_43CC_9D1A_6DA55A5B14F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/*! \class IgQtTextStream  IgQtTextStream.h Ig_Modules/IgVis/interface/IgQtTextStream.h
 *
 *  \brief An adaptor for QTextStream so that it can pose as a normal streambuf
 *
 *  \author G. Alverson, Northeastern University
 *
 *  \date 3-Sep-2000
 */

#include "Ig_Modules/IgVis/interface/config.h"
#include "qtextstream.h"
#include "qbuffer.h"
#include <iostream> 

class IG_VIS_API IgQtTextStream : public QTextStream  
{
public:
	IgQtTextStream();
	virtual ~IgQtTextStream();
	const QBuffer& buffer( void ) const;
private:
	QBuffer theBuffer;
};
inline const QBuffer& IgQtTextStream::buffer( void ) const { return theBuffer; }
#endif // !defined(AFX_IGQTTEXTSTREAM_H__A68743E5_584B_43CC_9D1A_6DA55A5B14F5__INCLUDED_)
