/**
 * @brief 
 * 
 * @file example_behavior_tree_return_exceptions.cpp
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

class BehaviourException : public Behaviour{
public:
    BehaviourException(std::string name) : Behaviour(name){}
    
    virtual bool action(std::map<std::string, std::string> params) override
    {
        throw CogniTAOException("TESTING EXCEPTION");
    }
};

void printFinishMessage(Machine* machineHandler){

    if(machineHandler->getState() == TaskState::error)
    {
        cout << machineHandler->getName() 
                << " finished with error " << endl;
        cout << machineHandler->getExceptionMessage() <<endl;

    }
    else
    {      
        cout << machineHandler->getName() 
                << " finished with " << "return (" <<machineHandler->getReturn() << ")" <<  endl;
    }

}

Machine* machine1(){
    // Create a behaviour tree machine 
   Machine* machineHandler = new MachineBT();
    //Declare nodes 
    auto loop =     new BehaviourLoop("@",1);
    auto b1 = new BehaviourSucceder("Ok"); // returns true
    //Build tree
    loop->addEdge(b1);
    //Set the machine initial task
    machineHandler->setInitialTask(loop);
    return machineHandler;    
}

Machine* machine2(){
    // Create a behaviour tree machine 
   Machine* machineHandler = new MachineBT();
    //Declare nodes 
    auto sequencer =     new BehaviourSequencer("->");
    auto b1 = new BehaviourFailer("NOk"); // returns true
    //Build tree
    sequencer->addEdge(b1);
    //Set the machine initial task
    machineHandler->setInitialTask(sequencer);
    return machineHandler;    
}

Machine* machine3(){
    // Create a behaviour tree machine 
   Machine* machineHandler = new MachineBT();
    //Declare nodes 
    auto sequencer =     new BehaviourSequencer("->");
    //Build tree
    sequencer->addEdge(new BehaviourException("BehaviourException"));
    //Set the machine initial task
    machineHandler->setInitialTask(sequencer);
    return machineHandler;    
}
int main(int argc, char* argv[]) {
    try
	{
    
        Machine* machineHandler;
        
        machineHandler = machine1();
        machineHandler->start();
        std::this_thread::sleep_for(std::chrono::seconds(2));
        machineHandler->stop();
        printFinishMessage(machineHandler);
        if(machineHandler!=nullptr)
            delete machineHandler;

        machineHandler = machine2();
        machineHandler->start();
        std::this_thread::sleep_for(std::chrono::seconds(2));
        machineHandler->stop();
        printFinishMessage(machineHandler);
        if(machineHandler!=nullptr)
            delete machineHandler;   

        machineHandler = machine3();
        machineHandler->start();
        std::this_thread::sleep_for(std::chrono::seconds(2));
        machineHandler->stop();
        printFinishMessage(machineHandler);
        if(machineHandler!=nullptr)
            delete machineHandler;   

    }
	catch (std::exception const& e) {
		std::cerr << "ERROR: " << e.what() << std::endl;
	}

	return 1;
}
