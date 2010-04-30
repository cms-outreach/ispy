#ifdef PROJECT_NAME
# include <Iguana/Framework/interface/IgCollection.h>
#else
# include <IgCollection.h>
#endif
#include <iostream>
#include <sstream>

#include <QtTest/QtTest>

void
foo()
{
  std::cerr << "debug" << std::endl;
}

void
doTestNavigateOneAssociation()
{
  IgDataStorage storage;
  IgCollection &a = storage.getCollection("A/V1");
  IgCollection &b = storage.getCollection("B/V1");
  IgProperty A = a.addProperty("a", 0);
  IgProperty B = b.addProperty("b", 0);
  IgAssociations &ba = storage.getAssociations("AB/V1");

  IgCollectionItem t1 = a.create();
  IgCollectionItem t2 = b.create();
  t1[A] = 1;
  t2[B] = 2;
  ba.associate(t2, t1);    
  bool found = false;
  for (IgCollection::iterator ci = b.begin(), ce = b.end(); ci != ce; ++ci)
  {
    QVERIFY(ci->get<int>(B) == 2);
    for (IgAssociations::iterator ai = ba.begin(ci), ae = ba.end(); ai != ae; ++ai)
    {
      QVERIFY(ai->get<int>(A) == 1);
      found = true;
    }
  }
  QVERIFY(found);
}

void
doTestNavigateAssociationsOneToOne()
{
  IgDataStorage storage;
  IgCollection &a = storage.getCollection("A/V1");
  IgCollection &b = storage.getCollection("B/V1");
  IgProperty A = a.addProperty("a", 0);
  IgProperty B = b.addProperty("b", 0);
  IgAssociations &ab = storage.getAssociations("AB/V1");
  IgAssociations &ba = storage.getAssociations("BA/V1");
  for (int i = 0; i < 10 ; ++i)
  {
    IgCollectionItem t1 = a.create();
    IgCollectionItem t2 = b.create();
    t1[A] = i;
    t2[B] = i;
    ab.associate(t1, t2);
    ba.associate(t2, t1);    
  }

  QVERIFY(ab.size() == 10);
  size_t count = 0;
  for (IgCollection::iterator ci = a.begin(), ce = a.end(); ci != ce; ++ci)
  {
    QVERIFY(ci->get<int>(A) == count);
    for (IgAssociations::iterator ai = ab.begin(ci), ae = ab.end(); ai != ae; ++ai)
    {
      QVERIFY(ai->get<int>(B) == count);
      count++;
    }
  }
  std::cerr << count << std::endl;
  QVERIFY(count == 10);
  
  QVERIFY(ab.size() == 10);
  count = 0;
  for (IgCollection::iterator ci = b.begin(), ce = b.end(); ci != ce; ++ci)
  {
    QVERIFY(ci->get<int>(B) == count);
    for (IgAssociations::iterator ai = ba.begin(ci), ae = ba.end(); ai != ae; ++ai)
    {
      if (count == 8)
        foo();
      QVERIFY(ai->get<int>(A) == count);
      count++;
    }
  }
  std::cerr << count << std::endl;
  QVERIFY(count == 10);
}

