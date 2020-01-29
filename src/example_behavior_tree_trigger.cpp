
//============================================================================
// Name        : example_behavior_tree_trigger.cpp
// Author      : Cogniteam
// Version     :
// Copyright   : MIT
// Description :
//============================================================================

#include <cognitao/CogniTao.h>


int main(int argc, char* argv[]) {

	try
	{
		// init WM interface
		WM::init(new MapThreadSafeDataSource());

        Machine *m = new MachineBT();
        
        // trigger will stop the subtree with success once condition in met
        auto trigger = new BehaviourTrigger("event","=","true");
        
        // wait an hour
        auto wait = new BehaviourWait("waiting",3600);

        trigger->addEdge(wait);
       
        m->setInitialTask(trigger);

        MachineWebServer w("127.0.0.1", "1234", m);

        // start the machine
        m->start();
        
        std::this_thread::sleep_for(std::chrono::seconds(4));

        // trigger the event
        WM::setVar("event","true");
        
        // wait for the tree to end
        while (!m->isFinished())
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        m->stop();
		delete m;
    
	}
	catch (std::exception const& e) {
		std::cerr << "ERROR: " << e.what() << std::endl;
	}
	return 1;
}
