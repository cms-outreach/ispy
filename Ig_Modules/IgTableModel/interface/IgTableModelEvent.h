#ifndef IG_TABLE_MODEL_IG_TABLE_MODEL_EVENT_H
# define IG_TABLE_MODEL_IG_TABLE_MODEL_EVENT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgTableModel/interface/config.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgTableModel;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_TABLE_MODEL_API IgTableModelEvent
{
public:
    enum ChangeType { Insert, Update, Delete };
    static const unsigned all_rows = ~0U;
    static const unsigned header_row = ~0U - 1;
    static const unsigned all_columns = ~0U;

    IgTableModelEvent (IgTableModel *source);
    IgTableModelEvent (IgTableModel *source, unsigned row);
    IgTableModelEvent (IgTableModel *source, unsigned first_row, unsigned last_row);
    IgTableModelEvent (IgTableModel *source, unsigned first_row, unsigned last_row,
		       unsigned column);
    IgTableModelEvent (IgTableModel *source, unsigned first_row, unsigned last_row,
		       unsigned column, ChangeType change);
    // default copy constructor
    // default assignment operator
    // default destructor

    IgTableModel *		source (void) const;
    unsigned			first_row (void) const;
    unsigned			last_row (void) const;
    unsigned			column (void) const;
    ChangeType			change (void) const;

private:
    IgTableModel *		m_source;
    unsigned			m_first_row;
    unsigned			m_last_row;
    unsigned			m_column;
    ChangeType			m_change;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

inline
IgTableModelEvent::IgTableModelEvent (IgTableModel *source)
    : m_source (source), m_first_row (all_rows), m_last_row (all_rows),
      m_column (all_columns), m_change (update)
{}

inline
IgTableModelEvent::IgTableModelEvent (IgTableModel *source, unsigned row)
    : m_source (source), m_first_row (row), m_last_row (row),
      m_column (all_columns), m_change (update)
{}

inline
IgTableModelEvent::IgTableModelEvent (IgTableModel *source, unsigned first_row,
				      unsigned last_row)
    : m_source (source), m_first_row (first_row), m_last_row (last_row),
      m_column (all_columns), m_change (update)
{}

inline
IgTableModelEvent::IgTableModelEvent (IgTableModel *source, unsigned first_row,
				      unsigned last_row, unsigned column)
    : m_source (source), m_first_row (first_row), m_last_row (last_row),
      m_column (column), m_change (update)
{}

inline
IgTableModelEvent::IgTableModelEvent (IgTableModel *source, unsigned first_row,
				      unsigned last_row, unsigned column,
				      ChangeType change)
    : m_source (source), m_first_row (first_row), m_last_row (last_row),
      m_column (column), m_change (change)
{}

inline IgTableModel *
IgTableModelEvent::source (void) const
{
    return m_source;
}

inline unsigned
IgTableModelEvent::first_row (void) const
{
    return m_first_row;
}

inline unsigned
IgTableModelEvent::last_row (void) const
{
    return m_last_row;
}

inline unsigned
IgTableModelEvent::column (void) const
{
    return m_column;
}

inline IgTableModelEvent::ChangeType
IgTableModelEvent::change (void) const
{
    return m_change;
}

#endif // IG_TABLE_MODEL_IG_TABLE_MODEL_EVENT_H