void
doTestNavigateAssociations()
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

  IgAssociations &trackMeasurements = storage.getAssociations("TrackMeasurements/V1");
  // We also create an empty association to test that iteration works also in 
  // case no objects are found.
  storage.getAssociations("EmptyAssociations/V1");
  std::vector<double> expectedResults;

  int k = 0;
  for (int i = 0; i < 10 ; ++i)
  {
    IgCollectionItem t = tracks.create();
    QVERIFY(t.currentRow() == i);
    t[P_T] = static_cast<double>(i*10);
    t[X] = static_cast<double>(i*10);
    t[Y] = static_cast<double>(i*10);
    t[Z] = static_cast<double>(i*10);
    t[P_X] = static_cast<double>(i*10);
    t[P_Y] = static_cast<double>(i*10);
    t[P_Z] = static_cast<double>(i*10);
    // Notice that the first track does not have any associations!
    for (int j = 0; j < i ; ++j)
    {
      IgCollectionItem m = measurements.create();
      QVERIFY(m.currentRow() == k++);
      m[M_X] = static_cast<double>(i);
      m[M_Y] = static_cast<double>(i);
      m[M_Z] = static_cast<double>(i);
      trackMeasurements.associate(t, m);
      expectedResults.push_back(i);
    }
  }
  std::cerr << storage;
  
  std::ostringstream os1;
  std::ostringstream os2;

  // Real part of the test, we iterate on tracks their measurements.
  {
    int n = 0;
    IgCollection &myTracks = storage.getCollection("Tracks/V1");
    IgCollection &myMeasurements = storage.getCollection("Measurements/V1");
    IgAssociations &views = storage.getAssociations("TrackMeasurements/V1");
    IgAssociations &empty = storage.getAssociations("EmptyAssociations/V1");
    
    std::vector<double>::iterator eri = expectedResults.begin();
    
    IgProperty X(myMeasurements, "x");
    IgProperty Y(myMeasurements, "y");
    IgProperty Z(myMeasurements, "z");

    for (IgCollection::iterator ti = myTracks.begin(), te = myTracks.end();
         ti != te;
         ++ti)
    {
      std::cerr << "Hits for track n. " << ti->currentRow() << std::endl;

      for (IgAssociations::iterator ai = views.begin(ti), ae = views.end();
           ai != ae; ++ai)
      {
        std::cerr << "  " << ai->get<double>(X)
                  << " == " << *eri << std::endl;
                  
        QVERIFY(*eri == ai->get<double>(X));
        QVERIFY(*eri == ai->get<double>(Y));
        QVERIFY(*eri++ == ai->get<double>(Z));
        QVERIFY(n++ < 55);
      }
      
      bool notExecuted = true;
      for (IgAssociations::iterator ei = empty.begin(ti), ee = empty.end();
           ei != ee;
           ++ei)
        notExecuted = false;
      QVERIFY(notExecuted);
    }

    n = 0;
    eri = expectedResults.begin();

    // There was an issue with the ai++ operator. Let's check it gives the
    // same results. 
    for (IgCollection::iterator ti = myTracks.begin(), te = myTracks.end();
         ti != te;
         ++ti)
    {
      std::cerr << "Hits for track n. " << ti->currentRow() << std::endl;

      for (IgAssociations::iterator ai = views.begin(ti), ae = views.end();
           ai != ae; ai++)
      {
        std::cerr << "  " << ai->get<double>(X)
                  << " == " << *eri << std::endl;
                  
        QVERIFY(*eri == ai->get<double>(X));
        QVERIFY(*eri == ai->get<double>(Y));
        QVERIFY(*eri++ == ai->get<double>(Z));
        QVERIFY(n++ < 55);
      }
    }
  }


  // Twice! But starting halfway...
  {
    IgCollection &myTracks = storage.getCollection("Tracks/V1");
    IgCollection &myMeasurements = storage.getCollection("Measurements/V1");
    IgAssociations &views = storage.getAssociations("TrackMeasurements/V1");
    
    std::vector<double>::iterator eri = expectedResults.begin() + 0 + 1 + 2 + 3 + 4;
    
    IgProperty X(myMeasurements, "x");
    IgProperty Y(myMeasurements, "y");
    IgProperty Z(myMeasurements, "z");

    for (IgCollection::iterator ti = myTracks.begin() + 5, te = myTracks.end();
         ti != te;
         ++ti)
    {
      std::cerr << "Hits for track n. " << ti->currentRow() << std::endl;

      for (IgAssociations::iterator ai = views.begin(ti), ae = views.end();
           ai != ae; ++ai)
      {
        std::cerr << "  " << ai->get<double>(X)
                  << " == " << *eri << std::endl;
                  
        QVERIFY(*eri == ai->get<double>(X));
        QVERIFY(*eri == ai->get<double>(Y));
        QVERIFY(*eri++ == ai->get<double>(Z));
      }
    }
  }
  
  // Twice! But starting halfway...and skipping a few...
  {
    IgCollection &myTracks = storage.getCollection("Tracks/V1");
    IgCollection &myMeasurements = storage.getCollection("Measurements/V1");
    IgAssociations &views = storage.getAssociations("TrackMeasurements/V1");
    
    std::vector<double>::iterator eri = expectedResults.begin() + 0 + 1 + 2 + 3 + 4;
    
    IgProperty X(myMeasurements, "x");
    IgProperty Y(myMeasurements, "y");
    IgProperty Z(myMeasurements, "z");

    for (IgCollection::iterator ti = myTracks.begin() + 5, te = myTracks.end();
         ti != te;
         ++ti)
    {
      std::cerr << "Hits for track n. " << ti->currentRow() << std::endl;

      if (ti->currentRow() % 2)
      {
        eri += ti->currentRow();
        continue;
      }
      
      for (IgAssociations::iterator ai = views.begin(ti), ae = views.end();
           ai != ae; ++ai)
      {
        std::cerr << "  " << ai->get<double>(X)
                  << " == " << *eri << std::endl;
                  
        QVERIFY(*eri == ai->get<double>(X));
        QVERIFY(*eri == ai->get<double>(Y));
        QVERIFY(*eri++ == ai->get<double>(Z));
      }
    }
  }

}
