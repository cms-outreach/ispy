#ifndef IGUANA_IG_COLLECTION_H
#define IGUANA_IG_COLLECTION_H

# ifdef PROJECT_NAME
#  include <Iguana/Framework/interface/IgLinearAlgebra.h>
# else
#  include <IgLinearAlgebra.h>
# endif
#include <cstring>
#include <string>
#include <cassert>
#include <vector>
#include <map>
#include <iostream>
#include <algorithm>

enum AssociatedType
{
  LEFT_ASSOCIATED = 1,
  RIGHT_ASSOCIATED = 2,
  BOTH_ASSOCIATED = 3
};

class IgRef
{
public:
  IgRef()
    {}

  IgRef(int collectionId, int objectId)
    : m_collectionId(collectionId), m_objectId(objectId)
    {}

  IgRef(const IgRef &ref)
    : m_collectionId(ref.collectionId()),
      m_objectId(ref.objectId())
    {}

  int collectionId(void) const
    {return m_collectionId;}

  int objectId(void) const
    {return m_objectId;}

  void set(int collectionId, int objectId)
    {
      m_collectionId = collectionId;
      m_objectId = objectId;
    }
private:
  int m_collectionId;
  int m_objectId;
};

class IgAssociation
{
public:
  IgAssociation(const IgRef &a, const IgRef &b)
    :m_refA(a), m_refB(b)
    {}

  IgAssociation()
    {}

  const IgRef &first(void) const
    {return m_refA; };

  const IgRef &second(void) const
    {return m_refB; };

  void set(int cA, int oA, int cB, int oB)
    {
      m_refA.set(cA, oA);
      m_refB.set(cB, oB);
    }
private:
  IgRef m_refA;
  IgRef m_refB;
};

enum ColumnType {
  INT_COLUMN = 0,           // int
  STRING_COLUMN,            // std::string
  DOUBLE_COLUMN,            // double
  VECTOR_2D,                // A 2vector of doubles
  VECTOR_3D,                // A 3vector of doubles
  VECTOR_4D,                // A 4vector of doubles
  NUMBER_OF_BASE_TYPES
};

// Helper class to generate associate a given type with its id.
template <class T>
struct ColumnTypeHelper
{
  static ColumnType typeId(void)
    {
      assert(false);
      return -1;
    }
};

template <>
struct ColumnTypeHelper<int>
{
  static ColumnType typeId(void)
    {
      return INT_COLUMN;
    }
};

template <>
struct ColumnTypeHelper<double>
{
  static ColumnType typeId(void)
    {
      return DOUBLE_COLUMN;
    }
};

template <>
struct ColumnTypeHelper<std::string>
{
  static ColumnType typeId(void)
    {
      return STRING_COLUMN;
    }
};

template <>
struct ColumnTypeHelper<IgV2d>
{
  static ColumnType typeId(void)
    { return VECTOR_2D; }
};

template <>
struct ColumnTypeHelper<IgV3d>
{
  static ColumnType typeId(void)
    { return VECTOR_3D; }
};

template <>
struct ColumnTypeHelper<IgV4d>
{
  static ColumnType typeId(void)
    { return VECTOR_4D; }
};

class IgColumnHandle
{
public:
  IgColumnHandle(void *data, ColumnType type)
    :m_data(data), m_type(type)
    { }

  ColumnType type(void)
    { return m_type; }

  void *data(void) const
    { return m_data; }

  template <class T>
  T &get(int index)
    {
      std::vector<T> &data = *(static_cast<std::vector<T> *>(m_data));
      return data[index];
    }

  int size(void)
    {
      switch (type())
      {
      case INT_COLUMN:
        return static_cast<std::vector<int> *>(m_data)->size();
        break;
      case STRING_COLUMN:
        return static_cast<std::vector<std::string> *>(m_data)->size();
        break;
      case DOUBLE_COLUMN:
        return static_cast<std::vector<double> *>(m_data)->size();
        break;
      case VECTOR_2D:
        return static_cast<std::vector<IgV2d> *>(m_data)->size();
        break;
      case VECTOR_3D:
        return static_cast<std::vector<IgV3d> *>(m_data)->size();
        break;
      case VECTOR_4D:
        return static_cast<std::vector<IgV4d> *>(m_data)->size();
        break;
      default:
        assert(false);
      }
    }

