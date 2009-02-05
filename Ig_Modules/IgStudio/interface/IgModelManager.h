#ifndef IG_STUDIO_IG_MODEL_MANAGER_H
# define IG_STUDIO_IG_MODEL_MANAGER_H

//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgStudio/interface/IgDocumentData.h"
#include "Ig_Modules/IgStudio/interface/IgDocumentDataManager.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgDocumentDataRoot.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgModel;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

// REMINDER: used to initialize system before creating Ig3DBaseModels.
// Ig3DSystem::init (m_state, 
//		  IgQtSite::selfFrom (site)->topLevelWidget ());

class IG_STUDIO_API IgModelManager : public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (IgModelManager);
public:
    IgModelManager (IgState *m_state);
    ~IgModelManager (void);
    
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    typedef std::map<std::string, std::vector<IgModel *> > ModelList;
    
    /** Searches for model of type T within the list of all models, ,
     * create it if id does not exist and change the passed
     * pointer so that it point to the object of the kind requested.*/
    template <class T>
    bool lookup (T **model, std::string name = "")
    {
	*model = 0;
	if (name.size () == 0)
	    name = IgDocumentDataRoot::getCurrentRoot ();
	    
	ModelList::iterator i =  m_models.find (name);
	
	if (i != m_models.end ())
	{
	    for (std::vector<IgModel *>::iterator j = i->second.begin ();
		 j != i->second.end (); j++)
	    {
	        *model = dynamic_cast<T*> (*j);
		if (*model)
		    return true;
	    }
	}
	return false;
    }
    
    template <class T>
    IgState* lookup (IgState *state, T **model, std::string name = "")
    {
	if (name.size () == 0)
	    name = IgDocumentDataRoot::getCurrentRoot ();
	lookup (model, name);
	    
	state = IgDocumentDataManager::get (state ? state : m_state)->
	           lookup(state, name);
	
	if (!*model)
	{
	    *model = new T (state);
	    m_models[name].push_back (*model);
	}
	return state;
    }

    template <class T>
    std::vector<std::string> roots (T */* model */)
    {
	std::vector<std::string> names;
	    
	for (ModelList::iterator i = m_models.begin ();
	     i != m_models.end (); i++)
	{
	    for (std::vector<IgModel *>::iterator j = i->second.begin ();
		 j != i->second.end (); j++)
	    {
		if (dynamic_cast<T*> (*j))
		{
		    names.push_back (i->first);
		    break;
	        }
            }
       }
	    
       return names;
    }
        
    void add (IgState *state, IgModel* model, std::string name = "");
    std::vector<std::string> roots (void);
    IgState* state (void);

private:
    IgState	*m_state;
    ModelList	m_models;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_STUDIO_IG_MODEL_MANAGER_H
