#ifndef IG_PYTHON_IG_SINGLETON_H
# define IG_PYTHON_IG_SINGLETON_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgPython/interface/config.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>
template <class T>
class IgSingleton
{
private:
    static T *&nakedPtr(void)
	{
	    static T *m_instance = NULL;
	    return m_instance;
	};	
    
    static int &nakedRefCounter(void)
	{
	    static int m_refCounter = 0;
	    return m_refCounter;
	};
protected:
    IgSingleton(void);	
    ~IgSingleton();	
    void dispose(void);
public:	
    static void incRefCount(void);
    static void decRefCount(void);
    static T &instance(void);
    static int refCounter(void);
};


//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

template <class T>
inline IgSingleton<T>::IgSingleton(void){};

template <class T>
inline IgSingleton<T>::~IgSingleton(void){};

template <class T>
inline void IgSingleton<T>::dispose()
{
    if(nakedPtr() != 0 ) 
    {
	delete nakedPtr();
	nakedPtr() = 0;
    };
};

template <class T>	
inline void IgSingleton<T>::incRefCount(void)
{
    nakedRefCounter()++;		
};

template <class T>	
inline void IgSingleton<T>::decRefCount(void)
{
    nakedRefCounter()--;
};

template <class T>
inline T &IgSingleton<T>::instance(void)
{
    if (nakedPtr() == 0 ) nakedPtr() = new T;
    return *nakedPtr();
};

template <class T>
inline int IgSingleton<T>::refCounter(void)
{
    return nakedRefCounter();
};

#endif // IG_PYTHON_IG_SINGLETON_H
