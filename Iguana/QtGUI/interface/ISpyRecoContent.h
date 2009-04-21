#ifndef VIEW_IVIEW_RECO_CONTENT_H
# define VIEW_IVIEW_RECO_CONTENT_H

class IgState;

class ISpyRecoContent 
{
public:
    ISpyRecoContent (IgState *state);

private:
    void                init (void);
    IgState *		state (void) const;

    IgState * 		m_state;
};

#endif // VIEW_IVIEW_RECO_CONTENT_H