  void stream(std::ostream &stream, int position)
    {
      switch (type())
      {
      case INT_COLUMN:
        stream << get<int>(position);
        break;
      case STRING_COLUMN:
        stream << "\""<< get<std::string>(position) << "\"";
        break;
      case DOUBLE_COLUMN:
        stream << get<double>(position);
        break;
      case VECTOR_2D:
        {
          IgV2d &v = get<IgV2d>(position);
          stream << "("<< v.x() << ", " << v.y() << ")";
        }
        break;
      case VECTOR_3D:
        {
          IgV3d &v = get<IgV3d>(position);
          stream << "("<< v.x() << ", " << v.y() << ", " << v.z() << ")";
        }
        break;
      case VECTOR_4D:
        {
          IgV4d &v = get<IgV4d>(position);
          stream << "("<< v.x() << ", " << v.y() << ", " << v.z() << ", " << v.w() << ")";
        }
        break;
      default:
        assert(false);
      }
    }

  void streamType(std::ostream &stream)
    {
      switch (type())
      {
      case INT_COLUMN:
        stream << "int";
        break;
      case STRING_COLUMN:
        stream << "string";
        break;
      case DOUBLE_COLUMN:
        stream << "double";
        break;
      case VECTOR_2D:
        stream << "v2d";
        break;
      case VECTOR_3D:
        stream << "v3d";
        break;
      case VECTOR_4D:
        stream << "v4d";
        break;
      default:
        assert(false);
      }
    }

  void extend()
    {
      switch (m_type)
      {
      case INT_COLUMN:
        return doExtend<int>();
        break;
      case DOUBLE_COLUMN:
        return doExtend<double>();
        break;
      case STRING_COLUMN:
        return doExtend<std::string>();
        break;
      case VECTOR_2D:
        return doExtend<IgV2d>();
        break;
      case VECTOR_3D:
        return doExtend<IgV3d>();
        break;
      case VECTOR_4D:
        return doExtend<IgV4d>();
        break;
      default:
        assert(false);
        break;
      }
    }

  void resize(unsigned int newSize)
    {
      switch (m_type)
      {
      case INT_COLUMN:
        return doResize<int>(newSize);
        break;
      case DOUBLE_COLUMN:
        return doResize<double>(newSize);
        break;
      case STRING_COLUMN:
        return doResize<std::string>(newSize);
        break;
      case VECTOR_2D:
        return doResize<IgV2d>(newSize);
        break;
      case VECTOR_3D:
        return doResize<IgV3d>(newSize);
        break;
      case VECTOR_4D:
        return doResize<IgV4d>(newSize);
        break;
      default:
        assert(false);
        break;
      }
    }

  void reserve(unsigned int size)
    {
      switch (m_type)
      {
      case INT_COLUMN:
        doReserve<int>(size);
        break;
      case DOUBLE_COLUMN:
        doReserve<double>(size);
        break;
      case STRING_COLUMN:
        doReserve<std::string>(size);
        break;
      case VECTOR_2D:
        doReserve<IgV2d>(size);
        break;
      case VECTOR_3D:
        doReserve<IgV3d>(size);
        break;
      case VECTOR_4D:
        doReserve<IgV4d>(size);
        break;
      default:
        assert(false);
        break;
      }
    }

  void compress()
    {
      switch (m_type)
      {
      case INT_COLUMN:
        doCompress<int>();
        break;
      case DOUBLE_COLUMN:
        doCompress<double>();
        break;
      case STRING_COLUMN:
        doCompress<std::string>();
        break;
      case VECTOR_2D:
        doCompress<IgV2d>();
        break;
      case VECTOR_3D:
        doCompress<IgV3d>();
        break;
      case VECTOR_4D:
        doCompress<IgV4d>();
        break;
      default:
        assert(false);
        break;
      }
    }

  void clear()
    {
      switch (m_type)
      {
      case INT_COLUMN:
        doClear<int>();
        break;
      case DOUBLE_COLUMN:
        doClear<double>();
        break;
      case STRING_COLUMN:
        doClear<std::string>();
        break;
      case VECTOR_2D:
        doClear<IgV2d>();
        break;
      case VECTOR_3D:
        doClear<IgV3d>();
        break;
      case VECTOR_4D:
        doClear<IgV4d>();
        break;
      default:
        assert(false);
        break;
      }
    }

