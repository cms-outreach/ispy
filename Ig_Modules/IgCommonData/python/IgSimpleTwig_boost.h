#ifndef IG_COMMON_DATA_IG_SIMPLE_TWIG_BOOST_H
# define IG_COMMON_DATA_IG_SIMPLE_TWIG_BOOST_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgCommonData/interface/config.h"
# include <boost/python.hpp>
# include <boost/cstdint.hpp>
# include <Ig_Framework/IgObjectBrowser/interface/IgRepresentable.h>
# include <Ig_Modules/IgCommonData/interface/IgTwig.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>


using namespace boost::python;

// Declarations ================================================================
struct IgSimpleTwig_Wrapper: IgSimpleTwig
{
    IgSimpleTwig_Wrapper(PyObject* self_, IgTwig *p0);
    IgSimpleTwig_Wrapper(PyObject* self_, IgTwig *p0, const std::string& p1);
    IgSimpleTwig_Wrapper(PyObject* self_, IgTwig *p0, const std::string& p1, bool p2);
    IgSimpleTwig_Wrapper(PyObject* self_, IgTwig *p0, const std::string& p1, bool p2, bool p3);
    IgSimpleTwig_Wrapper(PyObject* self_, IgTwig *p0, const std::string& p1, bool p2, bool p3, bool p4);
    IgSimpleTwig_Wrapper(PyObject* self_, const std::string& p0);
    IgSimpleTwig_Wrapper(PyObject* self_, const std::string& p0, bool p1);
    IgSimpleTwig_Wrapper(PyObject* self_, const std::string& p0, bool p1, bool p2);
    IgSimpleTwig_Wrapper(PyObject* self_, const std::string& p0, bool p1, bool p2, bool p3);
    std::string name() const ;
    std::string default_name() const ;
    void name(const std::string& p0) ;
    void default_name(const std::string& p0) ;
    bool selfTraverse() const ;
    bool default_selfTraverse() const ;
    void selfTraverse(bool p0) ;
    void default_selfTraverse(bool p0) ;
    bool selfVisible() const ;
    bool default_selfVisible() const ;
    void selfVisible(bool p0) ;
    void default_selfVisible(bool p0) ;
    bool cascade() const ;
    bool default_cascade() const ;
    void cascade(bool p0) ;
    void default_cascade(bool p0) ;
    unsigned int children() ;
    unsigned int default_children() ;
    unsigned int indexOf(const IgTwig* p0) const ;
    unsigned int default_indexOf(const IgTwig* p0) const ;
    bool expand() ;
    bool default_expand() ;
    void add(IgTwig* p0) ;
    void default_add(IgTwig* p0) ;
    void remove(IgTwig* p0) ;
    void default_remove(IgTwig* p0) ;
    void destroy() ;
    void default_destroy() ;
    void clear() ;
    void default_clear() ;
    std::string fullName() ;
    std::string default_fullName() ;
    bool traverse() const ;
    bool default_traverse() const ;
    bool visible() const ;
    bool default_visible() const ;
    void attributes(std::map<std::string, std::pair<lat::Callback1<IgDatum*&>, lat::Callback1<const IgDatum*> > >& p0) ;
    void default_attributes(std::map<std::string, std::pair<lat::Callback1<IgDatum*&>, lat::Callback1<const IgDatum*> > >& p0) ;
    PyObject* self;
};


//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_COMMON_DATA_IG_SIMPLE_TWIG_BOOST_H
