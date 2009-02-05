//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtProfileService/src/IgOProfProcDevice.h"
#include <classlib/utils/SearchPath.h>
#include <classlib/utils/DebugAids.h>
#include <sstream>
#include <iostream>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>


IgOProfProcDevice::IgOProfProcDevice (const char *prefix)	
    :m_prefix (lat::Filename (prefix).asDirectory ()),
     m_stream (),
     m_enabled (false)
{
    lat::Filename baseDir (m_prefix);
    
    if (baseDir.isDirectory ())
    {
	m_enabled = true;
    }    
    else
    {	
	m_enabled = false;    
    }    
}

IgOProfProcDevice::IgOProfProcDevice (const IgOProfProcDevice &t)
    :m_prefix (t.m_prefix),
     m_stream ()
{
}

bool 
IgOProfProcDevice::writeString (const char *path, const char *value)
{
    if (m_enabled)
    {	
	ASSERT (exists (path));
	ASSERT (m_stream.is_open () == false);
    
	m_stream.open (lat::Filename (m_prefix, path), std::ios::in);
    
	if (m_stream.is_open () == false) return false;
	if (m_stream.good () == false)
	{
	    m_stream.close ();	    
	    return false;
	}    
	m_stream << value;
	m_stream.close ();
	ASSERT (m_stream.is_open () == false);
    
	return true;
    }
    else return false;	
}

bool 
IgOProfProcDevice::writeInt (const char *path, const int value)
{
    if (m_enabled)
    {	
	ASSERT (exists (path));
	ASSERT (m_stream.is_open () == false);
	m_stream.open (lat::Filename (m_prefix, path) , std::ios::out);    
	if (m_stream.is_open () == false) return false;
	if (m_stream.good () == false) 
	{
	    m_stream.close ();	    
	    return false;
	}    
	m_stream << value;
	m_stream.close ();
	ASSERT (m_stream.is_open () == false);
	return true;
    }
    else return false;	
}

bool 
IgOProfProcDevice::writeBool (const char *path, const bool value)
{
    if (m_enabled)
    {	
	ASSERT (exists (path));
	if (value) return IgOProfProcDevice::writeString (path,"1");
	return IgOProfProcDevice::writeString (path,"0");
    }
    else return false;	
}

bool 
IgOProfProcDevice::read (const char *path, std::string & str)
{
    if (m_enabled)
    {	
	ASSERT (exists (path));
	ASSERT (canRead (path));    
	std::ostringstream bufStream;
    
	m_stream.open (lat::Filename (m_prefix,path),std::ios::in);    
	if (m_stream.is_open ()) return false;
	if (m_stream.good () == false) 
	{
	    m_stream.close ();	    
	
	    return false;
	}    
	bufStream << m_stream.rdbuf () << std::ends;    
	m_stream.close ();
	str = bufStream.str ();
	ASSERT (m_stream.is_open () == false);
	return true;
    }
    else return false;    
}

bool 
IgOProfProcDevice::readInt (const char *path, int &result)
{
    if (m_enabled)
    {	
	ASSERT (exists (path));
	ASSERT (canRead (path));
    
	lat::Filename filename (m_prefix,path);
       
	m_stream.open (filename, std::ios_base::in);
	if (m_stream.is_open () == false) 
	{
	    m_stream.close ();
	    m_stream.clear ();	
	    return false;
	}
    
	if (m_stream.good () == false) 
	{
	    m_stream.close ();	    
	    m_stream.clear ();	
	    return false;
	}    

	m_stream >> result;

	if (m_stream.fail () == false)
	{
	    m_stream.close ();
	    ASSERT (m_stream.is_open () == false);
	    return true;	
	}
    
	m_stream.clear ();    
	return false;
    }
    else return false;    
}

bool 
IgOProfProcDevice::readBool (const char *path, bool & result)
{
    if (m_enabled)
    {	
	int res;
	bool success;
    
	ASSERT (exists (path));
	ASSERT (canRead (path));
    
	success = readInt (path,res);    
    
	if (success == true)
	{	
	    if (res != 0) result = true;
	    else result = false;
	    return true;
	}
    
	return false;    
    }
    else return false;    
}

bool
IgOProfProcDevice::exists (const char *path)
{
    lat::SearchPath sp ((const char *) m_prefix);
    lat::Filename filename (path);
    
    if (sp.search (filename) == sp.end () ) return false;
    else return true;    
}

bool
IgOProfProcDevice::canDo (const char *filename,std::ios::openmode flag)
{
    bool success;

    ASSERT (m_stream.is_open () == false);
    if (exists (filename) == true)
    {	
	m_stream.open (lat::Filename (m_prefix, filename), flag);
	success = m_stream.good ();
	m_stream.close ();    
	ASSERT (m_stream.is_open () == false);
	m_stream.clear ();
    }else
    {
	success = false;	
    }    
        
    return success;
}		

bool
IgOProfProcDevice::canWrite (const char *filename)
{
    return canDo (filename, std::ios::out);
}

bool
IgOProfProcDevice::canRead (const char *filename)
{
    return canDo (filename, std::ios::in);
}