  template <class T>
  void doExtend()
    {
      std::vector<T> &data = *(static_cast<std::vector<T> *>(m_data));
      data.resize(data.size()+1);
    }

  template <class T>
  void doReserve(unsigned int size)
    {
      std::vector<T> &data = *(static_cast<std::vector<T> *>(m_data));
      data.reserve(size);
    }

  template <class T>
  void doCompress(void)
    {
      std::vector<T> &data = *(static_cast<std::vector<T> *>(m_data));
      data.reserve(data.size());
    }

  template <class T>
  void doClear(void)
    {
      std::vector<T> &data = *(static_cast<std::vector<T> *>(m_data));
      data.clear();
    }

  template <class T>
  void doResize(int newSize)
    {
      std::vector<T> &data = *(static_cast<std::vector<T> *>(m_data));
      data.resize(newSize);
    }

  void destroy()
    {
      switch (m_type)
      {
      case INT_COLUMN:
        doDestroy<int>();
        break;
      case DOUBLE_COLUMN:
        doDestroy<double>();
        break;
      case STRING_COLUMN:
        doDestroy<std::string>();
        break;
      case VECTOR_2D:
        doDestroy<IgV2d>();
        break;
      case VECTOR_3D:
        doDestroy<IgV3d>();
        break;
      case VECTOR_4D:
        doDestroy<IgV4d>();
        break;
      default:
        assert(false);
        break;
      }
    }


private:
  template <class T>
  void doDestroy(void)
    {
      std::vector<T> *data = static_cast<std::vector<T> *>(m_data);
      data->clear();
      delete data;
    }


  void *m_data;
  ColumnType m_type;
};

class IgProperty
{
public:
  IgProperty(IgColumnHandle &handle)
    : m_handle(handle)
    { }

  IgColumnHandle &handle(void)
    { return m_handle; }

private:
  IgColumnHandle m_handle;
};

class IgCollectionItem;
class IgCollection;

class IgCollectionIterator
{
public:
  IgCollectionIterator(IgCollection *collection, int rowPosition)
    :m_collection(collection), m_rowPosition(rowPosition)
    { }

  IgCollectionItem operator*();
  IgCollectionItem operator->(void);

  IgCollectionIterator operator++(int /*dummy*/)
    {
      IgCollectionIterator tmp(m_collection, m_rowPosition);
      m_rowPosition++;
      return tmp;
    }

  IgCollectionIterator& operator++(void)
    {
      m_rowPosition++;
      return *this;
    }

  IgCollectionIterator operator--(int /*dummy*/)
    {
      IgCollectionIterator tmp(m_collection, m_rowPosition);
      m_rowPosition--;
      return tmp;
    }

  IgCollectionIterator& operator--(void)
    {
      m_rowPosition--;
      return *this;
    }

  IgCollectionIterator& operator+=(int delta)
    {
      m_rowPosition += delta;
      return *this;
    }

  IgCollectionIterator& operator-=(int delta)
    {
      m_rowPosition -= delta;
      return *this;
    }

  IgCollectionIterator& operator=(int value)
    {
      m_rowPosition = value;
      return *this;
    }

  bool operator==(const IgCollectionIterator& other)
    {
      return m_rowPosition == other.m_rowPosition;
    }

  bool operator!=(const IgCollectionIterator& other)
    {
      return m_rowPosition != other.m_rowPosition;
    }

  IgCollectionIterator operator+(int value)
    {
      return IgCollectionIterator(m_collection, m_rowPosition + value);
    }

private:
  IgCollection *m_collection;
  int m_rowPosition;
};

/** Throw this error whenever the schema differs from what you
 *  were expecting.
 */
struct IgSchemaError
{
};

// TODO: for the time being the collection ID is defined by the creation order.
//       This is actually bad in the case we are merging files and should be
//       somehow replaced by some hash(maybe using some combination of
//       column name-column type for all the columns).
class IgCollection
{
public:
  typedef std::vector<std::string> Labels;
  typedef std::pair<const char *, IgColumnHandle> LabelColumn;
  typedef std::pair<ColumnType, IgColumnHandle> TypeColumn;
  typedef std::vector<IgProperty> Properties;

