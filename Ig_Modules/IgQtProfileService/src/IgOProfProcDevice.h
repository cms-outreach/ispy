#ifndef __IG_OPROF_PROC_DEVICE_H__
#define __IG_OPROF_PROC_DEVICE_H__

#include <classlib/iobase/Filename.h>
#include <fstream>
#include <string>

/** This is a convenience class to access /proc/ filesystem.
 */
class IgOProfProcDevice
{
public:
    /*!\param prefix this is to be considered the main /proc path to which all the others are relative
     */
    IgOProfProcDevice (const char *prefix);
    /**copy constructor copies the prefix but not the fstream, which is reconstructed*/
    IgOProfProcDevice (const IgOProfProcDevice &t);
    
    /*!Writes a string to the provided /proc file.
       \param path relative path to which to write.
       \param value string to be written
       \return true on success
     */
    bool writeString (const char *path, const char *value);
    /*!Writes an int to the provided /proc file.
       \param path relative path to which to write.
       \param value string to be written
       \return true on success
     */
    bool writeInt (const char *path, const int value);
    /*!Writes a bool to the provided /proc file.
       \param path relative path to which to write.
       \param value string to be written
       \return true on success
     */
    bool writeBool (const char *path, const bool value);
    
    bool read (const char *path, std::string &str);
    bool readInt (const char *path, int &result);
    bool readBool (const char *path, bool &result);
    /*!Checks whether the wanted path exists.
       \param path path to check.
     */
    bool exists (const char *path);
    bool canDo (const char *filename,std::ios::openmode flag);
    /*!Check if it is possible to write to the provided filename.
       \param filename filename to check
     */
    bool canWrite (const char *filename);
    /*!Check if it is possible to read to the provided filename.
       \param filename filename to check
     */
    bool canRead (const char *filename);
    
    bool enabled (void)
	{
	    return m_enabled;	    
	}    
private:    
    lat::Filename 	m_prefix;
    std::fstream 	m_stream;    
    /*!True if the profiler module is loaded, false otherwise.
     */
    bool 		m_enabled;    
};

#endif
