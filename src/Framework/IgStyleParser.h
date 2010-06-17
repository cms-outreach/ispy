#ifndef IG_STYLE_PARSER_H
#define IG_STYLE_PARSER_H

#include "Framework/IgTokenizer.h"
#include <sstream>

class IgStyleParser
{
public:
  struct StyleParserError 
  {
  public:
    StyleParserError(const char *text, const char *label)
      : m_text(text),
        m_label(label)
    {}
    std::string explain()
    {
      return m_text + " " + m_label;
    }
  private:
    std::string m_text;
    std::string m_label;
  };
  
  enum StyleParserStates
  {
    IN_BEGIN_DOCUMENT,
    IN_COMMENT,
    IN_RULE_SPEC,
    IN_BEGIN_PROPERTIES,
    IN_PROPERTY_LABEL,
    IN_PROPERTY_VALUE,
    IN_SINGLE_QUOTED,
    IN_DOUBLE_QUOTED,
    IN_END_PROPERTY,
    IN_EMPTY_PROPERTY,
    IN_END_STYLE,
    IN_WHITESPACE,
    IN_INVALID_STATE,
    IN_END_DOCUMENT
  };
  
  /** Parses the some CSS like syntax reading it from @a in stream.*/
  IgStyleParser(std::istream &in)
    : m_in(in),
      m_bufferSize(1024),
      m_buffer((char*)malloc(m_bufferSize))
  {}
  
  ~IgStyleParser()
  {
    free(m_buffer);
  }
  
  /** */
  void parse()
  {
    static const char * stateLabels[] = {
      "IN_BEGIN_DOCUMENT",
      "IN_COMMENT",
      "IN_RULE_SPEC",
      "IN_BEGIN_PROPERTIES",
      "IN_PROPERTY_LABEL",
      "IN_PROPERTY_VALUE",
      "IN_SINGLE_QUOTED",
      "IN_DOUBLE_QUOTED",
      "IN_END_PROPERTY",
      "IN_EMPTY_PROPERTY",
      "IN_END_STYLE",
      "IN_WHITESPACE",
      "IN_INVALID_STATE",
      "IN_END_DOCUMENT"
    };

    m_nextChar = m_in.get();
    m_state = IN_BEGIN_DOCUMENT;
    m_nextState = IN_INVALID_STATE;
    m_previousState = IN_INVALID_STATE;
    
    while(true)
    {
      m_nextState = handleState();
      if (m_nextState == IN_END_DOCUMENT)
        break;
      if (m_nextState == IN_INVALID_STATE)
        throw StyleParserError("Error while in state: ", stateLabels[m_state]);
      m_previousState = m_state;
      m_state = m_nextState;
    }
  }
  /** Skips the passed delimiter */
  void skipDelim(int delimiter)
  {
    if (m_nextChar == delimiter)
    {
      m_nextChar = m_in.get();
      return;
    }
    char buf[3] = {0, 0, 0};
    buf[0] = m_nextChar;
    buf[1] = delimiter;
    throw StyleParserError("Wrong delimiter", buf);
  }
  /** Skips spaces from the input stream. */
  void stripWhitespaces()
  {
    while(isspace(m_nextChar))
      m_nextChar = m_in.get();
  }
  
