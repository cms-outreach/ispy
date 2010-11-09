#include "Framework/IgStyleParser.h"
#include <cassert>
#include <fstream>

class TestParser : public IgStyleParser
{
public:
  TestParser(std::istream &f)
    : IgStyleParser(f)
  {}
  
  void ruleSpec(const std::string &rule)
  {
    std::cout << rule << std::endl;
  }
  
  void propertySpec(const std::string &label, const std::string &value)
  {
    std::cout << label << " " << value << std::endl;    
  }
};

int
main(int argc, char **argv)
{
  std::ifstream f("src/QtGUI/default-style.iss");
  TestParser parser(f);

  try
  {
    parser.parse();
  } 
  catch (IgStyleParser::StyleParserError &e)
  {
    std::cerr << e.explain() << std::endl;
    exit(1);
    assert(false);
  }
  
  std::stringstream str("* {color: rgb(.7,.7,.7) ;} Background {diffuse-color: rgb(1., 0, 0);}");
  std::cerr << str.str() << std::endl;
  TestParser styleParser(str);
  try
  {
    styleParser.parse();
  } 
  catch (IgStyleParser::StyleParserError &e)
  {
    std::cerr << e.explain() << std::endl;
    exit(1);
    assert(false);
  }
}
