// Supported types
#ifdef PROJECT_NAME
# include <Iguana/Framework/interface/IgCollection.h>
#else
# include <IgCollection.h>
#endif

#include <string>
#include <cassert>
#include <vector>
#include <map>
#include <iostream>

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

IgCollectionItem IgCollectionIterator::operator*()
{
  return IgCollectionItem(m_collection, m_rowPosition);
}

IgCollectionIterator IgCollection::begin(void)
{
  return IgCollectionIterator(this, 0);
}

IgCollectionIterator IgCollection::end(void)
{
  return IgCollectionIterator(this, m_rowCount);
}

std::ostream &operator<<(std::ostream &stream, IgCollection &collection)
{
  stream << "\""<< collection.name() << "\": [";

  for(IgCollectionIterator i = collection.begin();
      i != collection.end();
      i++)
  {
    stream << "[";
    (*i).stream(stream);
    stream << "]";
    if(i + 1 != collection.end())
    {stream << ", "; }
    stream << "\n";
  }

  stream << "]\n";
  return stream;
}

/** @return the associated set for the @a item. I.e. all the associations
    that have @a item as part of the association. The @a type of association
    determines whether @a item can be on both side of the association or only
    in one of the two.
    
    @a item the item to return the associated set of.
    
    @a type which indicates whether the associated objectes can be
     on the right, the left, or any of the side of the association.
*/
IgAssociatedSet
IgAssociationSet::getAssociatedSet(IgCollectionItem &item,
                                   enum AssociatedType type)
{
  return IgAssociatedSet(m_storage, this, &item, type);
}

std::ostream &operator<<(std::ostream &stream, const IgRef &ref)
{
  stream << "[" << ref.collectionId() << ", " << ref.objectId() << "]";
  return stream;
}

std::ostream &operator<<(std::ostream &stream, const IgAssociation &association)
{
  stream << "[" << association.first() << ", " << association.second() << "]";
  return stream;
}

std::ostream &operator<<(std::ostream &stream, IgAssociationSet &associationSet)
{
  stream << "\""<< associationSet.name() << "\": [";

  for(IgAssociationSet::Iterator i = associationSet.begin();
      i != associationSet.end();
      i++)
  {
    stream << *i;
    if(i + 1 != associationSet.end())
    {stream << ", "; }
    stream << "\n";
  }

  stream << "]\n";
  return stream;
}

std::ostream &operator<<(std::ostream &stream, IgDataStorage &storage)
{
  stream << "{"
         << "'Types': {";
  for (IgDataStorage::CollectionNames::iterator i = storage.collectionNames().begin();
       i != storage.collectionNames().end();
       i++)
  {
    stream << "\"" << *i << "\": [";
    IgCollection &collection = storage.getCollection(i->c_str());
    for(std::vector<IgCollection::LabelColumn>::iterator j = collection.columnLabels().begin();
        j != collection.columnLabels().end();
        j++)
    {
      stream << "[\"" << j->first << "\", ";
      stream << "\"";
      j->second.streamType(stream);
      stream << "\"]";
      if (j+1 != collection.columnLabels().end())
        stream << ",";
    }
    stream << "]\n";
    if(i+1 != storage.collectionNames().end())
      stream << ",\n";
  }
  stream << "},\n";
  stream << "'Collections': {";
  for (IgDataStorage::CollectionNames::iterator i = storage.collectionNames().begin();
       i != storage.collectionNames().end();
       i++)
  {
    stream << storage.getCollection(i->c_str());
    if(i+1 != storage.collectionNames().end())
    {
      stream << ",\n";
    }
  }
  stream << "},\n";
  stream << "'Associations': {";
  for (IgDataStorage::CollectionNames::iterator i = storage.associationSetNames().begin();
       i != storage.associationSetNames().end();
       i++)
  {
    stream << storage.getAssociationSet(i->c_str());
    if(i+1 != storage.associationSetNames().end())
    {
      stream << ",\n";
    }
  }
  stream << "}\n}";
  return stream;
}

/** Creates the IgProperty with the column
    handle named @a name of the collection @a collection
  */
IgProperty::IgProperty(IgCollection *collection, const char *name)
:m_handle(collection->getProperty(name).handle())
{
}
