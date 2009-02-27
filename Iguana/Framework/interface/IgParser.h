#ifndef IGUANA_IGGI_IG_PARSER_H
#define IGUANA_IGGI_IG_PARSER_H
# ifdef PROJECT_NAME 
# include <Iguana/Framework/interface/IgCollection.h>
# else
# include <IgCollection.h>
# endif

class ParseError
{
public:
  enum { POSITION_UNKNOWN = -1 };
  ParseError(int position = POSITION_UNKNOWN)
  :m_position(position)
  {}
private:
  int m_position;
};

class IgParser
{
public:
  IgParser (IgDataStorage *storage)
  :m_storage(storage)
  {
  }
  
  // FIXME: improve IgCollection::addProperty API
  //        there should be an IgCollection::addProperty<T>(const char *)
  //        option which avoids having to specify the actual type.
  void createColumn(const std::string &name, const std::string &type)
  {
    if(type == "string")
    {
      m_currentCollection->addProperty(name.c_str(), std::string());
    }
    else if(type == "int")
    {
      m_currentCollection->addProperty(name.c_str(), static_cast<int>(0));      
    }
    else if(type == "double")
    {
      m_currentCollection->addProperty(name.c_str(), static_cast<double>(1.0));
    }
    else if(type == "v2d")
    {
      m_currentCollection->addProperty(name.c_str(), IgV2d());
    }
    else if(type == "v3d")
    {
      m_currentCollection->addProperty(name.c_str(), IgV3d());
    }
    else if(type == "v4d")
    {
      m_currentCollection->addProperty(name.c_str(), IgV4d());
    }
  }
  
  void skipSpaces(void)
  {
    while(*m_buffer && (*m_buffer == '\n' || *m_buffer == '\t' || *m_buffer == ' '))
    {
      m_buffer++;
    }
  }
  
  bool checkChar(const char c)
  {
    skipSpaces();
    if(*m_buffer != c)
    {
      return false;
    }
    skipSpaces();
    ++m_buffer;
    return true;
  }
  
  void skipChar(const char c)
  {
    if(!checkChar(c))
    { throw ParseError(m_buffer-m_initialBuffer); }
  }

  void parseInt (int &result)
  {
    skipSpaces();
    char *endbuf;
    result = strtol(m_buffer, &endbuf, 10);
    if (!endbuf) 
    { throw ParseError(m_buffer-m_initialBuffer); }
    m_buffer = endbuf;
    skipSpaces();
  }

  void parseDouble (double &result)
  {
    skipSpaces();
    char *endbuf;
    result = strtod(m_buffer, &endbuf);
    if (!endbuf) 
    { throw ParseError(m_buffer-m_initialBuffer); }
    m_buffer = endbuf;
    skipSpaces();
  }

  void parseString(std::string &result)
  {
    skipSpaces();
    char delimeter = 0;
    if(*m_buffer == '\"')
      delimeter = '\"';
    else
      delimeter = '\'';
    skipChar(delimeter);
    const char *endbuf = m_buffer;
    while(true)
    { 
      if (*endbuf == 0)
      { break; }
      if ((*endbuf == delimeter) && (*(endbuf - 1) != '\\'))  
      { break; }
      endbuf++; 
    }
    result = std::string(m_buffer, endbuf-m_buffer);
    m_buffer = endbuf;
    skipChar(delimeter);
    skipSpaces();
  }
  
  void parseDoubleTuple(double *result)
  {
    skipSpaces();
    skipChar('(');
    int i = 0;
    do
    {
      parseDouble(result[i]);
      i++;
    } while(checkChar(','));
    skipChar(')');
    skipSpaces();
  }
  
  void parseAssociation(void)
  {
    int collectionIdA, objectIdA;
    int collectionIdB, objectIdB;

    skipChar('[');  
    skipSpaces();
    skipChar('[');
    parseInt(collectionIdA);
    skipChar(',');
    parseInt(objectIdA);
    skipChar(']');
    skipChar(',');
    skipChar('[');
    parseInt(collectionIdB);
    skipChar(',');
    parseInt(objectIdB);
    skipChar(']');
    skipSpaces();
    skipChar(']');
    
    m_currentAssociationSet->associate(IgRef (collectionIdA, objectIdA),
                                       IgRef (collectionIdB, objectIdB));
  }
  
  void parseAssociationSet(void)
  {
    std::string name;
    parseString(name);
    m_currentAssociationSet = m_storage->getAssociationSetPtr(name.c_str());
    skipChar(':');
    skipChar('[');
    if (checkChar(']')) return;
    do
    {
      parseAssociation();
    } while(checkChar(','));

    skipChar(']');
  }
  
