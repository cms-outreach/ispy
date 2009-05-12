#ifdef PROJECT_NAME
# include <Iguana/Framework/interface/IgCollection.h>
#else
# include <IgCollection.h>
#endif
#include <iostream>
int
main()
{
  IgDataStorage *storage = new IgDataStorage;
  assert(storage->empty() == true);

  // Create an in memory collection for tracks.
  // We define it by hand, in principle its definition can be read from an ig
  // file, once we are actually able to parse it.
  IgCollection &tracks = storage->getCollection("Tracks/V1");

  IgProperty SOMETHING = tracks.addProperty("someInt" /*label*/, 0 /*default value*/);
  assert (SOMETHING.handle().type() == INT_COLUMN);
  IgProperty SOMESTRING = tracks.addProperty("someString",std::string()); 
  assert (SOMESTRING.handle().type() == STRING_COLUMN);
  IgProperty X = tracks.addProperty("x", 0.0);
  IgProperty Y = tracks.addProperty("y", 0.0);
  IgProperty Z = tracks.addProperty("z", 0.0);
  IgProperty P_X = tracks.addProperty("px", 1.0);
  IgProperty P_Y = tracks.addProperty("py", 0.0);
  IgProperty P_Z = tracks.addProperty("pz", 0.0);

  // Creating a some tracks in the tracks collection.
  // First way of doing it: by label.
  IgCollectionItem t1 = tracks.create();
  assert(t1.currentColumn() == 0);
  t1["someInt"] = 1;
  assert(t1.currentColumn() == 0);
  t1["someString"] = std::string("foo");
  assert(t1.currentColumn() == 1);
  t1["x"] = 2.0;
  t1["y"] = 2.0;
  t1["z"] = 2.0;
  t1["px"] = 2.0;
  t1["py"] = 2.0;
  t1["pz"] = 2.0;
  
  // Second way of doing it: using the property index to random access the 
  // setters. 
  IgCollectionItem t2 = tracks.create();
  t2[SOMETHING] = 4;
  t2[SOMESTRING] = "foo2";
  t2[X] = 2.0;
  t2[Y] = 2.0;
  t2[Z] = 2.0;
  t2[P_X] = 2.0;
  t2[P_Y] = 2.0;
  t2[P_Z] = 2.0;
  
  // Third way of doing it
  IgCollectionItem t3 = tracks.create();
  t3 = 9, std::string("foo4"), 3.0, 3.0, 3.0, 3.0, 3.0, 3.0;
  
  // Yet another way:
  // * Alias X to POS and P_X to P.
  // * Fill POS from a vector.
  // * Fill P using the comma operator.
  IgProperty POS = X;
  IgProperty P = P_X;
  IgCollectionItem t4 = tracks.create();
  std::vector<double> pos(3, 4.0);
  pos[2] = 6;
  t4[POS] = pos;
  t4[P] = 5.0, 5.0, 5.5;
  std::vector<double> p(2, 4);
  t4[P] = 5.0, p;
  t4[P] = p, 5.0;
  
  assert(tracks.size() == 4);
  
  // Reading it back and printing everything to screen.
  for (IgCollectionIterator i = tracks.begin();
       i != tracks.end(); 
       i++)
  {
    std::cerr << "someInt" << ":" << (*i).get<int>("someInt") << ","
              << "someString" << ":" << (*i).get<std::string>("someString") << ","
              << "X" << ":" << (*i).get<double>("x") << ","
              << "Y" << ":" << (*i).get<double>("y") << ","
              << "Z" << ":" << (*i).get<double>("z") << ","
              << "P_X" << ":" << (*i).get<double>("px") << ","
              << "P_Y" << ":" << (*i).get<double>("py") << ","
              << "P_Z" << ":" << (*i).get<double>("pz") << "," 
              << std::endl;
  }
  
  //Adding another collection
  IgCollection &clusters= storage->getCollection("Clusters/V1");
  IgProperty C_X = clusters.addProperty("x", 0.0);
  IgProperty C_Y = clusters.addProperty("y", 0.0);
  IgProperty C_Z = clusters.addProperty("z", 0.0);
  IgProperty C_E = clusters.addProperty("e", 0.0);
 
  // Checks getProperty / hasProperty API. 
  assert(clusters.hasProperty ("e"));
  assert(!clusters.hasProperty ("foo"));
  assert(clusters.getProperty ("e").handle().data() == C_E.handle().data());
  bool didThrow = false;
  try {
    clusters.getProperty("foo");
  } 
  catch (IgSchemaError &e)
  {
    didThrow = true;
  }
  assert (didThrow);

  IgCollectionItem c1 = clusters.create();
  c1["e"] = 5.4;
  c1["z"] = 5.3;
  c1["y"] = 5.2;
  c1["x"] = 5.1;
  
  
  // Writing another collection, this time using IgVect3D and IgVect4D
  {
    IgCollection &measurements = storage->getCollection("TestMeasurement/V1");
    
    IgProperty POS = measurements.addProperty("position", IgV3d());
    IgProperty P = measurements.addProperty("quadmomentum", IgV4d());
    
    IgCollectionItem m = measurements.create();
    m[POS] = IgV3d(1,0,0);
    m[P] = IgV4d(1,0,0,0);
  }

  assert(storage->empty() == false);
  assert(storage->hasCollection("TestMeasurement/V1") == true);
  assert(storage->hasCollection("TestMeasurement/V2") == false);
// Writing out in Ig JSON based format.
  std::cerr << *storage << std::endl;
  delete storage;
}
