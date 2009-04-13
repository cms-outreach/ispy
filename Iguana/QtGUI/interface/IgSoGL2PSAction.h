#ifndef IG_OPEN_INVENTOR_IG_SO_GL2PS_ACTION_H
# define IG_OPEN_INVENTOR_IG_SO_GL2PS_ACTION_H

//<<<<<< INCLUDES                                                       >>>>>>

#include <Inventor/actions/SoGLRenderAction.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class SbViewportRegion;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

# ifdef WIN32
#  include <SoWinLeaveScope.h>
# endif

class IgSoGL2PSAction : public SoGLRenderAction
{
    SO_ACTION_HEADER(IgSoGL2PSAction);
public:

    static void		initClass (void);

    IgSoGL2PSAction (const SbViewportRegion&);
protected:
    //virtual void beginTraversal (SoNode *node);
private:
    static void indexedLineSet(SoAction* self, SoNode* node);
    static void lineSet       (SoAction* self, SoNode* node);
    static void pointSet      (SoAction* self, SoNode* node);
    static void text2         (SoAction* self, SoNode* node);
    static void markerSet     (SoAction* self, SoNode* node);
    static void separatorSet  (SoAction* self, SoNode* node);
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif // WIN32

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_OPEN_INVENTOR_IG_SO_GL2PS_ACTION_H
