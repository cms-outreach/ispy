#ifndef ISPY_LAYOUT_PARSER
#define ISPY_LAYOUT_PARSER

#include "Iguana/Framework/interface/SimpleSAXParser.h"
#include "Iguana/QtGUI/interface/ISpyApplication.h"

class ISpyLayoutParser : public SimpleSAXParser
{
public:
  ISpyLayoutParser(ISpyApplication *app);
  
};

#endif