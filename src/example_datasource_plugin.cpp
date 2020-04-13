//============================================================================
// Name        : example_runner_plugin.cpp
// Author      : Cogniteam
// Version     :
// Copyright   : MIT
// Description :
//============================================================================

#include <iostream>

#include <cognitao/CogniTao.h>
#include <cognitao/Runner.h>


using namespace std;


class SharedLibraryPrinterDataSource  : public DataSource {

public:
	
	virtual ~SharedLibraryPrinterDataSource() {

    }

	virtual bool publishUpdateEvent(const string& name, const string& value){
		// send to datasources
		return true;
	};

	void onDataSourceEvent()
	{
		//call variableUpdated(const string& name, const string& value);
	}
};

// RUNNER PLUGIN INTERFACE 
// runners compiled as plugins (so or dll) can be load using 
// PluginLoader::loadPlugins();

extern "C" DataSource* create_datasource() {

  return new SharedLibraryPrinterDataSource();
}

extern "C" void destroy_datasource(DataSource* object) {

  delete object;
}

extern "C" const char* get_datasource_type() {
    
  return "shared_datasource";
}


int main() {
	cout << "shared lib dummy runner example" << endl;
	return 0;
}
