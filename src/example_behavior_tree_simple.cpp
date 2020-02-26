//============================================================================
// Name        : example_behavior_tree_simple.cpp
// Author      : Cogniteam
// Version     :
// Copyright   : MIT
// Description :
//============================================================================

#include <cognitao/CogniTao.h>


int main(int argc, char* argv[]) {

	try
	{
        // Create a behaviour tree machine 
        Machine* m = new MachineBT();

        //Declare nodes 
        auto loop = new BehaviourLoop("loop",2);
        auto sequence = new BehaviourSequencer("->"); // runs childs in sequence
        auto b1 = new BehaviourSucceder("B1"); // returns true
        auto b2 = new BehaviourSucceder("B2"); // returns true
        auto b3 = new BehaviourSet("X","$time"); // sets X to the value of the WM::time
        auto wait1 = new BehaviourWait("waiting",2); // waits 2 seconds
        auto wait2 = new BehaviourWait("waiting",2); // waits 2 seconds

        //Build tree
        loop->addEdge(sequence);
       
        sequence->addEdge(b1);
        sequence->addEdge(wait1);
        sequence->addEdge(b2);
        sequence->addEdge(wait2);
        sequence->addEdge(b3);
        
        //Set the machine initial task
        m->setInitialTask(loop);

        //Create MachineWebServer to visually monitor the machine 
        MachineWebServer w("0.0.0.0", "1234", m);
        m->start();

        //While the machine has not returned
        while (!m->isFinished())
        {
            // Set WM::time 
            std::time_t t = std::time(nullptr);
            char mbstr[100];
            if(std::strftime(mbstr, sizeof(mbstr), "%A %c", std::localtime(&t)))
                WorldModel::setVar("time",std::string(mbstr)); // setting the current time in WM::time
            
            // Print WM
            cout<<WorldModel::serializeJson();    
            // Print Execution State
            cout<<MachineStringWriter::writeExecutionTrace(m) <<endl;
            //Sleep
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        // Serialize machine to JSON
        cout<<MachineJSONWriter::toString(m) <<endl;
        m->stop();
        delete m;
	}
	catch (std::exception const& e) {
		std::cerr << "ERROR: " << e.what() << std::endl;
	}
	return 1;
}
