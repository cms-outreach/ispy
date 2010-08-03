#include "IgSubProcess.h"

IgSubProcess::IgSubProcess()
{
  connect(this, SIGNAL(readyReadStandardOutput()), this, SLOT(readMore()));

}
void
IgSubProcess::readMore()
{
	m_buffer.append(readAllStandardOutput());
}
