#ifdef IGUANA_BASE
# include <Iguana/Framework/interface/IgCollection.h>
#else
# include <IgCollection.h>
#endif
#include <iostream>
int
main()
{
  IgDataStorage storage;
  // Create an in memory collection for tracks.
  // We define it by hand, in principle its definition can be read from an ig
  // file, once we are actually able to parse it.
  IgCollection &tracks = storage.getCollection("Tracks/V1");

  IgProperty ID = tracks.addProperty("id" /*label*/, 0 /*default value*/);
  assert (ID.handle().type() == INT_COLUMN);
  IgProperty NAME = tracks.addProperty("name",std::string()); 
  assert (NAME.handle().type() == STRING_COLUMN);
  IgProperty X = tracks.addProperty("x", 0.0);
  IgProperty Y = tracks.addProperty("y", 0.0);
  IgProperty Z = tracks.addProperty("z", 0.0);
  IgProperty P_X = tracks.addProperty("px", 1.0);
  IgProperty P_Y = tracks.addProperty("py", 0.0);
  IgProperty P_Z = tracks.addProperty("pz", 0.0);

  // Read back the column labels in the `check` vector.
  std::vector<std::string> check;
  std::vector<IgCollection::LabelColumn> &labels = tracks.columnLabels();
  for(std::vector<IgCollection::LabelColumn>::iterator i = labels.begin();
      i != labels.end();
      i++)
  {
    check.push_back(i->first);
  }
  
  // Checking that the names are actually the correct ones.
  assert(check[0] == "id");
  assert(check[1] == "name");
  assert(check[2] == "x");
  assert(check[3] == "y");
  assert(check[4] == "z");
  assert(check[5] == "px");
  assert(check[6] == "py");
  assert(check[7] == "pz");
}
