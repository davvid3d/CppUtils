# Configuration tool

This class allow to use a dynamic configuration based on a map with string keys and different primitives values.

 *cant be used in embedded projects, std lib is mandatory* 

It also allow to read a file.

it can be used as following:

```cpp
genericTools::configuration config;

// to load a config file
bool validConfLoad = config.loadConfig(std::string("./confTest"));

// to add custom config
config["mybool"] = false;
config["myint"] = 11;
config["mydouble"] = 22.3333;
config["myCat"]["subCat"]["float"] = 44.55;

// to read config 
bool mybool = config["mybool"];
int myint = config["myint"];
double mydouble = config["mydouble"];
float myfloatsubcat = config["myCat"]["subCat"]["float"];
```