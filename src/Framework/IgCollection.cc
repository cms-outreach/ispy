// Supported types

#ifdef PROJECT_NAME
 #include <ISpy/Services/interface/IgCollection.h>
#else
 #include <Framework/IgCollection.h>
#endif

#include <string>
#include <cassert>
#include <vector>
#include <map>
#include <iostream>
#include <iterator>

IgCollectionItem IgCollection::create(void)
{
  assert(!m_properties.empty());
  for (std::vector<IgProperty>::iterator i = m_properties.begin();
       i != m_properties.end();
       i++)
  {
    i->handle().extend();
  }
  return IgCollectionItem(this, m_rowCount++);
}

IgCollectionItem IgCollection::iterator::operator*() const
{
  return IgCollectionItem(m_collection, m_rowPosition);
}

IgCollectionItem IgCollection::operator[](size_t pos)
{
  return IgCollectionItem(this, pos);
}

std::ostream &operator<<(std::ostream &stream, IgCollection &collection)
{
  stream << "\""<< collection.name() << "\": [";

  for(size_t i = 0, e = collection.size(); i != e; ++i)
  {
    stream << "[";
    collection[i].stream(stream);
    stream << "]";
    if (i + 1 != e)
      stream << ", ";
    stream << "\n";
  }

  stream << "]\n";
  return stream;
}

std::ostream &operator<<(std::ostream &stream, IgDataStorage &storage)
{
  // Stream the types.
  stream << "{"
         << "\"Types\": {";
  for (size_t cni = 0, cne = storage.collectionNames().size(); cni != cne; ++cni)
  {
    const char *collectionName = storage.collectionNames()[cni].c_str();
    IgCollection &collection = storage.getCollection(collectionName);
    
    stream << "\"" << collectionName << "\": [";
    for(size_t cti = 0, cte = collection.columnLabels().size(); cti != cte; ++cti)
    {
      ColumnInfo &info = collection.columnLabels()[cti];
      stream << "[\"" << info.label << "\", \"" << info.handle.typeName() << "\"]";
      if (cti + 1 != cte)
        stream << ",";
    }
    stream << "]\n";
    if(cni + 1 != cne)
      stream << ",\n";
  }
  stream << "},\n";

  // Stream the collection contents.
  stream << "\"Collections\": {";
  for (size_t cni = 0, cne = storage.collectionNames().size(); cni != cne; ++cni)
  {
    const char *collectionName = storage.collectionNames()[cni].c_str();
    stream << storage.getCollection(collectionName);
    if (cni + 1 != cne)
      stream << ",\n";
  }
  stream << "},\n";
  
  // Stream the associations.
  stream << "\"Associations\": {";
  for (size_t ai = 0, ae = storage.associationsNames().size(); ai != ae; ++ai)
  {
    const char *associationsName = storage.associationsNames()[ai].c_str();
    IgAssociations &assoc = storage.getAssociations(associationsName);
    stream << "\"" << associationsName << "\": [";
    assoc.stream(stream);
    stream << "]";
    if (ai + 1 != ae)
      stream << ",\n";
  }
  stream << "}\n}";
  return stream;
}

/** Creates the IgProperty with the column
    handle named @a name of the collection @a collection
  */
IgProperty::IgProperty(IgCollection *collection, const char *name)
:m_handle(collection->getProperty(name).handle())
{}

IgProperty::IgProperty(IgCollection &collection, const char *name)
:m_handle(collection.getProperty(name).handle())
{}

/** Associate two references @a a and @a b between themselves,
    effectively creating an association between the two.
    
    Given that we are most likely inserting associations so that reference
    @a and @b are ordered we exploit this fact to make sure that their store
    is maintained that way.

    @a a a reference to an object in one of the collections in the IgDataStorage.
    
    @a b a reference to an object in one of the collections in the IgDataStorage.
 */
void
IgAssociations::associate(const IgRef &a, const IgRef &b)
{
  // Hopefully the upper_bound will always be the vector tail, so that
  // we can insert at full speed.
  AssociationRef cur(a, b);
  Associations::iterator insertionPoint = std::upper_bound(m_associations.begin(),
                                                           m_associations.end(),
                                                           cur);
  m_associations.insert(insertionPoint, cur);
}
