#ifndef IGUANA_FRAMEWORK_IG_PLUGIN_DEF_H
# define IGUANA_FRAMEWORK_IG_PLUGIN_DEF_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/config.h"
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgModuleDef;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Helper to simplify registration in #IgModuleDef implementations. */
template <class Factory, class Type>
struct IgPluginDef
{
    static void declare (IgModuleDef *def);
    static void declareName (IgModuleDef *def, std::string name);
    static void installFactory (IgModuleDef *def);
    static void installFactoryName (IgModuleDef *def, std::string name);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

/** Add @c Type as @c Type::catalogLabel() for module @a def.  Call
    this method in the module definition's @c query() method.  */
template <class Factory, class Type>
inline void
IgPluginDef<Factory,Type>::declare (IgModuleDef *def)
{ Factory::get ()->declare (def, Type::catalogLabel ()); }

/** Add @c Type as @a name for module @a def.  Call this method
    in the module definition's @c query() method.  */
template <class Factory, class Type>
inline void
IgPluginDef<Factory,Type>::declareName (IgModuleDef *def, std::string name)
{ Factory::get ()->declare (def, name); }

/** Add factory for @c Type, previously registered as @c
    Type::catalogLabel() for module @a def.  Call this method in
    the module definition's @c attach() method.  */
template <class Factory, class Type>
inline void
IgPluginDef<Factory,Type>::installFactory (IgModuleDef *def)
{
    typedef typename Factory::Types::template AutoFactory<Type> AutoFactory;
    Factory::get ()->installFactory (def, Type::catalogLabel (), new AutoFactory);
}

/** Add factory for @c Type, previously registered as @a name for
    module @a def.  Call this method in the module definition's
    @c attach() method.  */
template <class Factory, class Type>
inline void
IgPluginDef<Factory,Type>::installFactoryName (IgModuleDef *def, std::string name)
{
    typedef typename Factory::Types::template AutoFactory<Type> AutoFactory;
    Factory::get ()->installFactory (def, name, new AutoFactory);
}

#endif // IGUANA_FRAMEWORK_IG_PLUGIN_DEF_H
