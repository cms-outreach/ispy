#ifndef ISPY_DRAW_HELPERS_H
#define ISPY_DRAW_HELPERS_H

#include "QtGUI/Projectors.h"
#include <map>
#include <string>

class IgCollection;
class IgAssociations;
class SoSeparator;
class Style;

/** Typedef defining the generic function that goes from one (or more)
    collection(s), possibly linked together via one or more @a associations
    to a OpenInventor representation which must be attached to @a group.
    
    Such an helper also gets a list of projectors to be used when creating the 
    representation.
 */
typedef void(*Make3D)(IgCollection **collections, IgAssociations **associations, 
                      SoSeparator *group, Style *style, Projectors &projectors);

/** Call this function to get a list of all the available helpers saved in 
    @a helpers.
  */ 
void registerDrawHelpers(std::map<std::string, Make3D> &helpers);

void initHelpers(void);

#endif
