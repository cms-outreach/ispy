#ifndef IGUANA_IG_NET_ERROR_H
# define IGUANA_IG_NET_ERROR_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/utils/Error.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IgNetError : public lat::Error
{
public:
  IgNetError(const std::string &reason, lat::Error *next);
  // implicit copy constructor
  // implicit assignment operator
  // implicit destructor

  virtual std::string explainSelf(void) const;
  virtual lat::Error *clone(void) const;
  virtual void        rethrow(void);

private:
  std::string         m_reason;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_IG_NET_ERROR_H
