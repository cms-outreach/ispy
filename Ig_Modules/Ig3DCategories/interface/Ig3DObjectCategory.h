#ifndef IG_3D_CATEGORIES_IG_3D_BASE_OBJECT_CATEGORY_H
# define IG_3D_CATEGORIES_IG_3D_BASE_OBJECT_CATEGORY_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/Ig3DCategories/interface/config.h"
# include "Ig_Modules/IgControlCentre/interface/IgControlCategory.h"
# include "Ig_Modules/Ig3DBaseModel/interface/Ig3DBaseModel.h"
# include <classlib/utils/DebugAids.h>
# include <Inventor/nodes/SoGroup.h>
# include <algorithm>
# include <vector>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class Ig3DBaseBrowser;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

template <class T>
class Ig3DObjectCategory : public IgControlCategory
{
public:
    Ig3DObjectCategory (IgState *state, QString title,
                        int flags = MultiItem | 
			            ManageItems |
				    MonitorChanges );
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    virtual size_t	objects		(void) const;
    virtual size_t	find		(T *object);
    virtual void	add		(T *object, bool option = true);

    virtual void	remove		(T *object);
    virtual void	remove		(size_t index);
    virtual void	replace		(T *object, T *with);
    virtual void	replace		(size_t index, T *with);
    virtual void	rename		(T *object, const QString& newName);
    virtual void	rename		(size_t index, const QString& newName);
    virtual void	clear		(void);
    virtual void	registerBrowser (IgState *state, 
					 Ig3DBaseBrowser */*browser*/);
protected:
    virtual T *		object (size_t index) const;

private:
    std::vector<T *>	m_objects;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

template <class T>
inline
Ig3DObjectCategory<T>::Ig3DObjectCategory (IgState *state,
					   QString title, int flags)
    : IgControlCategory (state, title, flags )
{}

template <class T>
inline size_t
Ig3DObjectCategory<T>::objects (void) const
{ return m_objects.size (); } 

template <class T>
inline T *
Ig3DObjectCategory<T>::object (size_t index) const
{ 
    if (m_objects.size () != 0)
    {
	ASSERT (index < m_objects.size ()); 
	return m_objects [index]; 
    }
    else
    {
	return 0;	
    }
}

template <class T>
inline size_t
Ig3DObjectCategory<T>::find (T *object)
{
    size_t pos = std::find (m_objects.begin (), m_objects.end (), object)
	    	 - m_objects.begin ();
    ASSERT (pos != m_objects.size ());
    ASSERT (pos != (size_t) items ());
    return pos;
}

template <class T>
inline void
Ig3DObjectCategory<T>::add (T *object, bool option /* = true */)
{
    m_objects.push_back (object);
    addItem (Ig3DBaseModel::decode (object->getName()).c_str (),
             option);
}

template <class T>
inline void
Ig3DObjectCategory<T>::remove (T *object)
{
    size_t pos = find (object);
    m_objects.erase (m_objects.begin () + pos);
    removeItem (pos);
}

template <class T>
inline void
Ig3DObjectCategory<T>::remove (size_t index)
{
    ASSERT (index != m_objects.size ());
    ASSERT (index != (size_t) items ());
    m_objects.erase (m_objects.begin () + index);
    removeItem (index);
}

template <class T>
inline void
Ig3DObjectCategory<T>::replace (T *object, T *with)
{ m_objects[find (object)] = with; }

template <class T>
inline void
Ig3DObjectCategory<T>::replace (size_t index, T *with)
{
    ASSERT (index != m_objects.size ());
    ASSERT (index != (size_t) items ());
    m_objects[index] = with;
}

template <class T>
inline void
Ig3DObjectCategory<T>::rename (T *object, const QString& newName)
{	
    m_objects[find (object)]->setName 
	(Ig3DBaseModel::encode (newName.utf8 ().data ())); 
}

template <class T>
inline void
Ig3DObjectCategory<T>::rename (size_t index, const QString& newName)
{
    ASSERT (index != m_objects.size ());
    ASSERT (index != (size_t) items ());
    m_objects[index]->setName 
      (Ig3DBaseModel::encode (newName.utf8 ().data ()));
}

template <class T>
inline void
Ig3DObjectCategory<T>::clear (void)
{
    while (objects ())
	remove ((size_t) 0);
}

// FIXME: This is an hack...There should be a way to get a list of
// browsers from the state.
template <class T>
inline void
Ig3DObjectCategory<T>::registerBrowser (IgState *state,
					Ig3DBaseBrowser */*browser*/)
{
    IgControlCategory::registerMe (state);
}

#endif // IG_3D_CATEGORIES_IG_3D_BASE_OBJECT_CATEGORY_H
