#ifndef IG_NTUPLE_MODEL_IG_NTUPLE_REP_H
# define IG_NTUPLE_MODEL_IG_NTUPLE_REP_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgNTupleModel/interface/config.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgRep.h"
# include <vector>
# include <deque>
# include <string>
# include <map>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
class IgRepContext;
class IgNTupleModel;
class IgModel;


//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_NTUPLE_MODEL_API IgNTupleRep : public IgRep
{
public:
    typedef std::string 		ColumnName;    
    typedef std::vector<ColumnName> 	ColumnNames;
    typedef std::vector<double>		Row;
    
    IgNTupleRep (IgNTupleModel *model);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    
    // Methods required by the iguana architecture.
    virtual IgRepContext *	context (void) const;
    virtual IgModel *		model (void) const;
    

    // NTuple managing methods.
    virtual void		copy (const IgNTupleRep &source);    
    virtual void		clear (void);
    virtual void 		addRow (const Row &values);
    virtual const Row 		&getRow (unsigned int row) const;
    virtual double		valueAt (unsigned int column, 
					 unsigned int row) const;
    virtual bool 		empty (void) const;
    virtual void		columnNames (const ColumnNames &names);
    virtual const ColumnNames & columnNames (void) const;
    virtual int				rows (void) const;
    
protected:
    virtual void		context (IgRepContext *context);    
private:
    typedef std::map<ColumnName, unsigned int> 	ColumnNamesMap;
    typedef std::deque<Row> 			Rows;
    ColumnNames					m_columnNames;    
    ColumnNamesMap 				m_columnNamesMap;    
    Rows					m_rows;
    IgNTupleModel *   				m_model;    
    IgRepContext *				m_context;    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_NTUPLE_MODEL_IG_NTUPLE_REP_H
