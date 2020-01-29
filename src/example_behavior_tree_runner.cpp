//============================================================================
// Name        : example_behavior_tree_runner
// Author      : CogniTeam
// Version     : 
// Description : 
//============================================================================


#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <cognitao/CogniTao.h>


//////////////////////////////////////////////////////////////////
// Example For defining your own custom Runner (ROS1, ROS2 etc...)
//////////////////////////////////////////////////////////////////

// Runners are used in BehaviourTask instances
// * To implement action logic execution (local/remote implementation of tasks)
// Runners can be used to interface any system of execution
// * Needs to respond to stop requests
// * Has action_ and parameters_ member data to send the information to the remote logic

class UserRunner : public Runner{

	atomic<bool> stopRequested;
public:

	UserRunner(string action, std::map<std::string, std::string> parameters) : Runner(action,parameters){}

	virtual bool run(){
		cout << "USER CODE THAT EXECUTES "<< action_ <<" TASK REMOTELY START" << endl;
		// use your runners api to START a remote task
		// here
		int i = 3;
		while(i) // while the remote task has not finished
		{
			if(stopRequested)
			{
				// use your runners api to stop the remote execution
				// here
			}
			std::this_thread::sleep_for(std::chrono::seconds(1));
			i--;
		}
		cout << "USER CODE THAT EXECUTES "<< action_ <<" TASK REMOTELY STOP" << endl;
		// return the remote task return value
		// here
		return true;
	}
	virtual void stop() {
		stopRequested = true;
	}
	virtual std::string getType() { return "user_remote";};

};

// BehaviourTaskFactoryMethod enables to register new FactoryMethod
// thus enabling to instantiate new BehaviuorTasks with runners created outside the CogniTAO library

class UserRunnerFactoryMethod : public RunnerFactoryMethod {
public:
	UserRunnerFactoryMethod(){}
	virtual ~UserRunnerFactoryMethod(){}

	virtual Runner* createRunner(
			std::string action,
			Task *t
	) {

		Runner *tRet = nullptr;
		tRet= new UserRunner(action,t->getParameters());
		cout << "USER RUNNER CREATED" << endl;
		return tRet;
	}
};

int main(int argc, char* argv[])
{

	try
	{
	
		// init WM interface
		WM::init(new MapThreadSafeDataSource());

		// Registering a runner factory method that uses an alternative runner
		// For example in the following tag in the XML
		// <task name="Report" runner="user-remote" action="wait" param_time="2.0" />
		RunnerFactory::registerFactoryMethod("user_remote",new UserRunnerFactoryMethod());

		if (argc < 2) {
			cout<<"-- MISSING XML FILE --"<<endl;
			return 1;
		}
		else{
			
			//initialize World Model
			WM::init(new MapThreadSafeDataSource());



			cout<< "Loading XML " << argv[1] << endl;
			Machine* m =  MachineXMLReader::read(argv[1]);


			MachineWebServer w("127.0.0.1", "1234", m);
			m->start();

			while (true) // can be !m->isFinished() for one time run
			{
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