  /** Actually parses the style. */
  enum StyleParserStates handleState()
  {
    switch(m_state)
    {
      case IN_BEGIN_DOCUMENT:
        stripWhitespaces();
        if(isalpha(m_nextChar) || m_nextChar == '*')
          return IN_RULE_SPEC;
        if(m_nextChar == '/')
          return IN_COMMENT;
        return IN_INVALID_STATE;

      case IN_RULE_SPEC:
      stripWhitespaces();
        if (m_nextChar == '/')
          return IN_COMMENT;
        fgettoken(m_in, &m_buffer, &m_bufferSize, " \t\n{", &m_nextChar);
        if (m_nextChar == EOF)
          return IN_INVALID_STATE;
        ruleSpec(m_buffer);
        return IN_BEGIN_PROPERTIES;

      case IN_BEGIN_PROPERTIES:
        stripWhitespaces();
        if (m_nextChar == '{')
        {
          m_nextChar = m_in.get();
          stripWhitespaces();
          return IN_PROPERTY_LABEL;
        }
        else if (m_nextChar == '/')
          return IN_COMMENT;
        return IN_INVALID_STATE;

      case IN_PROPERTY_LABEL:
        if (!isalpha(m_nextChar) && m_nextChar != '-')
           return IN_INVALID_STATE;
        fgettoken(m_in, &m_buffer, &m_bufferSize, ":", &m_nextChar);
        m_currentLabel = m_buffer;
        skipDelim(':');
        stripWhitespaces();
        if (m_nextChar == '"')
           return IN_DOUBLE_QUOTED;
        else if (m_nextChar == '\'')
           return IN_SINGLE_QUOTED;
        return IN_PROPERTY_VALUE;

      case IN_DOUBLE_QUOTED:
        return IN_PROPERTY_VALUE;

      case IN_SINGLE_QUOTED:
        return IN_PROPERTY_VALUE;

      case IN_PROPERTY_VALUE:
        if (m_previousState == IN_DOUBLE_QUOTED)
        {
          skipDelim('"');
          fgettoken(m_in, &m_buffer, &m_bufferSize, "\"", &m_nextChar);
          propertySpec(m_currentLabel, m_buffer);
          skipDelim('"');
        }
        if (m_previousState == IN_SINGLE_QUOTED)
        {  
          skipDelim('\'');
          fgettoken(m_in, &m_buffer, &m_bufferSize, "\"", &m_nextChar);
          propertySpec(m_currentLabel, m_buffer);
          skipDelim('\'');
        }
        else 
        {
          fgettoken(m_in, &m_buffer, &m_bufferSize, ";", &m_nextChar);
          propertySpec(m_currentLabel, m_buffer);
        }
        stripWhitespaces();
        skipDelim(';');
        stripWhitespaces();
        if (m_nextChar == '}')
          return IN_END_STYLE;
        return IN_PROPERTY_LABEL;
                
      case IN_END_STYLE:
        skipDelim('}');
        stripWhitespaces();
        std::cerr << m_nextChar << std::endl;
        if (m_nextChar == EOF)
          return IN_END_DOCUMENT;
        return IN_RULE_SPEC;
      
      case IN_COMMENT:
        m_nextChar = m_in.get();
        if (m_nextChar == '/')
        {
          while(m_nextChar != '\n')
            m_nextChar = m_in.get();
        }
        else if (m_nextChar == '*')
          while(true)
          {
            while(m_nextChar != '*')
              m_nextChar = m_in.get();
            m_nextChar = m_in.get();
            if (m_nextChar == '/')
              break;
          }
        m_nextChar = m_in.get();
        return m_previousState;
      
      default:
      {
        std::ostringstream str;
        str << m_state;
        throw StyleParserError("Unhandled state.", str.str().c_str());
      }
    }
  }
  
  /** Invoked for every style definition parsed. 
  
      @a rule which has to be matched for the subsequent properties to apply.
    */
  virtual void ruleSpec(const std::string &rule) = 0;
  
  /**Emitted every time a key value pair in the style is found.
  
     @a label is the parsed name of the property, e.g. "color".
     @a value is the parsed value of the property, e.g. "#ff0000".
  
    */
  virtual void propertySpec(const std::string &label, const std::string &value) = 0;
private:
  std::istream                &m_in;
  int                         m_nextChar;
  enum StyleParserStates      m_state;
  enum StyleParserStates      m_nextState;
  enum StyleParserStates      m_previousState;
  std::string                 m_currentLabel;
  size_t                      m_bufferSize;
  char                        *m_buffer;
};

#endif