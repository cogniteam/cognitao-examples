
//============================================================================
// Name        : example_state_machine.cpp
// Author      : Cogniteam
// Version     :
// Copyright   : MIT
// Description :
//============================================================================

#include <cognitao/CogniTao.h>


int main(int argc, char* argv[]) {

	try
	{
		std::ostringstream strs;
		std::map<std::string,std::string> params;
		auto s1 = new StateEmpty("locked",params);
		auto s2 = new StateEmpty("unlocked",params);

		Machine *m = new MachineFSM();
		auto E1 = new ProtocolTransition ({"PUSH"});
		auto E2 = new ProtocolTransition ({"COIN"});
		auto E3 = new ProtocolTransition ({"PUSH"});
		auto E4 = new ProtocolTransition ({"COIN"});
		m->setInitialTask(s1);
		s1->addEdge(s2,E2);
		s2->addEdge(s1,E1);
		s1->addEdge(s1,E3);
		s2->addEdge(s2,E4);
		m->start();
		std::this_thread::sleep_for(std::chrono::seconds(2));
		bool lockCoinEvent_,unLockCoinEvent_,unLockPushEvent_,lockPushEvent_;


		cout<<MachineStringWriter::writeExecutionTrace(m) <<endl;
		WorldModel::setVar("COIN","event");
		cout << "->coin" <<endl;
		cout<<MachineStringWriter::writeExecutionTrace(m) <<endl;

		(m->getCurrentState()->getName().compare("unlocked")==0)?
						lockCoinEvent_=true:lockCoinEvent_=false;

		std::this_thread::sleep_for(std::chrono::seconds(2));

		cout<<MachineStringWriter::writeExecutionTrace(m) <<endl;
		WorldModel::setVar("COIN","event");
		cout << "->coin" <<endl;
		cout<<MachineStringWriter::writeExecutionTrace(m) <<endl;

		(m->getCurrentState()->getName().compare("unlocked")==0)?
						unLockCoinEvent_=true:unLockCoinEvent_=false;
		std::this_thread::sleep_for(std::chrono::seconds(2));

		cout<<MachineStringWriter::writeExecutionTrace(m) <<endl;
		WorldModel::setVar("PUSH","event");
		cout << "->push" <<endl;
		cout<<MachineStringWriter::writeExecutionTrace(m) <<endl;

		(m->getCurrentState()->getName().compare("locked")==0)?
						unLockPushEvent_=true:unLockPushEvent_=false;
		std::this_thread::sleep_for(std::chrono::seconds(2));

		cout<<MachineStringWriter::writeExecutionTrace(m) <<endl;
		WorldModel::setVar("PUSH","event");
		cout << "->push" <<endl;
		cout<<MachineStringWriter::writeExecutionTrace(m) <<endl;

		(m->getCurrentState()->getName().compare("locked")==0)?
						lockPushEvent_=true:lockPushEvent_=false;
		std::this_thread::sleep_for(std::chrono::seconds(2));

		cout<<MachineStringWriter::writeExecutionTrace(m) <<endl;
		WorldModel::setVar("COIN","event");
		cout << "->coin" <<endl;
		cout<<MachineStringWriter::writeExecutionTrace(m) <<endl;

		std::this_thread::sleep_for(std::chrono::seconds(2));
		(m->getCurrentState()->getName().compare("unlocked")==0)?
						lockCoinEvent_=true:lockCoinEvent_=false;


		(lockCoinEvent_&&lockPushEvent_&&unLockCoinEvent_&&unLockPushEvent_)?
						strs << __func__ << " PASSED ": strs << __func__ << " FAILED ";

		std::cout<<strs.str()<<endl;
		m->stop();
		delete m;
	}
	catch (std::exception const& e) {
		std::cerr << "ERROR: " << e.what() << std::endl;
	}

	return 1;
}
