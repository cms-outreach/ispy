#if !defined(AFX_IGQTINFOSTREAM_H__2E584DD9_F4A9_4116_9EF9_90E1FA03A526__INCLUDED_)
#define AFX_IGQTINFOSTREAM_H__2E584DD9_F4A9_4116_9EF9_90E1FA03A526__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*! \class IgQtInfoStream IgQtInfoStream.h  Ig_Modules/IgVis/interface/IgQtInfoStream.h
 *
 *  \brief Provides a connection from an iostream to a QTextBrowser
 *
 *
 *  \author G. Alverson, Northeastern University
 *  \date 4-Sep-2000
 */

#include "Ig_Modules/IgVis/interface/config.h"
#include "qtextbrowser.h"
#include "qglobal.h"
#include <iostream>

class IG_VIS_API IgQtInfoStream : public std::ostream  
{
	/*! \class IgQtInfoStreamBuf IgQtInfoStream.h IgQtInfoStream.h
	 *  \brief Private streambuf for IgQtInfoStream
	 */
	class IgQtInfoStreamBuf : public std::streambuf {
	public:
		//! c-tor needs parent widget for its output widget
		explicit IgQtInfoStreamBuf(QTextBrowser *pWidget=0);
		//! d-tor
		virtual ~IgQtInfoStreamBuf() {};
		//! need to be able to set the output widget
		void setWidget(QTextBrowser *pWidget);
		//! clear the output widget and internal buffers
		void clear();
	private:
		//! re-implements basic streambuf method
		virtual int_type overflow(int_type ch = std::streambuf::traits_type::eof());
		//! re-implements basic streambuf method
		virtual std::streamsize xsputn(const char *c, std::streamsize nc);
		//! re-implements basic streambuf method
		virtual int underflow() {return std::streambuf::traits_type::eof(); }
		//! re-implements basic streambuf method
		virtual int uflow() {return std::streambuf::traits_type::eof(); }
		//! internal buffer
		QString m_buffer;
		//! pointer to the output widget
		QTextBrowser * statusWidget;
	};


public:
	//! c-tor really needs a parent widget
	explicit IgQtInfoStream(QWidget *parent=0);
	//! d-tor
	virtual ~IgQtInfoStream();
	//! re-implement basic ostream method
	const std::streambuf *rdbuf() const {return static_cast<const std::streambuf *>(&m_streamBuf);}
	//! sets the caption for the info widget
	void setCaption(const QString& caption);
	//! and access the info widget
	QWidget *getWidget(void);
	//! shows the info widget
	void show();
	//! clear the contents of the widget
	void clear();
	//! for use with a default InfoStream
	static IgQtInfoStream* getDefaultInfoStream(QWidget *parent=0);
	//! to set the caption on the default
	static void setDefaultCaption(const QString& caption);
private:
	//! the output widget
	QTextBrowser *theBrowser;
	//! internally maintained stream buffer
	IgQtInfoStreamBuf m_streamBuf;
	//! unique name
	static const QString className;
	//! serial no.
	static int serialNo;
	//! the default stream
	static IgQtInfoStream *defaultStream;
	//! are we the default? 
	bool the_default;

};

#endif // !defined(AFX_IGQTINFOSTREAM_H__2E584DD9_F4A9_4116_9EF9_90E1FA03A526__INCLUDED_)
