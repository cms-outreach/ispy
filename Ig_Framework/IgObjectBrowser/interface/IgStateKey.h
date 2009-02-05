#ifndef IG_OBJECT_BROWSER_IG_STATE_KEY_H
# define IG_OBJECT_BROWSER_IG_STATE_KEY_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Framework/IgObjectBrowser/interface/config.h"
# include <string>
# include <map>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_OBJECT_BROWSER_API IgStateKey
{
public:
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    /**/		operator unsigned (void) const;
    unsigned		value (void) const;
    std::string		label (void) const;

    static IgStateKey	index (const std::string &label);

protected:
    IgStateKey (unsigned value);

    static unsigned	create (const std::string &label);
    static void		remove (const std::string &label);

private:
    unsigned int	m_value;

    typedef std::map<std::string, unsigned> Map;
    static Map &	keymap (void);
    static unsigned	s_count;
};

class IG_OBJECT_BROWSER_API IgStateKeyDef : public IgStateKey
{
public:
    IgStateKeyDef (const std::string &label);
    ~IgStateKeyDef (void);

private:
    std::string		m_label;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

/** Return the index key of this state key.  */
inline
IgStateKey::operator unsigned (void) const
{ return m_value; }

/** Return the index key of this state key.  */
inline unsigned
IgStateKey::value (void) const
{ return m_value; }

#endif // IG_OBJECT_BROWSER_IG_STATE_KEY_H
