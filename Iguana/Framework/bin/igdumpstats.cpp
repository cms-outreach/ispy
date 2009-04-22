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
  bool dumpTracks = false;
  int filenamePos = 1; 
  
  if (argc < 2)
  {
    std::cerr << "usage: igdump [--dump-tracks] <ig-file>" << std::endl;
    exit (1);
  }
  
  for (int c = 0; c < argc; c++)
  {
    if (strcmp(argv[c], "--dump-tracks") == 0)
    {
      dumpTracks = true;
    }
    else
    {
      filenamePos = c;
    }
  }
  
  IgDataStorage storage;
  IgParser parser(&storage);
  FILE *f = fopen (argv[filenamePos], "r");
  if (!f)
  {
    std::cerr << "Unable to open file " << argv[filenamePos] << std::endl;
    exit(1);
  }
  fseek(f, 0, SEEK_END);
  int size = ftell(f);
  fseek(f, 0, SEEK_SET);
  char *buffer = new char[size];
  fread(buffer, size, 1, f);
  fclose(f);
  parser.parse(buffer);
  std::cerr << "Some stats on file " << argv[filenamePos] << "\n"
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
  
  if (dumpTracks)
  {
    IgCollection *tracks = storage.getCollectionPtr ("Tracks_V1");
    for (IgCollectionIterator i = tracks->begin();
         i != tracks->end();
         i++)
    {
      IgCollectionItem track = *i;
      IgAssociatedSet extras = storage.getAssociatedSet("TrackExtras_V1", track);
      for (IgAssociatedSet::Iterator e = extras.begin();
           e != extras.end();
           e++)
      {
        IgCollectionItem extra = *e;
        std::cerr << "Track: " << track.currentRow() << std::endl;
        IgV3d pos_1 = extra.get<IgV3d>("pos_1");
        IgV3d dir_1 = extra.get<IgV3d>("pos_2");
        IgV3d pos_2 = extra.get<IgV3d>("dir_1");
        IgV3d dir_2 = extra.get<IgV3d>("dir_2");
         
        std::cerr << "Measurements: (" << pos_1.x() << ", " << pos_1.y() << ", " << pos_1.z() << "), "
                  << "(" << dir_1.x() << ", " << dir_1.y() << ", " << dir_1.z() << "), "
                  << "(" << pos_2.x() << ", " << pos_2.y() << ", " << pos_2.z() << "), "
                  << "(" << dir_2.x() << ", " << dir_2.y() << ", " << dir_2.z() << ") "
                  << std::endl;
      }
    }
  }
}
