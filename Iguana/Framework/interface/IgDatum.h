#ifndef IGUANA_FRAMEWORK_IG_DATUM_H
# define IGUANA_FRAMEWORK_IG_DATUM_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/config.h"
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

template <class T> class IGUANA_FRAMEWORK_API IgSimpleDatum;

typedef IgSimpleDatum<std::string>	IgStringDatum;
typedef IgSimpleDatum<bool>		IgBoolDatum;
typedef IgSimpleDatum<long>		IgIntDatum;
typedef IgSimpleDatum<double>		IgRealDatum;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_FRAMEWORK_API IgDatum
{
public:
    virtual ~IgDatum (void);
};

template <class T>
class IGUANA_FRAMEWORK_API IgSimpleDatum : public IgDatum
{
public:
    explicit IgSimpleDatum (const T &value);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    const T &		value (void) const;
    void		value (const T &newval);

private:
    T			m_value;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

template <class T>
inline
IgSimpleDatum<T>::IgSimpleDatum (const T &value)
    : m_value (value)
{}

template <class T>
inline const T &
IgSimpleDatum<T>::value (void) const
{
    return m_value;
}

template <class T>
inline void
IgSimpleDatum<T>::value (const T &newval)
{
    m_value = newval;
}

#endif // IGUANA_FRAMEWORK_IG_DATUM_H
