#ifdef PROJECT_NAME
# include <Iguana/Framework/interface/IgCollection.h>
# include <Iguana/Framework/interface/IgParser.h>
#else
# include <IgCollection.h>
# include <IgParser.h>
#endif

#include <iostream>
#include <sstream>
#include <string>


void
validateStorage(IgDataStorage &storage)
{
  std::ostringstream os;
  os << storage << std::flush;
  IgDataStorage tmpStorage;
  IgParser tmpParser(&tmpStorage);
  tmpParser.parse(os.str().c_str());
}

int
main(int argc, char **argv)
{
  IgDataStorage storage;
  validateStorage(storage);
  
  // Create a collection of tracks.
  IgCollection &tracks = storage.getCollection("Tracks/V1");
  validateStorage(storage);

  IgProperty X = tracks.addProperty("x", 0.0);
  IgProperty Y = tracks.addProperty("y", 0.0);
  IgProperty Z = tracks.addProperty("z", 0.0);
  IgProperty P_X = tracks.addProperty("px", 1.0);
  IgProperty P_Y = tracks.addProperty("py", 0.0);
  IgProperty P_Z = tracks.addProperty("pz", 0.0);

  // Create a collection for clusters.
  IgCollection &clusters = storage.getCollection("Clusters/V1");
  IgProperty C_X = clusters.addProperty("x", 0.0);
  IgProperty C_Y = clusters.addProperty("y", 0.0);
  IgProperty C_Z = clusters.addProperty("z", 0.0);
  IgProperty C_E = clusters.addProperty("e", 0.0);

  // Create a collection for points to test persistency of IgV3d
  IgCollection &points = storage.getCollection("TestPoints/V1");
  IgProperty POINT_2D = points.addProperty("pos2d", IgV2d());
  IgProperty POINT_3D = points.addProperty("pos3d", IgV3d());
  IgProperty POINT_4D = points.addProperty("pos4d", IgV4d());
  validateStorage(storage);

  // Add a few tracks.
  for (int i = 0; i < 500000 ; i++)
  {
    IgCollectionItem t = tracks.create();
    t[X] = static_cast<double>(i);
    t[Y] = static_cast<double>(i);
    t[Z] = static_cast<double>(i);
    t[P_X] = static_cast<double>(i);
    t[P_Y] = static_cast<double>(i);
    t[P_Z] = static_cast<double>(i);
  }

  // Add a few clusters.
  for (int i = 0; i < 500000 ; i++)
  {
    IgCollectionItem c = clusters.create();
    c[C_X] = static_cast<double>(i);
    c[C_Y] = static_cast<double>(i);
    c[C_Z] = static_cast<double>(i);
    c[C_E] = static_cast<double>(i);
  }
  
  // Add a few points
  for (int i = 0; i < 500000 ; i++)
  {
    IgCollectionItem p = points.create();
    p[POINT_2D] = IgV2d(1*i, 2*i);
    p[POINT_3D] = IgV3d(1*i, 2*i, 3*i);
    p[POINT_4D] = IgV4d(1*i, 2*i, 3*i, 4*i);
  }
  
  // One to one associations
  {  
    IgAssociationSet &trackClusters = storage.getAssociationSet("TrackClusters/V1");
    IgCollectionIterator c = clusters.begin(); 
    IgCollectionIterator t = tracks.begin();
    
    while((c != clusters.end()) && (t != tracks.end()))
    {
      trackClusters.associate (*t, *c);
      t++;
      c++;
    }
  }
  // One to many
  {
    IgAssociationSet &trackClusters = storage.getAssociationSet("TrackClusters2/V1");
    
    IgCollectionIterator c = clusters.begin(); 
    IgCollectionIterator t = tracks.begin();
    
    while((c != clusters.end()) && (t != tracks.end()))
    {
      trackClusters.associate (*t, *c);
      t++;
    }
  }
  // Writing out in Ig JSON based format.
  std::ostringstream os1;
  os1 << storage << std::endl;
  std::cout << "Size of the ig-file: " << os1.str().size() 
            << " bytes." << std::endl;
  
  // Parse the data in a new storage.
  IgDataStorage storage2;
  IgParser parser(&storage2);
  parser.parse(os1.str().c_str());
  
  // Stream it back.
  std::ostringstream os2;
  os2 << storage2 << std::endl;
  
  // Read again.
  IgDataStorage storage3;
  IgParser parser2(&storage3);
  parser2.parse(os2.str().c_str());

  // Write it again
  std::ostringstream os3;
  os3 << storage3 << std::endl;
  
  // Check it is always the same.
  assert (os2.str() == os1.str());
  assert (os3.str() == os1.str());  
}
