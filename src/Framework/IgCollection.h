#ifndef IGUANA_IG_COLLECTION_H
#define IGUANA_IG_COLLECTION_H

#ifdef PROJECT_NAME
# include <ISpy/Services/interface/IgLinearAlgebra.h>
#else
# include <Framework/IgLinearAlgebra.h>
#endif
#include <cstring>
#include <string>
#include <cassert>
#include <vector>
#include <map>
#include <iostream>
#include <algorithm>

/** A reference to a particular row in a particular collection */
struct IgRef
{
  /** Builds a default IgRef. The reason why we use MAX_SIZE as index is 
      because this way we can make sure that when an Association, is created
      using one default IgRef, such an Association ends up after all the
      other Associations if the same kind.
  */
  IgRef()
    :collectionId((size_t) -1), objectId((size_t)-1)
    {}

  IgRef(size_t iCollectionId, size_t iObjectId)
    : collectionId(iCollectionId), objectId(iObjectId)
    {}

  IgRef(const IgRef &ref)
    :collectionId(ref.collectionId), objectId(ref.objectId)
    {}

  /** Order for IgRefs, first by collection, then by object
      This is the natural order in which objects are inserted in associations.
  */
  bool operator<(const IgRef& other) const
    {
      if (collectionId == other.collectionId)
        return objectId < other.objectId;
      return collectionId < other.collectionId;
    }

  /** Equality of two refs is really equality of its members.
  */
  bool operator==(const IgRef& other) const
    {
      if (objectId != other.objectId)
        return false;
      if (collectionId != other.collectionId)
        return false;
      return true;
    }

  size_t collectionId;
  size_t objectId;
};


enum ColumnType {
  INT_COLUMN = 0,           // int
  STRING_COLUMN,            // std::string
  DOUBLE_COLUMN,            // double
  VECTOR_2D,                // A 2vector of doubles
  VECTOR_3D,                // A 3vector of doubles
  VECTOR_4D,                // A 4vector of doubles
  NUMBER_OF_BASE_TYPES,
  INVALID
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
  IgColumnHandle(void)
    :m_data(0), m_type(INVALID)
    {}
  
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