  void checkMagic(const char *magicName)
  {
    std::string magic;
    parseString(magic);
    if (magic != magicName)
      { throw(ParseError()); }
  }
  
  
  void parseAssociationSets(void)
  {
    checkMagic("Associations");
    skipChar(':');
    skipChar('{');
    if (checkChar('}')) return;
    do
    {
      parseAssociationSet();
    } while(checkChar(','));
    skipChar('}');  
  }
  
  void parseCollection(void)
  {
    std::string collectionName;
    double doubleBuffer[4];
    std::string stringBuffer;
    
    stringBuffer.reserve(1024);
    
    parseString(collectionName);
    
    m_currentCollection = m_storage->getCollectionPtr(collectionName.c_str());
    m_currentCollection->reserve(100000);
    skipChar(':');
    skipChar('[');
    if (checkChar(']')) return;
    
    do {
      skipChar('[');
      assert(m_currentCollection->properties().size());
      IgCollectionItem item = m_currentCollection->create();
      for (IgCollection::Properties::iterator i = m_currentCollection->properties().begin();
           i != m_currentCollection->properties().end();
           i++)
      {
        switch ((*i).handle().type())
        {
          case INT_COLUMN:
            {
              int result;
              parseInt(result);
              item[*i] = result;
            }
            break;
          case STRING_COLUMN:
            { 
              stringBuffer.clear();
              parseString(stringBuffer);
              item[*i] = stringBuffer;
            }
            break;
          case DOUBLE_COLUMN:
            {
              double result;
              parseDouble(result);
              item[*i] = result;
            }
            break;
          case VECTOR_2D:
            {
              parseDoubleTuple(doubleBuffer);
              item[*i] = IgV2d(doubleBuffer[0], doubleBuffer[1]);
            }
            break;
          case VECTOR_3D:
            {
              parseDoubleTuple(doubleBuffer);
              item[*i] = IgV3d(doubleBuffer[0], doubleBuffer[1], doubleBuffer[2]);
            }
            break;
          case VECTOR_4D:
            {
              parseDoubleTuple(doubleBuffer);
              item[*i] = IgV4d(doubleBuffer[0], doubleBuffer[1], 
                               doubleBuffer[2], doubleBuffer[3]);
            }
            break;          
          default:
            std::cerr << "Unknown type" << std::endl;
            assert(false);
            break;
        }
        if (i+1 != m_currentCollection->properties().end())
          skipChar(',');
      }
      skipChar(']');
    } while(checkChar(','));
    skipChar(']');
    m_currentCollection->compress();
  }
  
  void parseCollections(void)
  {
    checkMagic("Collections");
    skipChar(':');
    skipChar('{');
    if (checkChar('}')) return;
    do
    {
      parseCollection();
    }while(checkChar(','));
    skipChar('}');
  }
  
  void parseTypeDefinition(void)
  {
    std::string type;
    std::string name;
    skipChar('[');
    parseString(name);
    skipChar(',');
    parseString(type);
    skipChar(']');
    createColumn(name, type);
  }
  
  void parseTypes(void)
  {
    std::string magic;
    parseString(magic);
    if (magic != "Types")
      { throw (ParseError()); }
    skipChar(':');
    skipChar('{');
    if (checkChar('}')) return;
    
    do
    {
      std::string typeName;
      parseString(typeName);
      skipChar(':');
      skipChar('[');
      if (checkChar(']')) 
      {
        skipChar('}');
        return;
      }
      m_currentCollection = m_storage->getCollectionPtr(typeName.c_str());
      do
      {
        parseTypeDefinition();
      }while(checkChar(','));
      skipChar(']');
    }while(checkChar(','));
    skipChar('}');
  }
  
  void parse(const char *buffer)
  {
    m_buffer = buffer;
    m_initialBuffer = buffer;
    
    try
    {
      skipChar('{');
      parseTypes();
      skipChar(',');
      parseCollections();
      skipChar(',');
      parseAssociationSets();
      skipChar('}');
    } 
    catch (ParseError &e)
    {
      int position = m_buffer - m_initialBuffer ;
      std::string parsedSentence (m_initialBuffer + position);
      size_t endOfLinePos = parsedSentence.find('\n');
      if (endOfLinePos != std::string::npos)
      {
        parsedSentence = std::string(parsedSentence, endOfLinePos);
      }
      std::cerr << "Error at char n. " << position << std::endl;
      std::cerr << parsedSentence << std::endl;
      std::cerr << "^" << std::endl;
      throw ParseError(position);
    }
  }
private:
  IgDataStorage *m_storage;
  IgCollection *m_currentCollection;
  IgAssociationSet *m_currentAssociationSet;
  const char *m_buffer;
  const char *m_initialBuffer;
};

#endif // IGUANA_IG_FILE_IG_PARSER_H