  template <class T>
  IgProperty &addProperty(const char *label, T /*defaultValue*/)
    {
      Labels::iterator l;
      if (doHasProperty(label, &l))
      {
        return m_properties[std::distance(m_labels.begin(), l)];
      }

      m_labels.push_back(label);
      std::vector<T> *columnData = new std::vector<T>;
      IgColumnHandle handle((void *)columnData, ColumnTypeHelper<T>::typeId());
      m_columnLabelsIndex.push_back(LabelColumn(m_labels.back().c_str(), handle));
      m_columnTypesIndex[ColumnTypeHelper<T>::typeId()].push_back(handle);
      m_properties.push_back(IgProperty(handle));
      return m_properties.back();
    }

  IgCollectionIterator begin(void);
  IgCollectionIterator end(void);

  IgProperty &getProperty(const char *label)
    {
      Labels::iterator l;
      if (!doHasProperty(label, &l))
      {
        throw IgSchemaError();
      }

      return m_properties[std::distance(m_labels.begin(), l)];
    }

  bool hasProperty(const char *label)
    {
      return doHasProperty(label);
    }

  bool hasProperty(const std::string &label)
    {
      return doHasProperty(label.c_str());
    }

  int size(void)
    {
      // We consider the size of the first column as the size of the whole
      // collection.
      return m_rowCount;
    }

  IgColumnHandle &getHandleByLabel(const char *label)
    {
      for (std::vector<LabelColumn>::iterator i = m_columnLabelsIndex.begin();
           i != m_columnLabelsIndex.end();
           i++)
      {
        if (strcmp(i->first, label) == 0)
        {
          return i->second;
        }
      }
      std::cout << "IgColumnHandle::getHandleByLabel " << label << " from " << m_name << std::endl;

      assert(false && "Column not found. Did you create it?");
    }

  IgColumnHandle &getHandleByPosition(const unsigned int position)
    {
      return m_properties[position].handle();
    }

  void reserve(unsigned int size)
    {
      for (Properties::iterator i = m_properties.begin();
           i != m_properties.end();
           i++)
      {
        i->handle().reserve(size);
      }
    }

  void resize(unsigned int size)
    {
      for (Properties::iterator i = m_properties.begin();
           i != m_properties.end();
           i++)
      {
        i->handle().resize(size);
      }
      m_rowCount = size;
    }

  void compress(void)
    {
      for (Properties::iterator i = m_properties.begin();
           i != m_properties.end();
           i++)
      {
        i->handle().compress();
      }
    }

  void clear(void)
    {
      for (Properties::iterator i = m_properties.begin();
           i != m_properties.end();
           i++)
      {
        i->handle().clear();
      }
    }

  Properties &properties(void)
    {
      return m_properties;
    }

  IgCollectionItem create();
  const char *name(void) const
    {
      return m_name.c_str();
    }

  int id(void) const
    {
      return m_id;
    }

  std::vector<LabelColumn> &columnLabels(void)
    { return m_columnLabelsIndex; };

protected:
  IgCollection(const char *name, int id)
    : m_name(name), m_id(id), m_rowCount(0)
    { }

  ~IgCollection()
    {
    }

  friend class IgDataStorage;

private:
  bool doHasProperty(const char *label, Labels::iterator *i = 0)
    {
      Labels::iterator l = std::find(m_labels.begin(), m_labels.end(), label);
      if (i != 0)
        *i = l;
      return l != m_labels.end();
    }

  typedef std::vector<LabelColumn> ColumnLabels;
  typedef std::vector<TypeColumn> ColumnTypes;
  typedef std::map<ColumnType, std::vector<IgColumnHandle> > ColumnTypeIndex;
  std::vector<std::string>  m_labels;
  std::vector<int>          m_primaryKeys;
  std::vector<LabelColumn>  m_columnLabelsIndex;
  Properties   m_properties;
  ColumnTypeIndex m_columnTypesIndex;
  std::string m_name;
  int m_id;
  int m_rowCount;
};


template <class T>
struct IgStorageGetterTrait
{ typedef T storage_type; };

template <>
struct IgStorageGetterTrait<char *>
{ typedef std::string storage_type; };

template <>
struct IgStorageGetterTrait<const char *>
{ typedef std::string storage_type; };

