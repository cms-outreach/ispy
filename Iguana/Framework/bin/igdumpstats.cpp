#ifdef PROJECT_NAME
# include <Iguana/Framework/interface/IgCollection.h>
# include <Iguana/Framework/interface/IgParser.h>
#else
# include <IgCollection.h>
# include <IgParser.h>
#endif

#include <cstdio>
#include <iostream>

int
main(int argc, char **argv)
{
  if (argc != 2)
  {
    std::cerr << "usage: igdump <ig-file>" << std::endl;
    exit (1);
  }
  IgDataStorage storage;
  IgParser parser(&storage);
  FILE *f = fopen (argv[1], "r");
  fseek(f, 0, SEEK_END);
  int size = ftell(f);
  fseek(f, 0, SEEK_SET);
  char *buffer = new char[size];
  fread(buffer, size, 1, f);
  fclose(f);
  parser.parse(buffer);
  std::cerr << "Some stats on file " << argv[1] << "\n"
            << "File size:" << size << "\n" 
            << "N. of collections:" << storage.collectionNames().size() << "\n";
  for (IgDataStorage::CollectionNames::const_iterator i = storage.collectionNames().begin();
       i != storage.collectionNames().end();
       i++)
  {
    std::cerr << "  "<< *i << "\n";
  }
  std::cerr << "N. of associations:" << storage.associationSetNames().size() << "\n";
  for (IgDataStorage::AssociationSetNames::const_iterator i = storage.associationSetNames().begin();
       i != storage.associationSetNames().end();
       i++)
  {
    std::cerr << "  "<< *i << "\n";
  }
  std::cerr << std::endl;
}