  void stream(std::ostream &stream, size_t position)
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
          stream << "["<< v.x() << ", " << v.y() << "]";
        }
        break;
      case VECTOR_3D:
        {
          IgV3d &v = get<IgV3d>(position);
          stream << "["<< v.x() << ", " << v.y() << ", " << v.z() << "]";
        }
        break;
      case VECTOR_4D:
        {
          IgV4d &v = get<IgV4d>(position);
          stream << "["<< v.x() << ", " << v.y() << ", " << v.z() << ", " << v.w() << "]";
        }
        break;
      default:
        assert(false);
      }
    }
    
  const char *typeName()
    {
      static const char *typenames[] = {
        "int",
        "string",
        "double",
        "v2d",
        "v3d",
        "v4d"};
      return typenames[type()];
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

  void resize(size_t newSize)
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

  void reserve(size_t size)
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


class IgCollection;

/** An IgProperty is used to index 
    a given column inside an IgCollection.
  
    FIXME: is this really a good idea?
          Why not using IgColumnHandle directly???
  */
class IgProperty
{
public:
  IgProperty(IgCollection *collection, const char *name);

  IgProperty(IgCollection &collection, const char *name);

  IgProperty(const IgProperty &property)
    :m_handle(property.m_handle)
  {
  }

  IgProperty(IgColumnHandle &handle)
    : m_handle(handle)
    { }

  IgColumnHandle &handle(void)
    { return m_handle; }

private:
  IgColumnHandle m_handle;
};

template <class T>
class IgColumn : public IgProperty
{
public:
  IgColumn(IgCollection *collection, const char *name)
  : IgProperty(collection, name)
  {}
  
  typedef T Type;
  typedef T Original;
};

class IgCollectionItem;

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

struct ColumnInfo
{
  std::string     label;
  IgColumnHandle  handle;
};

class IgCollection
{
public:
  class iterator
  {
  public:
    iterator(IgCollection *collection, size_t rowPosition)
      :m_collection(collection), m_rowPosition(rowPosition)
      { }

    IgCollectionItem operator*() const;
    IgCollectionItem operator->(void);

    size_t pos(void)
    {
      return m_rowPosition;
    }
    
    iterator operator++(int /*dummy*/)
      {
        iterator tmp(m_collection, m_rowPosition);
        m_rowPosition++;
        return tmp;
      }

    iterator& operator++(void)
      {
        m_rowPosition++;
        return *this;
      }

    iterator operator--(int /*dummy*/)
      {
        iterator tmp(m_collection, m_rowPosition);
        m_rowPosition--;
        return tmp;
      }

    iterator& operator--(void)
      {
        m_rowPosition--;
        return *this;
      }

    iterator& operator+=(int delta)
      {
        m_rowPosition += delta;
        return *this;
      }

    iterator& operator-=(int delta)
      {
        m_rowPosition -= delta;
        return *this;
      }

    iterator& operator=(int value)
      {
        m_rowPosition = value;
        return *this;
      }

    bool operator==(const iterator& other)
      {
        return m_rowPosition == other.m_rowPosition;
      }

    bool operator!=(const iterator& other)
      {
        return m_rowPosition != other.m_rowPosition;
      }

    iterator operator+(int value)
      {
        return iterator(m_collection, m_rowPosition + value);
      }

  private:
    IgCollection *m_collection;
    size_t m_rowPosition;
  };

  typedef std::vector<std::string> Labels;
  typedef std::pair<const char *, IgColumnHandle> LabelColumn;
  typedef std::pair<ColumnType, IgColumnHandle> TypeColumn;
  typedef std::vector<IgProperty> Properties;

  template <class T>
  IgProperty &addProperty(const char *label, T /*defaultValue*/)
    {
      Labels::iterator l;
      if (doHasProperty(label, &l))
        return m_properties[std::distance(m_labels.begin(), l)];

      m_labels.push_back(label);
      std::vector<T> *columnData = new std::vector<T>;
      IgColumnHandle handle((void *)columnData, ColumnTypeHelper<T>::typeId());
      m_columnLabelsIndex.resize(m_columnLabelsIndex.size() + 1);
      ColumnInfo &info = m_columnLabelsIndex.back();
      info.label = m_labels.back();
      info.handle = handle;
      m_columnTypesIndex[ColumnTypeHelper<T>::typeId()].push_back(handle);
      m_properties.push_back(IgProperty(handle));
      return m_properties.back();
    }

  iterator begin(void)    
    {
      return iterator(this, 0);
    }
  
  iterator end(void)
    {
      return iterator(this, m_rowCount);
    }

  IgProperty &getProperty(const char *label)
    {
      Labels::iterator l;
      if (!doHasProperty(label, &l))
        throw IgSchemaError();

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
      for (size_t i = 0, e = m_columnLabelsIndex.size(); i != e; ++i)
        if (m_columnLabelsIndex[i].label == label)
          return m_columnLabelsIndex[i].handle;

      std::cout << "IgColumnHandle::getHandleByLabel " << label << " from " << m_name << std::endl;

      assert(false && "Column not found. Did you create it?");
    }

  IgColumnHandle &getHandleByPosition(const unsigned int position)
    {
      return m_properties[position].handle();
    }

  void reserve(size_t size)
    {
      for (size_t i = 0, e = m_properties.size(); i != e; ++i)
        m_properties[i].handle().reserve(size);
    }

  void resize(unsigned int size)
    {
      for (size_t i = 0, e = m_properties.size(); i != e; ++i)
        m_properties[i].handle().resize(size);
      m_rowCount = size;
    }

  void compress(void)
    {
      for (size_t i = 0, e = m_properties.size(); i != e; ++i)
        m_properties[i].handle().compress();
    }

  void clear(void)
    {
      for (size_t i = 0, e = m_properties.size(); i != e; ++i)
        m_properties[i].handle().clear();
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

  std::vector<ColumnInfo> &columnLabels(void)
    { return m_columnLabelsIndex; };

  IgCollectionItem operator[](size_t i);
  
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

  typedef std::vector<ColumnInfo> ColumnLabels;
  typedef std::vector<TypeColumn> ColumnTypes;
  typedef std::map<ColumnType, std::vector<IgColumnHandle> > ColumnTypeIndex;
  std::vector<std::string>  m_labels;
  std::vector<int>          m_primaryKeys;
  ColumnLabels              m_columnLabelsIndex;
  Properties                m_properties;
  ColumnTypeIndex           m_columnTypesIndex;
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


  /** This template can be overwritten to transform IgV3d to foreign types,
      like those used by eigen or even Coin.
    */
  template <class T>
  typename IgColumn<T>::Type get(IgColumn<T> &column)
    {
      return column.handle().get<typename IgColumn<T>::Original>(m_position);
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
      for (size_t i = 0, e = m_collection->properties().size(); i != e; ++i)
      {
        IgColumnHandle &handle = m_collection->properties()[i].handle();
        handle.stream(stream, m_position);
        if (i + 1 != e)
          stream << ", ";
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

class IgDataStorage;

class IgAssociations
{
  struct AssociationRef
  {
    /** Constructor an association between two references. 
        Notice that if the @a iB ref is not given, such an association
        will always be stored after all the Associations sharing the same
        iA.

        Also notice that iA is effectively used as a key of the association,
        while iB is the actual retrievable. I.e. if you do:
        
            associations.associate(a, b);
        
        If you do:
            
            associations.begin(a);
        
        you will get an iterator pointing to b, while if you do:
        
            associations.begin(b);
        
        you will *NOT* get an iterator pointing to a. If you want such a 
        behavior you need to do:
        
            associations.associate(a, b);
            associations.associate(b, a);
        
        @a iKey an IgRef to the object to associate which acts as a key.
        
        @a iValue an IgRef to the object that acts as a value.
    */
    AssociationRef(const IgRef &iKey = IgRef(), const IgRef &iValue = IgRef())
    :key(iKey), value(iValue)
    {}
    
    /** Order for Association, first by key ref, then by value ref,
        This is the natural order in which objects are inserted in
        associations.
    */
    bool operator<(const AssociationRef& other) const
      {
        if (key == other.key)
          return value < other.value;
        return key < other.key;
      }

    IgRef key;
    IgRef value;
  };

  typedef std::vector<AssociationRef> Associations;

public:
  class iterator
  {
  public:
    /** An end iterator. Both m_i and m_end will be initialised to 
        end(). */
    iterator(void)
      :m_isOutOfRange(true)
    {
    }
    /** Constructs an iterator over a given set of associations. 
      
        @a first is the first element in the range.
        
        @a last is the last element in the range.
        
        @a storage is the storage of the collections being associated,
                   so that we can retrieve objects directly. 
     */
    iterator(const Associations::iterator &first,
             const Associations::iterator &last,
             IgDataStorage *storage)
    :m_i(first),
     m_end(last),
     m_isOutOfRange(first == last),
     m_storage(storage)
    {
    }
    
    IgCollectionItem operator*();

    IgCollectionItem operator->(void)
    {
      return **this;
    }

    iterator operator++(int /*dummy*/)
    {
      IgAssociations::iterator tmp(m_i, m_end, m_storage);
      ++(*this);
      return tmp;
    }
    
    iterator &operator++(void)
    {
      m_i++;
      if (m_i == m_end)
        m_isOutOfRange = true;
      return *this;
    }

    iterator operator--(int /*dummy*/)
    {
      IgAssociations::iterator tmp(m_i, m_end, m_storage);
      --(*this);
      return tmp;
    }
    
    iterator &operator--(void)
    {
      m_i--;
      return *this;
    }

    iterator& operator+=(int delta)
      {
        m_i += delta;
        if (m_i >= m_end)
          m_isOutOfRange = true;
        
        return *this;
      }

    iterator& operator-=(int delta)
      {
        m_i -= delta;
        if (m_i <= m_end)
          m_isOutOfRange = false;
        return *this;
      }
    
    /** Compares two iterators. They are the same also if  we reached
        the end of the range. They are different if one of the two is out of 
        range.
     */
    bool operator==(const iterator& other)
      {
        if (m_isOutOfRange && other.m_isOutOfRange)
          return true;
        else if ((!m_isOutOfRange) && (!other.m_isOutOfRange))
          return true;
        else if (m_isOutOfRange || other.m_isOutOfRange)
          return false;
        return m_i == other.m_i;
      }
    
    bool operator!=(const iterator& other)
      {
        if (m_isOutOfRange && other.m_isOutOfRange)
          return false;
        else if ((!m_isOutOfRange) && (!other.m_isOutOfRange))
          return false;
        else if (m_isOutOfRange || other.m_isOutOfRange)
          return true;
        return m_i != other.m_i;
      }
    
    iterator operator+(int value)
      {
        if (m_i + value >= m_end)
          return iterator();

        return iterator(m_i + value, m_end, m_storage);
      }
      
    size_t distance(iterator &other)
    {
      return std::distance(m_i, other.m_i);
    }

  private:
    Associations::iterator  m_i;
    Associations::iterator  m_end;
    bool                    m_isOutOfRange;
    IgDataStorage          *m_storage;
  };

  IgAssociations(IgDataStorage *storage)
    :m_storage(storage),
     m_resetIterators(true)
    { }

  void associate(const IgRef &a, const IgRef &b);

  /** @return an iterator to the first IgCollectionItem associated with
      the one pointed by @a i.
      
      @a i an iterator to an IgCollectionItem of which we want to get the 
           associated items.
    */
  iterator begin(const IgCollection::iterator &i)
    { return begin(*i); }

  /** @return an iterator to the first IgCollectionItem associated with @a ref.
  
     @a ref an IgRef of which we want to get the associated items.
  */
  iterator begin(const IgRef &ref)
    {
      // Resets all the cached iterators in case this is marked as necessary.
      if (m_resetIterators)
      {
        m_rangeBegin = m_associations.begin();
        m_rangeEnd = m_associations.begin();
        m_cachedRef = IgRef();
        m_resetIterators = false;
      }
      
      // If the ref is the same as the last one cached we simply
      // return it again.
      if (ref == m_cachedRef)
        return m_cachedRangeIterator;
      m_cachedRef = ref;
      
      // First possible association for a given collection.
      // In general associations are navigated in order, which
      // means we can short circuit finding the begin(i+1) reusing end(i).
      AssociationRef first(ref, IgRef(0, 0));
      
      // Given the fact that begin(i+1) is likely to follow up begin(i),
      // we exploit this fact by reusing the old range end as new rangeBegin.
      // In case the above was not the case, we simply fallback to
      // a full binary search of the starting point.
      if (m_rangeEnd != m_associations.end()
          && m_rangeEnd + 1 != m_associations.end()
          && *m_rangeEnd < first 
          && first < *(m_rangeEnd + 1))
        m_rangeBegin = m_rangeEnd;
      else
        m_rangeBegin = std::lower_bound(m_associations.begin(),
                                        m_associations.end(),
                                        first);

      // We then search for the end of the range and return the iterator
      // associated to the range. To do so we look for the lower_bound of the 
      // last possible association for a given ref.
      AssociationRef last(ref);
      m_rangeEnd = std::upper_bound(m_rangeBegin, m_associations.end(), last);

      m_cachedRangeIterator = iterator(m_rangeBegin, m_rangeEnd, m_storage);
      return m_cachedRangeIterator;
    }
  
  /** @return the end iterator for all the associations related to @a ref.
      Notice that the end iterator is always the same regardless of the 
      IgRef that was used for begin.
  */
  iterator end(void)
    {
      return iterator();
    }

  /** Return the number of associations */
  size_t size(void)
    { 
      return m_associations.size();
    }

  /** Clears all the associations*/
  void clear(void)
    { 
      m_associations.clear();
      m_resetIterators = true;
    }

  /** Reserves enough space in the storage to hold @a capacity Associations.
    */
  void reserve(size_t capacity)
    { m_associations.reserve(capacity);}

  /** Resizes the storage to @a newSize elements. Notice that those elements
      will be invalid associations, but the order in the storage will be 
      maintained.
    */
  void resize(size_t newSize)
    { 
      m_associations.resize(newSize);
      m_resetIterators = true;
    }

  void stream(std::ostream &s)
    {
      for (size_t i = 0, e = m_associations.size(); i != e; ++i)
      {
        AssociationRef &assoc = m_associations[i];
        s << "[" << "[" << assoc.key.collectionId << ","
                        << assoc.key.objectId << "],"
                 << "[" << assoc.value.collectionId << ","
                        << assoc.value.objectId << "]]";
        if (i+1 != e)
          s << ",";
      }
    }

private:
  std::string              m_name;
  IgDataStorage           *m_storage;
  Associations             m_associations;
  Associations::iterator   m_rangeBegin;
  Associations::iterator   m_rangeEnd;
  IgRef                    m_cachedRef;
  iterator                 m_cachedRangeIterator;
  bool                     m_resetIterators;
};

class IgDataStorage
{
public:
  // FIXME: quick and dirty...
  typedef std::vector<std::string> CollectionNames;
  typedef std::vector<std::string> AssociationsNames;
  typedef std::vector<IgCollection *> Collections;

  // Data storage is responsible for destroying all its collections
  // and their contents.
  ~IgDataStorage(void)
    {
      for (size_t ai = 0, ae = m_associationsStorage.size(); ai != ae; ++ai)
        delete m_associationsStorage[ai];

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
  //        Maybe in future getCollection and getAssociations could be
  //        put in a templated get<T> function. This needs some template magic
  //        which I rather avoid for the time being.
  IgCollection &getCollection(const char *label)
    {
      return *getCollectionPtr(label);
    }

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

  IgAssociations &getAssociations(const char *label)
    {
      return *getAssociationsPtr(label);
    }

  IgAssociations *getAssociationsPtr(size_t indexInASetNames)
    {
      return m_associationsStorage[indexInASetNames];
    }

  IgAssociations *getAssociationsPtr(const std::string &label)
    {
      return getAssociationsPtr(label.c_str());
    }

  IgAssociations *getAssociationsPtr(const char * label)
    {
      AssociationsNames::iterator n = std::find(m_associationsNames.begin(),
                                                  m_associationsNames.end(),
                                                  label);
      if (n == m_associationsNames.end())
      {
        IgAssociations *associations = new IgAssociations(this);
        m_associationsNames.push_back(label);
        m_associationsStorage.push_back(associations);
        return associations;
      }
      return m_associationsStorage[std::distance(m_associationsNames.begin(), n)];
    }

  IgCollectionItem deref(const IgRef &ref)
    {
      return IgCollectionItem(m_collections[ref.collectionId], ref.objectId);
    }

  CollectionNames &collectionNames(void)
    {
      return m_collectionNames;
    }

  AssociationsNames &associationsNames(void)
    {
      return m_associationsNames;
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
  AssociationsNames m_associationsNames;
  Collections m_collections;
  std::vector<IgAssociations *>   m_associationsStorage;
};

std::ostream &operator<<(std::ostream &stream, IgCollection &collection);
std::ostream &operator<<(std::ostream &stream, IgDataStorage &storage);

inline IgCollectionItem
IgCollection::iterator::operator->(void)
{
  return **this;
}

inline IgCollectionItem 
IgAssociations::iterator::operator*()
{
  assert(!m_isOutOfRange);
  return m_storage->deref(m_i->value);
}

#endif /* IGUANA_IG_COLLECTION_H */
