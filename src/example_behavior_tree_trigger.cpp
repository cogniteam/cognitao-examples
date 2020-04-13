/**
 * @brief 
 * 
 * @file example_behavior_tree_trigger.cpp
 * 
 * @author ari (ari@cogniteam.com)
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


int main(int argc, char* argv[]) {

	try
	{
		Machine *machineHandler = new MachineBT();
        
        // trigger will stop the subtree with success once condition in met
        auto trigger = new BehaviourTrigger("event","=","true");
        
        // wait an hour
        auto wait = new BehaviourWait("waiting",3600);

        trigger->addEdge(wait);
       
        machineHandler->setInitialTask(trigger);

        // start the machine
        machineHandler->start();
        
        std::this_thread::sleep_for(std::chrono::seconds(4));

        cout<<MachineTraceWriter::writeExecutionTrace(machineHandler) <<endl;
        
        // trigger the event
        WorldModel::setVar("event","true");

        cout<<MachineTraceWriter::writeExecutionTrace(machineHandler) <<endl;
        // wait for the tree to end
        while (!machineHandler->isFinished())
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        cout<<MachineTraceWriter::writeExecutionTrace(machineHandler) <<endl;
        machineHandler->stop();
		delete machineHandler;
    
	}
	catch (std::exception const& e) {
		std::cerr << "ERROR: " << e.what() << std::endl;
	}
	return 1;
}
