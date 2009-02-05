#include "Ig_Modules/IgConfig/interface/IgConfigElement.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgArgsElement.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgState.h"
#include <qfile.h>
#include <qtextstream.h>
#include <iostream>
#include <string>
#include <vector>
#include <classlib/utils/DebugAids.h>

int failed = 0;
template<class T>
void check(IgConfigElement* conf, std::string key, T value)
{
  T flag;
  key = "/iguana/application/setup/"+key;
  std::cout <<"Checking "<<key<<" ..... ";
  conf->getValue(key, flag);
  if(flag == value)
    std::cout <<"PASS";
  else
  {
    std::cout <<"FAIL";
    failed++;
  }
  std::cout <<" ("<<flag<<","<<value<<")"<<std::endl;  
}

int main(int /*argc*/, char* argv[])
{
    std::cout.setf(std::ios::fixed);
    std::cout.precision(20);
    std::string filename("/tmp/test_IgConfigElement.conf");
    QFile file (filename.c_str());
    
    ASSERT (file.open (IO_WriteOnly));
    
    std::map<std::string, std::string> data;
    data["true1"] = "1";
    data["true2"] = "t";
    data["true3"] = "T";
    data["true4"] = "true";
    data["true5"] = "TRUE";
    data["true6"] = "True";
    data["false1"] = "0";
    data["false2"] = "f";
    data["false3"] = "F";
    data["false4"] = "false";
    data["false5"] = "FALSE";
    data["false6"] = "False";
    data["false7"] = "any";
    data["int1"] = "123";
    data["int2"] = "-123";
    data["uint"] = "123456789";
    data["long1"] = "123456789";
    data["long2"] = "-123456789";
    data["ulong"] = "123456789";
    data["float1"] = "1.23456789";
    data["float2"] = "-1.23456789";
    data["double"] = "1.234567890123";
    data["string"] = "any string";
    data["vstring"] = "a,b,c,d,e";
    data["vstring_int"] = "1,2,3,    4,5,6";
    
    QTextStream stream( &file );
    stream <<"<iguana>\n"
           <<"  <application>\n"
	   <<"    <setup>\n";
    std::map<std::string, std::string>::const_iterator itr = data.begin();
    while(itr!=data.end())
    {
      stream <<"      <"<<itr->first<<">"<<itr->second<<"</"<<itr->first<<">\n";
      itr++;
    }
    stream <<"    </setup>\n"
           <<"  </application>\n"
	   <<"</iguana>\n";
    file.close();
    
    std::vector<std::string> arguments;
    arguments.push_back(argv[0]);
    arguments.push_back("--ig-config");
    arguments.push_back(filename);
    char* argv1[4];
    argv1[0] = &arguments[0][0];
    argv1[1] = &arguments[1][0];
    argv1[2] = &arguments[2][0];
    argv1[3] = '\0';
    IgState* state = new IgState();
    new IgArgsElement(state, 3, argv1);
    new IgConfigElement(state);
    file.remove ();
    
    IgConfigElement* conf = IgConfigElement::get(state);
    std::string value, key;
    
    std::cout <<"First Pass:"<<std::endl;
    itr = data.begin();
    while(itr!=data.end())
    {
      key = "/iguana/application/setup/"+itr->first;
      std::cout <<"Checking "<<key<<" ..... ";
      if (!conf->getValue(key, value) || (value != itr->second))
      {
        std::cout <<"FAIL";
	failed++;
      }
      else
        std::cout <<"PASS";
      std::cout <<" ("<<value<<"!="<<itr->second<<")"<<std::endl;
      itr++;
    }

    key = "/iguana/application/setup/tempItem/key";
    conf->putValue(key, "value");
    std::cout <<"Checking "<<key<<" ..... ";
    if (!conf->getValue(key, value) || (value != "value"))
    {
      std::cout <<"FAIL";
      failed++;
    }
    else
      std::cout <<"PASS";
    std::cout <<" ("<<value<<"!=value)"<<std::endl;
    
    //testing get/putValues for vector<string>
    bool pass=true;
    std::vector<std::string> svec;
    svec.push_back("A"); svec.push_back("B"); svec.push_back("C");
    key = "/iguana/application/setup/tempItem/StringVec";
    conf->putValues(key, svec);
    std::cout <<"Checking "<<key<<" ..... ";
    svec.clear();
    if(!conf->getValues(key, svec))
    {
        pass=false;
	failed++;
    }
    else
    {
        char ch = 'A';
        for(size_t i=0; i<svec.size(); i++)
        {
	    if ((svec[i].size()!=1) || (svec[i][0]!=ch))
	    {
	        pass=false;
		failed++;
	        break;
	    }
	    else
	        ch++;
        }
    }
    if(!pass)
        std::cout <<"FAIL";
    else
        std::cout <<"PASS";
    std::cout <<std::endl;
     
    //testing get/putValues for vector<double>
    pass=true;
    std::vector<double> dvec;
    double mdouble = 5.1234567890123456789;
    dvec.push_back(mdouble++); dvec.push_back(mdouble++); dvec.push_back(mdouble++);
    key = "/iguana/application/setup/tempItem/DoubleVec";
    conf->putValues(key, dvec);
    std::cout <<"Checking "<<key<<" ..... ";
    dvec.clear();
    if(!conf->getValues(key, dvec))
    {
        pass=false;
	failed++;
    }
    else
    {
        mdouble = 5.1234567890123456789;
        for(size_t i=0; i<dvec.size(); i++)
        {
	    if (dvec[i]!=mdouble++)
	    {
	        pass=false;
		failed++;
	        break;
	    }
        }
    }
    if(!pass)
        std::cout <<"FAIL";
    else
        std::cout <<"PASS";
    std::cout <<std::endl;
    
    //testing get/putValues for vector<int>
    pass=true;
    std::vector<int> ivec;
    int mint = 100;
    ivec.push_back(mint++); ivec.push_back(mint++); ivec.push_back(mint++);
    key = "/iguana/application/setup/tempItem/IntVec";
    conf->putValues(key, ivec);
    std::cout <<"Checking "<<key<<" ..... ";
    ivec.clear();
    if(!conf->getValues(key, ivec))
    {
        pass=false;
	failed++;
    }
    else
    {
        mint = 100;
        for(size_t i=0; i<ivec.size(); i++)
        {
	    if (ivec[i]!=mint++)
	    {
	        pass=false;
		failed++;
	        break;
	    }
        }
    }
    if(!pass)
        std::cout <<"FAIL";
    else
        std::cout <<"PASS";
    std::cout <<std::endl;

    //testing get/putValues for vector<bool>
    pass=true;
    std::vector<bool> bvec;
    bool mbool = true;
    bvec.push_back(mbool); bvec.push_back(!mbool); bvec.push_back(mbool);
    key = "/iguana/application/setup/tempItem/BoolVec";
    conf->putValues(key, bvec);
    std::cout <<"Checking "<<key<<" ..... ";
    bvec.clear();
    if(!conf->getValues(key, bvec))
    {
        pass=false;
	failed++;
    }
    else
    {
	mbool=true;
	for(size_t i=0; i<bvec.size(); i++)
        {
	    if (bvec[i]!=mbool)
	    {
	        pass=false;
		failed++;
	        break;
	    }
	    else
	        mbool=!mbool;
        }
    }
    if(!pass)
        std::cout <<"FAIL";
    else
        std::cout <<"PASS";
    std::cout <<std::endl;

    std::cout <<std::endl<<"Second Pass:"<<std::endl;
    itr = data.begin();
    while(itr!=data.end())
    {
	key = itr->first;
	if(key.find("true")==0)
	{ check(conf, key, true); }
	else if (key.find("false")==0)
	{ check(conf, key, false); }
	else if (key.find("int")==0)
	{ check(conf, key, (int)atoi(data[key].c_str())); }
	else if (key.find("long")==0)
	{ check(conf, key, (long)atol(data[key].c_str())); }
	else if (key.find("uint")==0)
	{ check(conf, key, (unsigned int)atol(data[key].c_str())); }
	else if (key.find("ulong")==0)
	{ check(conf, key, (unsigned long)strtoul(data[key].c_str(), (char**)NULL, 10)); }
	else if (key.find("float")==0)
	{ check(conf, key, (float)atof(data[key].c_str())); }
	else if (key.find("double")==0)
	{ check(conf, key, (double)strtod(data[key].c_str(), (char**)NULL)); }
	else if (key.find("string")==0)
	{ check(conf, key, data[key]); }
	itr++;
    }
    key = "/iguana/application/setup/vstring";
    std::vector<std::string> vec;
    conf->getValues(key, vec);
    pass=true;
    if (vec.size() == 5)
    {
      char ch = 'a';
      for(size_t i=0; i<vec.size();i++)
      {
	if((vec[i].size() != 1) || ((char)vec[i][0] != ch))
	{
	  pass=false;
	  break;
	}
	ch++;
      }
    }
    
    if(pass)
        std::cout <<"Checking "<<key<<" ..... PASS" <<std::endl;
    else
        std::cout <<"Checking "<<key<<" ..... FAIL" <<std::endl;
	

    key = "/iguana/application/setup/vstring_int";
    std::vector<int> veci;
    conf->getValues(key, veci);
    pass=true;
    if (veci.size() == 6)
    {
      int n = 1;
      for(size_t i=0; i<veci.size();i++)
      {
	if(veci[i] != n)
	{
	  pass=false;
	  break;
	}
	n++;
      }
    }
    
    if(pass)
        std::cout <<"Checking "<<key<<" ..... PASS" <<std::endl;
    else
        std::cout <<"Checking "<<key<<" ..... FAIL" <<std::endl;
	
    if(failed>0)
    {
      std::cout <<"FAILED: "<<failed<<" test(s) failed"<<std::endl;
      return 1;
    }
    return 0;
}


