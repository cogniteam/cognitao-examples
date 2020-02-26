//============================================================================
// Name        : example_behavior_tree_user_action.cpp
// Author      : Cogniteam
// Version     :
// Copyright   : MIT
// Description :
//============================================================================

#include <cognitao/CogniTao.h>

//////////////////////////////////////////////////////////////////
// Example For defining your own custom actions (used by a "local" runner)
//////////////////////////////////////////////////////////////////

class UserFunctions{
public:
	bool wait(Task* b)
	{
		while(!b->isStopRequested())
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		return true;
	}

	bool generateRandom(Task* b)
	{
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		srand(seed);
		int rnd = (rand() % 9) + 1;
		WorldModel::setVar("random",StringUtils::itos(rnd));
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		return true;
	}

	bool loop10(Task* b)
	{
		for(int i = 0; i<10 ; i++)
		{
			WorldModel::setVar("loop",StringUtils::itos(i));
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}
		return true;
	}

	bool waitRandom(Task* b)
	{
		std::string rnd = WorldModel::getVar("random");
		std::this_thread::sleep_for(std::chrono::seconds(stoi(rnd)));
		//std::this_thread::sleep_for(std::chrono::seconds(stoi("hello")));
		return true;
	}
};


int main(int argc, char* argv[])
{

	try
	{
		// instance of functions class;
		auto userFunctions = new UserFunctions();

		
		// Registering BehaviourTask actions defined by the user
		// These will be called when runner="local" and action="some_method"
		// defined in UserFunctions::some_method
		// For example in the following tag in the XML
		// <task name="Report" runner="local" action="wait" param_time="2.0" />
		Action::registerAction("wait", &UserFunctions::wait, userFunctions);
		Action::registerAction("generateRandom", &UserFunctions::generateRandom, userFunctions);
		Action::registerAction("loop10", &UserFunctions::loop10, userFunctions);
		Action::registerAction("waitRandom", &UserFunctions::waitRandom, userFunctions);

		if (argc < 2) {
			cout<<"-- MISSING XML FILE --"<<endl;
			return 1;
		}
		else{
			
			cout<< "Loading XML " << argv[1] << endl;
			Machine* m =  MachineXMLReader::read(argv[1]);


			MachineWebServer w("0.0.0.0", "1234", m);
			m->start();

			while (true) // can be !m->isFinished() for one time run
			{
				// Print WM
				cout<<WorldModel::serializeJson();    
            	// Print Execution State
            	cout<<MachineStringWriter::writeExecutionTrace(m) <<endl;	
				
				std::this_thread::sleep_for(std::chrono::seconds(1));
			}

			m->stop();
			delete m;
		}
	}
	catch (std::exception const& e) {
		std::cerr << "ERROR: " << e.what() << std::endl;
	}
	cout << "CogniTao Sever shutting down" <<endl;
	return 1;
}
