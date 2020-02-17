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

class SharedLibDummyRunner : public Runner {

public:

	SharedLibDummyRunner() : Runner() {

	}

	virtual bool run() {

		stopRequested_ = false;
		cout << "IN SHARED LIBRARY RUNNER " << action_ << " SLEEP "  << endl;

		double msec = 2.0 * 1000;

		int idx = msec / 100;

		for(int i =0; i<idx && !stopRequested_; i++) {
			
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		// cerr << getDataSource()->getVar("DistanceAA") << endl;

		cerr << "DistanceAA = " << WM::getVar("DistanceAA") << endl;
		
		return true;
	}

	virtual void stop() {
		stopRequested_ = true;
	}

	virtual std::string getType() { 
		return "shared_dummy";
	};
	
public:

	atomic<bool> stopRequested_;
};

// RUNNER PLUGIN INTERFACE 
// runners compiled as plugins (so or dll) can be load using 
// PluginLoader::loadPlugins();

extern "C" Runner* create_runner()
{
  return new SharedLibDummyRunner();
}

extern "C" void destroy_runner(Runner* object)
{
  delete object;
}

extern "C" const char* get_runner_type()
{
  return "shared_dummy";
}


int main() {
	cout << "shared lib dummy runner example" << endl;
	return 0;
}
