//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Framework/interface/IgEnvsElement.h"
#include <stdlib.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (IgEnvsElement, "Services/Global/Envs");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgEnvsElement::IgEnvsElement (IgState *state)
    : m_state (state)
{ m_state->put (s_key, this); }

bool
IgEnvsElement::getEnv(const std::string& name, std::string& value) const
{
  char *envValue = getenv (name.c_str());
  if (envValue != NULL)
  {
    value = envValue;
    return true;
  }
  return false;
}

bool
IgEnvsElement::getEnv(const std::string& name, std::vector<std::string>& value,
                   char separator /* = ':' */) const
{
  std::string all;
  if (getEnv(name, all))
  {
    std::string item ("");
    size_t size = all.size();
    for (size_t i=0; i<size; i++)
    {
      if (all[i] == separator)
      {
        value.push_back (item);
	item="";
      }
      else
        item.push_back(all[i]);
    }
    value.push_back (item);
    return true;
  }
  return false;
}

bool
IgEnvsElement::getEnv(const std::string& name, int& value) const
{
  std::string all;
  if (getEnv(name, all))
  {
    value = atoi(all.c_str());
    return true;
  }
  return false;
}

bool
IgEnvsElement::getEnv(const std::string& name, long& value) const
{
  std::string all;
  if (getEnv(name, all))
  {
    value = atol(all.c_str());
    return true;
  }
  return false;
}

bool
IgEnvsElement::getEnv(const std::string& name, unsigned int& value) const
{
  unsigned long value1;
  if (getEnv(name, value1))
  {
    value = (unsigned int)(value1);
    return true;
  }
  return false;
}

bool
IgEnvsElement::getEnv(const std::string& name, unsigned long& value) const
{
  std::string all;
  if (getEnv(name, all))
  {
    value = strtoul(all.c_str(), (char**)NULL, 10);
    return true;
  }
  return false;
}

bool
IgEnvsElement::getEnv(const std::string& name, float& value) const
{
  std::string all;
  if (getEnv(name, all))
  {
    value = atof(all.c_str());
    return true;
  }
  return false;
}

bool
IgEnvsElement::getEnv(const std::string& name, double& value) const
{
  std::string all;
  if (getEnv(name, all))
  {
    value = strtod(all.c_str(), (char**)NULL);
    return true;
  }
  return false;
}
