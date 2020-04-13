/**
 * @brief 
 * 
 * @file example_behavior_tree_construction.cpp
 * 
 * @author your name (you@domain.com)
 * @date 2020-04-23
 * @copyright Cogniteam (c) 2020
 *    
 * MIT License
 *   
 * Permission is hereby granted, free of charge, to any person obtaining a  copy
 * of this software and associated documentation files (the 'Software'), to deal
 * in the Software without restriction, including without  limitation the rights
 * to use, copy, modify, merge,  publish,  distribute,  sublicense,  and/or sell
 * copies of the Software, and  to  permit  persons  to  whom  the  Software  is 
 * furnished to do so, subject to the following conditions:
 *   
 * The   above   copyright   notice   and  this   permission   notice  shall  be
 * included in all copies or substantial portions of the Software.
 *   
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY  KIND,  EXPRESS  OR
 * IMPLIED, INCLUDING BUT NOT LIMITED  TO  THE  WARRANTIES  OF  MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN  NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE  LIABLE  FOR  ANY  CLAIM,  DAMAGES  OR  OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 */

#include <cognitao/CogniTao.h>

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
    return buf;
}

int main(int argc, char* argv[]) {
    try
	{
    
        Machine* machineHandler;
    
        string sequencer_name = "->";
        string selector_name = "?";
        string loop_name = "@";
        
        
        // Create a behaviour tree machine 
        machineHandler = new MachineBT();

        //Declare nodes 
        auto loop =     new BehaviourLoop(loop_name,1);
        auto sequencer = new BehaviourSequencer(sequencer_name); // runs childs in sequence
        auto selector = new BehaviourSelector(selector_name); // runs childs in sequence
        
        auto b1 = new BehaviourSucceder("Ok"); // returns true
        auto b2 = new BehaviourSucceder("Ok"); // returns true
        auto b3 = new BehaviourFailer("NOk"); // returns false
        auto b4 = new BehaviourSucceder("Ok"); // returns true

        auto time = new BehaviourSet("TIME","Now is the time"); // sets X to the value of the WM::time
        
        auto wait1 = new BehaviourWait("waiting 1...",2); // waits 2 seconds
        auto wait2 = new BehaviourWait("waiting 2...",2); // waits 2 seconds
        auto wait3 = new BehaviourWait("waiting 3...",2); // waits 2 seconds
        auto wait4 = new BehaviourWait("waiting 4...",2); // waits 2 seconds

         //Build tree
        loop->addEdge(sequencer);
        //SEQUENCERS will coontinue while tasks succeed
        sequencer->addEdge(b1);
        sequencer->addEdge(wait1);
        sequencer->addEdge(b2);
        sequencer->addEdge(wait2);
        sequencer->addEdge(time);
        sequencer->addEdge(selector);
        //SELECTOR will stop after wait3 succeeds
        selector->addEdge(b3);
        selector->addEdge(wait3);
        selector->addEdge(b4);
        selector->addEdge(wait4);
        
       
        
        //Set the machine initial task
        machineHandler->setInitialTask(loop);

        machineHandler->start();

        //While the machine has not returned
        while (!machineHandler->isFinished())
        {  
             // Print Execution State
            cout<<MachineTraceWriter::writeExecutionTrace(machineHandler) <<endl;
            //Sleep
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        // Serialize machine to JSON
        machineHandler->stop();

        if(machineHandler!=nullptr)
            delete machineHandler;   

    }
	catch (std::exception const& e) {
		std::cerr << "ERROR: " << e.what() << std::endl;
	}

	return 1;
}
