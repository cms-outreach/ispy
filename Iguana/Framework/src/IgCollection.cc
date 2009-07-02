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

IgAssociatedSet
IgDataStorage::getAssociatedSetPtr(IgAssociationSet *associations,
                                   IgCollectionItem &item,
                                   enum AssociatedType type)
{
  return IgAssociatedSet(this, associations, &item, type);
}

IgAssociatedSet
IgDataStorage::getAssociatedSet(const char *name,
                                IgCollectionItem &item,
                                enum AssociatedType type)
{
  return this->getAssociatedSetPtr(this->getAssociationSetPtr(name), item, type);
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