class IgCollectionItem
{
public:
  IgCollectionItem(IgCollection *collection, int position)
    :m_collection(collection), m_position(position), m_propertyPosition(0)
    {
    }

  IgCollectionItem *operator->(void)
    {
      return this;
    }

  int currentColumn()
    {
      return m_propertyPosition;
    }

  int currentRow()
    {
      return m_position;
    }

  IgCollectionItem &nextColumn(void)
    {
      m_propertyPosition++;
      return *this;
    }

  IgCollectionItem &moveToColumn(const IgColumnHandle &handle)
    {
      void *data = handle.data();
      int j = 0;
      for (IgCollection::Properties::iterator i = m_collection->properties().begin();
           (i != m_collection->properties().end()) &&(i->handle().data() != data);
           i++)
      { j++; }
      m_propertyPosition = j;
      return *this;
    }

  IgCollectionItem &operator*(void)
    {
      return *this;
    }

  template <class T>
  T &current(void)
    {
      return currentHandle().get<T>(m_position);
    }

  IgCollectionItem &operator[](IgProperty &property)
    {
      return this->moveToColumn(property.handle());
    }

  IgCollectionItem &operator[](const char *label)
    {
      assert(m_collection && "Collection ptr is 0");
      return this->moveToColumn(m_collection->getHandleByLabel(label));
    }

  template <class T>
  IgCollectionItem &operator=(T value)
    {
      currentHandle().get<typename IgStorageGetterTrait<T>::storage_type>(m_position) = value;
      return *this;
    }

  IgCollectionItem &operator=(const std::string &value)
    {
      currentHandle().get<std::string>(m_position) = value;
      return *this;
    }

  IgCollectionItem &operator=(const char *value)
    {
      currentHandle().get<std::string>(m_position) = value;
      return *this;
    }

  template <class T, class A, template <typename, typename> class C>
  IgCollectionItem &operator=(C<T,A> container)
    {
      ContainerTraits<C,T,A>::put(m_collection,
                                  m_propertyPosition,
                                  m_position,
                                  container);
      return *this;
    }

  template <class T>
  IgCollectionItem &operator,(T value)
    {
      m_propertyPosition++;
      currentHandle().get<T>(m_position) = value;
      return *this;
    }

  IgCollectionItem &operator,(const char *value)
    {
      m_propertyPosition++;
      currentHandle().get<std::string>(m_position) = value;
      return *this;
    }

  IgCollectionItem &operator,(const std::string &value)
    {
      m_propertyPosition++;
      currentHandle().get<std::string>(m_position) = value;
      return *this;
    }


  template <class T, class A, template <typename, typename> class C>
  IgCollectionItem &operator,(C<T, A> container)
    {
      m_propertyPosition=ContainerTraits<C,T,A>::put(m_collection,
                                                     m_propertyPosition,
                                                     m_position,
                                                     container);
      return *this;
    }

  template <class T>
  T &get(const char *label)
    {
      return m_collection->getHandleByLabel(label).get<T>(m_position);
    }

  template <class T>
  T &get(IgProperty &property)
    {
      return property.handle().get<T>(m_position);
    }

  template <class T>
  void set(IgProperty &property, T value)
    {
      property.handle().get<T>(m_position) = value;
    }

  template <class T>
  void set(const char *label, T value)
    {
      this->get<T>(label) = value;
    }

  void stream(std::ostream &stream)
    {
      for (IgCollection::Properties::iterator i = m_collection->properties().begin();
           i != m_collection->properties().end();
           i++)
      {
        IgColumnHandle &handle = i->handle();
        handle.stream(stream, m_position);
        if (i+1 != m_collection->properties().end())
        {
          stream << ", ";
        }
      }
    }

  operator IgRef() const
    { return IgRef(m_collection->id(), m_position); }

private:
  template <template <typename, typename> class C, class T, class A>
  struct ContainerTraits
  {
    static unsigned int put(IgCollection *collection,
                            unsigned int propertyPosition,
                            unsigned int position, C<T,A> container)
      {
        for (typename C<T,A>::iterator i = container.begin();
             i != container.end();
             i++)
        {
          collection->getHandleByPosition(propertyPosition)
            .get<T>(position) = *i;
          propertyPosition++;
        }
        return propertyPosition;
      }
  };

