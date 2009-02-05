// IgQtInfoStream.cpp: implementation of the IgQtInfoStream class.
//
//////////////////////////////////////////////////////////////////////

#include "Ig_Modules/IgVis/interface/IgQtInfoStream.h"

const QString IgQtInfoStream::className = QString("IgQtInfoStream");
int IgQtInfoStream::serialNo = 0;

// single global status widget
IgQtInfoStream* IgQtInfoStream::defaultStream = NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IgQtInfoStream* IgQtInfoStream::getDefaultInfoStream(QWidget *parent)
{
	// the parent widget is first-come, first serve
	if (!defaultStream)
	{
		// create the default stream
		defaultStream = new IgQtInfoStream(parent);
		setDefaultCaption("Info"); 
		defaultStream->the_default = true;
	}
	return defaultStream;
}
IgQtInfoStream::IgQtInfoStream(QWidget *parent) :
std::ostream(&m_streamBuf), the_default(false)
{
	QString sn;
	sn.setNum(serialNo++);
	QString uniqueName = className+sn;
	theBrowser = new QTextBrowser(parent, uniqueName);
	m_streamBuf.setWidget(theBrowser);
	theBrowser->setMinimumHeight( 100 );
}

IgQtInfoStream::~IgQtInfoStream()
{
	theBrowser->close(true); // close and delete the widget
}

IgQtInfoStream::IgQtInfoStreamBuf::IgQtInfoStreamBuf(QTextBrowser *pWidget) :
	statusWidget(pWidget)
{
}
#ifdef IGUANA_HAVE_NEW_IOSTREAMS
IgQtInfoStream::int_type IgQtInfoStream::IgQtInfoStreamBuf::overflow(int_type int_char) 
#else
int IgQtInfoStream::IgQtInfoStreamBuf::overflow(int int_char) 
#endif
{
#ifdef IGUANA_HAVE_NEW_IOSTREAMS
  const char_type aChar = std::char_traits<char_type>::to_char_type(int_char);
  if (int_char != traits_type::eof() && int_char != int_type('\n')) {
#else
  char aChar = int_char;
  if (int_char != EOF && aChar != '\n' && aChar != '\0') {
#endif
    m_buffer.append(aChar); }
  else {
#ifdef IGUANA_HAVE_NEW_IOSTREAMS
    if (int_char != traits_type::eof() ) m_buffer.append(aChar);
#else
    if (int_char != EOF && aChar != '\0') m_buffer.append(aChar);
#endif
//  will use append, so don't need following format
#if 0 //unused
	text.append(m_buffer);
    if (statusWidget != NULL) statusWidget->setText(text);
#else //unused
    if (statusWidget != NULL) statusWidget->append(m_buffer);
#endif //unused
    m_buffer = ""; //reset the buffer
	statusWidget->ensureVisible(0,statusWidget->contentsHeight(),0,20); //scroll to bottom
  }
  return int_char;
}
void IgQtInfoStream::IgQtInfoStreamBuf::setWidget(QTextBrowser *pWidget)
{
		statusWidget = pWidget;

}
std::streamsize IgQtInfoStream::IgQtInfoStreamBuf::xsputn(const char *c,std::streamsize nc)
{
	bool do_flush = false;
#if 0 // do we really need to check for newline & flush on each character?
	for (int i = 0; i < nc; i++) {
		if (!do_flush) do_flush = *c == '\n';
		m_buffer += *c++;
	}
#else
	int start = m_buffer.length();
    QCString tmpString(c, nc+1); // pick up only first nc chars from c, plus room for \0
    m_buffer += tmpString;
	do_flush = (m_buffer.find('\n',start) != -1 || m_buffer.find('\0',start) != -1);
#endif
	if (do_flush) {
		statusWidget->append(m_buffer);
		m_buffer = "";
	}
	return nc;
}
void IgQtInfoStream::IgQtInfoStreamBuf::clear()
{
	statusWidget->setText("");
}
void IgQtInfoStream::clear()
{
	flush();
	m_streamBuf.clear();
}
QWidget* IgQtInfoStream::getWidget(void)
{
    return theBrowser;
}
void IgQtInfoStream::setCaption(const QString& caption)
{
	// must use setDefaultCaption to set the caption on the default
	if (!the_default)
	{
		theBrowser->setCaption(caption);
	}
}
void IgQtInfoStream::setDefaultCaption(const QString& caption)
{
	defaultStream->theBrowser->setCaption(caption);
}
void IgQtInfoStream::show()
{
	theBrowser->show();
}
