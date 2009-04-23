#ifndef VIEW_IG_DRAW_FACTROY_SERVICE_H
# define VIEW_IG_DRAW_FACTROY_SERVICE_H

# include "Iguana/Framework/interface/IgStateElement.h"

class SoNode;

class IgDrawFactoryService : public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (IgDrawFactoryService);
public:
    typedef SoNode * (*CreateDrawCallback)(IgState *state, const std::string &collection,
					   const std::string &name,
					   const std::string &rep);

    IgDrawFactoryService (IgState *state);
    ~IgDrawFactoryService (void);    

    // Returns 'true' if registration was successful
    bool                registerDrawFunction (const std::string &name, CreateDrawCallback createFn);
    
    // Returns 'true' if the Collection name was registered before
    bool                unregisterDrawFunction (const std::string &name);
    SoNode *     	draw (const std::string &name, 
			      IgState *state, const std::string &collection,
			      const std::string &rep);
    
private:
    typedef std::map<const std::string, CreateDrawCallback> CallbackMap;

    IgState                     *m_state;
    CallbackMap                 m_callback;
};

#endif // VIEW_IG_DRAW_FACTROY_SERVICE_H