  IgColumnHandle &currentHandle()
    {
      return m_collection->getHandleByPosition(m_propertyPosition);
    }

  IgCollection *m_collection;
  unsigned int m_position;
  unsigned int m_propertyPosition;
};

class IgAssociationSet
{
public:
  typedef std::vector<IgAssociation> Associations;
  typedef Associations::iterator Iterator;

  IgAssociationSet(const char *name)
    :m_name(name)
    { }

  void associate(const IgRef &a, const IgRef &b)
    { m_associations.push_back(IgAssociation(a, b)); }

  const char *name(void)
    {return m_name.c_str(); }

  Iterator begin(void)
    { return m_associations.begin(); }

  Iterator end(void)
    { return m_associations.end(); }

  int size(void)
    { return m_associations.size(); }

  void clear(void)
    { m_associations.clear(); }

  void reserve(unsigned int capacity)
    { m_associations.reserve(capacity);}

  void resize(unsigned int newSize)
    { m_associations.resize(newSize);}

  void compress(void)
    { m_associations.reserve(m_associations.size());}

  IgAssociation &operator[](unsigned int pos)
    {
      return m_associations[pos];
    }
private:
  std::string m_name;
  Associations m_associations;
};

class IgAssociatedSet;

class IgDataStorage
{
public:
  // FIXME: quick and dirty...
  typedef std::vector<std::string> CollectionNames;
  typedef std::vector<std::string> AssociationSetNames;
  typedef std::vector<IgCollection *> Collections;
  typedef std::vector<IgAssociationSet *> AssociationSets;

  // Data storage is responsible for destroying all its collections
  // and their contents.
  ~IgDataStorage(void)
    {
      for (AssociationSets::iterator i = m_associationSets.begin();
           i != m_associationSets.end();
           i++)
      {
        delete *i;
      }

      for (Collections::iterator i = m_collections.begin();
           i != m_collections.end();
           i++)
      {
        for (IgCollection::Properties::iterator j = (*i)->properties().begin();
             j !=(*i)->properties().end();
             j++)
        {
          j->handle().destroy();
        }
        delete *i;
      }
    }

  // FIXME: for the time being a little bit of duplication does not harm.
  //        Maybe in future getCollection and getAssociationSet could be
  //        put in a templated get<T> function. This needs some template magic
  //        which I rather avoid for the time being.
  IgCollection &getCollection(const char *label)
    {
      return *getCollectionPtr(label);
    }

  IgAssociatedSet getAssociatedSetPtr(IgAssociationSet *associations,
                                      IgCollectionItem &item,
                                      AssociatedType which = BOTH_ASSOCIATED);

  IgAssociatedSet getAssociatedSet(const char *name,
                                   IgCollectionItem &item,
                                   AssociatedType which = BOTH_ASSOCIATED);

  IgCollection &getCollectionByIndex(unsigned int index)
    {
      assert(index < m_collections.size());
      return *(m_collections[index]);
    }

  IgCollection *getCollectionPtr(size_t indexInNameList)
    {
      return m_collections[indexInNameList];
    }

  IgCollection *getCollectionPtr(const std::string &label)
    {
      return getCollectionPtr(label.c_str());
    }

  IgCollection *getCollectionPtr(const char *label)
    {

      CollectionNames::iterator n = std::find(m_collectionNames.begin(),
                                              m_collectionNames.end(),
                                              label);

      if (n == m_collectionNames.end())
      {
        IgCollection *collection = new IgCollection(label, m_collections.size());
        m_collectionNames.push_back(label);
        m_collections.push_back(collection);
        return collection;
      }
      return m_collections[std::distance(m_collectionNames.begin(), n)];
    }

  IgAssociationSet &getAssociationSet(const char *label)
    {
      return *getAssociationSetPtr(label);
    }

  IgAssociationSet *getAssociationSetPtr(size_t indexInASetNames)
    {
      return m_associationSets[indexInASetNames];
    }

  IgAssociationSet *getAssociationSetPtr(const std::string &label)
    {
      return getAssociationSetPtr(label.c_str());
    }

