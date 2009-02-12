#ifdef PROJECT_NAME
# include <Iguana/Framework/interface/IgCollection.h>
#else
# include <IgCollection.h>
#endif
#include <iostream>
#include <sstream>

int
main()
{
  IgDataStorage storage;
  
  // Setup the data structure. This is not strictly necessary, as in general it
  // will be read from file, but we do it by hand, to simplify the test.
  // * Create a collection of tracks.
  // * Create a collection for additional measurements.
  // * Create an association set to associate the measurements to the given tracks.
  // * Add a few tracks and the associated measurements. 
  //   Each track as a different number of measurements: 
  //
  //       * 1st track 0 measurements, 
  //       * 2nd track 1 measurement,
  //       * 3rd track 2 measurement
  //
  // etc.)
  // * Create a vector with the expected results.

  IgCollection &tracks = storage.getCollection("Tracks/V1");
  IgProperty P_T = tracks.addProperty("pt", 0.0);
  IgProperty X = tracks.addProperty("x", 0.0);
  IgProperty Y = tracks.addProperty("y", 0.0);
  IgProperty Z = tracks.addProperty("z", 0.0);
  IgProperty P_X = tracks.addProperty("px", 1.0);
  IgProperty P_Y = tracks.addProperty("py", 0.0);
  IgProperty P_Z = tracks.addProperty("pz", 0.0);

  IgCollection &measurements = storage.getCollection("Measurements/V1");
  IgProperty M_X = measurements.addProperty("x", 0.0);
  IgProperty M_Y = measurements.addProperty("y", 0.0);
  IgProperty M_Z = measurements.addProperty("z", 0.0);

  IgAssociationSet &trackMeasurements = storage.getAssociationSet("TrackMeasurements/V1");

  for (int i = 0; i < 10 ; i++)
  {
    IgCollectionItem t = tracks.create();
    t[P_T] = static_cast<double>(i*10);
    t[X] = static_cast<double>(i*10);
    t[Y] = static_cast<double>(i*10);
    t[Z] = static_cast<double>(i*10);
    t[P_X] = static_cast<double>(i*10);
    t[P_Y] = static_cast<double>(i*10);
    t[P_Z] = static_cast<double>(i*10);
      for (int j = 0; j < i ; j++)
      {
        IgCollectionItem m = measurements.create();
        m[M_X] = static_cast<double>(j);
        m[M_Y] = static_cast<double>(j);
        m[M_Z] = static_cast<double>(j);
        trackMeasurements.associate(t, m);
      }
  }
  
  // Debug stuff... print out the result.
  std::cerr << storage << std::endl;
  
  std::vector<double> expectedResults;
  expectedResults.push_back(0);
  expectedResults.push_back(0);
  expectedResults.push_back(1);
  expectedResults.push_back(0);
  expectedResults.push_back(1);
  expectedResults.push_back(2);
  expectedResults.push_back(0);
  expectedResults.push_back(1);
  expectedResults.push_back(2);
  expectedResults.push_back(3);
  expectedResults.push_back(0);
  expectedResults.push_back(1);
  expectedResults.push_back(2);
  expectedResults.push_back(3);
  expectedResults.push_back(4);
  expectedResults.push_back(0);
  expectedResults.push_back(1);
  expectedResults.push_back(2);
  expectedResults.push_back(3);
  expectedResults.push_back(4);
  expectedResults.push_back(5);
  expectedResults.push_back(0);
  expectedResults.push_back(1);
  expectedResults.push_back(2);
  expectedResults.push_back(3);
  expectedResults.push_back(4);
  expectedResults.push_back(5);
  expectedResults.push_back(6);
  expectedResults.push_back(0);
  expectedResults.push_back(1);
  expectedResults.push_back(2);
  expectedResults.push_back(3);
  expectedResults.push_back(4);
  expectedResults.push_back(5);
  expectedResults.push_back(6);
  expectedResults.push_back(7);
  expectedResults.push_back(0);
  expectedResults.push_back(1);
  expectedResults.push_back(2);
  expectedResults.push_back(3);
  expectedResults.push_back(4);
  expectedResults.push_back(5);
  expectedResults.push_back(6);
  expectedResults.push_back(7);
  expectedResults.push_back(8);

  std::ostringstream os1;
  std::ostringstream os2;

  // Real part of the test, we iterate on tracks their measurements.
  {
    int n = 0;
    IgCollection & myTracks = storage.getCollection("Tracks/V1");
    IgCollection & myMeasurements = storage.getCollection("Measurements/V1");
    std::vector<double>::iterator eri = expectedResults.begin();
    std::vector<double>::iterator eri2 = expectedResults.begin();
    
    for (IgCollectionIterator t = myTracks.begin();
         t != myTracks.end();
         t++)
    {
      IgCollectionItem track = *t;
      os1 << "Hits for track n. " << track.currentRow() << std::endl;

      IgAssociatedSet view = storage.getAssociatedSet("TrackMeasurements/V1", track);
      IgAssociatedSet view2 = storage.getAssociatedSet("TrackMeasurements/V1", track, LEFT_ASSOCIATED);
      IgAssociatedSet view3 = storage.getAssociatedSet("TrackMeasurements/V1", track, RIGHT_ASSOCIATED);
      for (IgAssociatedSet::Iterator a = view.begin();
           a != view.end();
           a++)
      {
         IgCollectionItem m = *a;
         os1 << "  " << m.get<double>("x")
             << " " << m.get<double>("y")
             << " " << m.get<double>("z") << std::endl;
         assert(*eri == m.get<double>("x"));
         assert(*eri == m.get<double>("y"));
         assert(*eri++ == m.get<double>("z"));
         assert(n++ < 55);
      }

      for (IgAssociatedSet::Iterator a = view2.begin();
           a != view2.end();
           a++)
      {
        assert(false);
      }

      for (IgAssociatedSet::Iterator a = view3.begin();
           a != view3.end();
           a++)
      {
         IgCollectionItem m = *a;
         assert(*eri2 == m.get<double>("x"));
         assert(*eri2 == m.get<double>("y"));
         assert(*eri2++ == m.get<double>("z"));
      }

    }
  }

  // Explicit way of doing the navigation, this is here only for reference 
  // while the above way should be preferred.
  {
    int n = 0;
    IgCollection & myTracks = storage.getCollection("Tracks/V1");
    IgCollection & myMeasurements = storage.getCollection("Measurements/V1");
    IgAssociationSet & myAssociations = storage.getAssociationSet("TrackMeasurements/V1");
    std::vector<double>::iterator eri = expectedResults.begin();
    
    for (IgCollectionIterator t = myTracks.begin();
         t != myTracks.end();
         t++)
    {
      IgCollectionItem track = *t;
      os2 << "Hits for track n. " << track.currentRow() << std::endl;
      for (IgAssociationSet::Iterator a = myAssociations.begin();
           a != myAssociations.end();
           a++)
      {
        if(a->first().objectId() == track.currentRow())
        {
           IgCollectionItem m(&myMeasurements, a->second().objectId());
           os2 << "  " << m.get<double>("x")
               << " " << m.get<double>("y")
               << " " << m.get<double>("z") << std::endl;
           assert(*eri == m.get<double>("x"));
           assert(*eri == m.get<double>("y"));
           assert(*eri++ == m.get<double>("z"));
           assert(n++ < 55);
        }
      }
    }
  }
  assert(os1.str() == os2.str());
}
