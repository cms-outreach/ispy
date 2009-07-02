#ifndef IGUANA_IG_PARSER_H
#define IGUANA_IG_PARSER_H
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

  int position(void) const
    { return m_position; }

private:
  int m_position;
};

class IgParser
{
public:
  IgParser(IgDataStorage *storage)
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
      while((*m_buffer == ' ' || *m_buffer == '\n' || *m_buffer == '\t' ) && *m_buffer)
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
      ++m_buffer;
      return true;
    }

  void throwParseError(const int position)
    {
      throw ParseError(position);
    }

  void skipChar(const char c)
    {
      if(!checkChar(c))
      { throwParseError(m_buffer-m_initialBuffer); }
    }

  void parseInt(int &result)
    {
      skipSpaces();
      char *endbuf;
      result = strtol(m_buffer, &endbuf, 10);
      if (!endbuf)
      { throwParseError(m_buffer-m_initialBuffer); }
      m_buffer = endbuf;
    }

  void parseDouble(double &result)
    {
      skipSpaces();
      char *endbuf;
      result = strtod(m_buffer, &endbuf);
      if (!endbuf)
      { throwParseError(m_buffer-m_initialBuffer); }
      m_buffer = endbuf;
    }

  void parseString(std::string &result)
    {
      skipSpaces();
      char delimeter[] = "\'";
      if(*m_buffer == '\"')
        delimeter[0] = '\"';
      m_buffer++;
      int stringSize = 0;
      while(true)
      {
        stringSize += strcspn(m_buffer + stringSize, delimeter);
        if (m_buffer[stringSize-1] != '\\')
        { break; }
      }
      result.assign(m_buffer, stringSize);
      m_buffer += stringSize+1;
    }

  void parseDoubleTuple(double *result)
    {
      skipChar('(');
      int i = 0;
      do
      {
        parseDouble(result[i]);
        i++;
      } while(checkChar(','));
      skipChar(')');
    }

  void parseAssociation(IgAssociation &result)
    {
      int collectionIdA, objectIdA;
      int collectionIdB, objectIdB;

      skipChar('[');
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
      skipChar(']');
      result.set(collectionIdA, objectIdA, collectionIdB, objectIdB);
    }

  void parseAssociationSet(void)
    {
      std::string name;
      parseString(name);
      m_currentAssociationSet = m_storage->getAssociationSetPtr(name.c_str());
      m_currentAssociationSet->reserve(1000000);
      skipChar(':');
      skipChar('[');
      if (checkChar(']')) return;
      int currentRow = 0;
      int maxSize = 0;
      do
      {
        if (currentRow >= maxSize)
        {
          maxSize += 100;
          m_currentAssociationSet->resize(maxSize);
        }
        parseAssociation((*m_currentAssociationSet)[currentRow++]);
      } while(checkChar(','));
      m_currentAssociationSet->resize(currentRow);
      m_currentAssociationSet->compress();
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
      parseString(m_currentCollectionName);
      m_currentCollection = m_storage->getCollectionPtr(m_currentCollectionName.c_str());
      m_currentCollection->reserve(200000);
      skipChar(':');
      skipChar('[');
      if (checkChar(']')) return;
      int maxSize = 0;
      int currentRow = 0;
      do {
        if (currentRow >= maxSize)
        {
          maxSize += 10;
          m_currentCollection->resize(maxSize);
        }

        skipChar('[');
        assert(m_currentCollection->properties().size());
        IgCollectionItem item(m_currentCollection, currentRow);
        for (IgCollection::Properties::iterator i = m_currentCollection->properties().begin();
             i != m_currentCollection->properties().end();
             i++)
        {
          switch ((*i).handle().type())
          {
          case INT_COLUMN:
            parseInt(item.current<int>());
            break;
          case STRING_COLUMN:
            parseString(item.current<std::string>());
            break;
          case DOUBLE_COLUMN:
            parseDouble(item.current<double>());
            break;
          case VECTOR_2D:
            parseDoubleTuple(item.current<IgV2d>().data());
            break;
          case VECTOR_3D:
            parseDoubleTuple(item.current<IgV3d>().data());
            break;
          case VECTOR_4D:
            parseDoubleTuple(item.current<IgV4d>().data());
            break;
          default:
            std::cerr << "Unknown type" << std::endl;
            assert(false);
            break;
          }

          if (i+1 != m_currentCollection->properties().end())
          {
            skipChar(',');
            item.nextColumn();
          }
        }
        currentRow++;
        skipChar(']');
      } while(checkChar(','));
      m_currentCollection->resize(currentRow);
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
      { throw(ParseError()); }
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
      catch(ParseError &e)
      {
        int position = m_buffer - m_initialBuffer ;
        std::string parsedSentence(m_initialBuffer + position);
        size_t endOfLinePos = parsedSentence.find('\n');
        if (endOfLinePos != std::string::npos)
        {
          parsedSentence = std::string(parsedSentence, endOfLinePos);
        }
        std::cerr << "Error at char n. " << position << std::endl;
        std::cerr << parsedSentence << std::endl;
        std::cerr << "^" << std::endl;
        throwParseError(position);
      }
    }
private:
  IgDataStorage *m_storage;
  IgCollection *m_currentCollection;
  IgAssociationSet *m_currentAssociationSet;
  const char *m_buffer;
  const char *m_initialBuffer;
  std::string m_currentCollectionName;
};

#endif // IGUANA_IG_PARSER_H