  IgAssociationSet *getAssociationSetPtr(const char * label)
    {
      AssociationSetNames::iterator n = std::find(m_associationSetNames.begin(),
                                                  m_associationSetNames.end(),
                                                  label);
      if (n == m_associationSetNames.end())
      {
        IgAssociationSet *associationSet = new IgAssociationSet(label);
        m_associationSetNames.push_back(label);
        m_associationSets.push_back(associationSet);
        return associationSet;
      }
      return m_associationSets[std::distance(m_associationSetNames.begin(), n)];
    }

  IgCollectionItem deref(const IgRef &ref)
    {
      return IgCollectionItem(m_collections[ref.collectionId()],
                              ref.objectId());
    }

  CollectionNames &collectionNames(void)
    {
      return m_collectionNames;
    }

  AssociationSetNames &associationSetNames(void)
    {
      return m_associationSetNames;
    }

  bool empty(void)
    {
      return m_collectionNames.empty();
    }

  bool hasCollection(const char *name)
    {
      return std::find(m_collectionNames.begin(), m_collectionNames.end(), name) != m_collectionNames.end();
    }
private:
  CollectionNames m_collectionNames;
  AssociationSetNames m_associationSetNames;
  Collections m_collections;
  AssociationSets m_associationSets;
};

class IgAssociatedSet
{
public:
  class Iterator
  {
  public:
    // FIXME: IgCollectionItem should have an objectId method.
    Iterator(IgDataStorage *storage, IgAssociationSet::Iterator current,
             IgAssociationSet::Iterator end,
             const IgRef &item,
             enum AssociatedType associatedType = BOTH_ASSOCIATED)
      :m_storage(storage),
       m_cur(current),
       m_end(end),
       m_ref(item),
       m_associatedType(associatedType)
      {
        find();
      }

    void operator++(int /*dummy*/)
      {
        m_cur++;
        find();
      }

    void find(void)
      {
        while(m_cur != m_end)
        {
          if ((m_associatedType & RIGHT_ASSOCIATED)
              && m_cur->first().objectId() == m_ref.objectId()
              && m_cur->first().collectionId() == m_ref.collectionId())
          {
            m_pos = 0;
            return;
          }
          if ((m_associatedType & LEFT_ASSOCIATED)
              && m_cur->second().objectId() == m_ref.objectId()
              && m_cur->second().collectionId() == m_ref.collectionId())
          {
            m_pos = 1;
            return;
          }
          m_cur++;
        }
      }

    bool operator==(const Iterator &other)
      {
        return other.m_cur == this->m_cur;
      }

    bool operator!=(const Iterator &other)
      {
        return other.m_cur != this->m_cur;
      }

    IgCollectionItem operator*(void)
      {
        return m_storage->deref(m_pos ? m_cur->first() : m_cur->second());
      }

  private:
    IgDataStorage *m_storage;
    IgAssociationSet::Iterator m_cur;
    IgAssociationSet::Iterator m_end;
    const IgRef m_ref;
    int   m_pos;
    enum AssociatedType m_associatedType;
  };

  IgAssociatedSet(IgDataStorage *storage,
                  IgAssociationSet *associations,
                  IgCollectionItem *item,
                  enum AssociatedType associatedType = BOTH_ASSOCIATED)
    : m_storage(storage),
      m_associations(associations),
      m_item(item),
      m_associatedType(associatedType)
    { }

  Iterator begin(void)
    {
      Iterator result(m_storage,
                      m_associations->begin(),
                      m_associations->end(),
                      static_cast<IgRef>(*m_item), m_associatedType);
      return result;
    }

  Iterator end(void)
    { return Iterator(m_storage, m_associations->end(),
                      m_associations->end(),
                      static_cast<const IgRef>(*m_item), m_associatedType); }
private:
  IgDataStorage *m_storage;
  IgAssociationSet *m_associations;
  IgCollectionItem *m_item;
  enum AssociatedType m_associatedType;
};


std::ostream &operator<<(std::ostream &stream, IgCollection &collection);
std::ostream &operator<<(std::ostream &stream, const IgRef &ref);
std::ostream &operator<<(std::ostream &stream, const IgAssociation &association);
std::ostream &operator<<(std::ostream &stream, IgAssociationSet &associationSet);
std::ostream &operator<<(std::ostream &stream, IgDataStorage &storage);

inline IgCollectionItem
IgCollectionIterator::operator->(void)
{
  return **this;
}

#endif /* IGUANA_IG_COLLECTION_H */
