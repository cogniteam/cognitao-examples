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

class SharedLibDummyRunner : public Runner{

	atomic<bool> stopRequested;
public:

	SharedLibDummyRunner() : Runner(){}

	virtual bool run(){
		stopRequested = false;
		cout << "IN SHARED LIBRARY RUNNER " << action_ << " SLEEP "  << endl;

		double msec = 2.0 * 1000;
		int idx = msec / 100;
		for(int i =0; i<idx && !stopRequested; i++)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
		return true;
	}
	virtual void stop() {
		stopRequested = true;
	}
	virtual std::string getType() { return "shared_dummy";};

};

// RUNNER PLUGIN INTERFACE 
// runners compiled as plugins (so or dll) can be load using 
// PluginLoader::loadPlugins();

extern "C" Runner* create_runner()
{
  return new SharedLibDummyRunner();
}

extern "C" void destroy_runner( Runner* object )
{
  delete object;
}

extern "C" const char* get_runner_type()
{
  return string("shared_dummy").c_str();
}


int main() {
	cout << "shared lib dummy runner example" << endl;
	return 0;
}
