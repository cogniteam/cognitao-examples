/**
 * @brief 
 * 
 * @file example_behavior_tree_user_action.cpp
 * 
 * @author ari (ari@cogniteam.com)
 * @date 2020-04-22
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

//////////////////////////////////////////////////////////////////
// Example For defining your own custom actions (used by a "local" runner)
//////////////////////////////////////////////////////////////////

class UserFunctions{
public:
	bool wait_forever(Task* b)
	{
        // get parameters written in the XML in the form of 
        // < .... param_<name>="value">
		cout << "PARAMETERS :" << endl;
		for(auto param : b->getParameters())
		{
			cout<< param.first << " " << param.second << endl;
		}

        
		while(!b->isStopRequested())
		{
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }	
		return true;
	}

    bool wait_random(Task* b)
	{
        // get parameters written in the XML in the form of 
        // < .... param_<name>="value">
		cout << "PARAMETERS :" << endl;
		for(auto param : b->getParameters())
		{
			cout<< param.first << " " << param.second << endl;
		}

        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		srand(seed);
		int rnd = (rand() % 9) + 1;
		WorldModel::setValue("random",rnd);
        for(int i=0 ; (i < rnd) && !b->isStopRequested(); i++)
		{
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
		return true;
	}

    bool wait_and_print(Task* b)
	{
        // get parameters written in the XML in the form of 
        // < .... param_<name>="value">
		cout << "PARAMETERS :" << endl;
		for(auto param : b->getParameters())
		{
			cout<< param.first << " " << param.second << endl;
		}

        for(int i=0 ; i < 10 && !b->isStopRequested(); i++)
		{
            std::this_thread::sleep_for(std::chrono::seconds(1));
            cout << b->getParameters()["input"] << endl;
        }
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
		// <task name="Report" runner="local" action="wait_random"/>
        // <task name="Report" runner="local" action="wait_and_print" param_input="HELLO WORLD!"/>
		Action::registerAction("wait_forever", &UserFunctions::wait_forever, userFunctions);
		Action::registerAction("wait_random", &UserFunctions::wait_random, userFunctions);
        Action::registerAction("wait_and_print", &UserFunctions::wait_and_print, userFunctions);
        
		
		cout<< "Loading XML " << argv[1] << endl;
		Machine* machineHandler =  MachineXMLReader::read(argv[1]);

        // used to view you machine state in the browser
        MachineWebServer w("0.0.0.0", "1234", machineHandler);
        machineHandler->start();

        while (!machineHandler->isFinished()) 
        {
            // Print Execution State
            cout<<MachineTraceWriter::writeExecutionTrace(machineHandler) <<endl;	
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        if(machineHandler->getState() == TaskState::error)
        {
            std::cout << "\n\nERROR IN MACHINE : TRACE" << std::endl;
            cout<<MachineTraceWriter::writeExecutionTrace(machineHandler) << "\n\n" <<endl;
        }

        bool bRet = machineHandler->getReturn();

        cout << "Machine ended with "<< bRet << endl;
        machineHandler->stop();
        delete machineHandler;
	
	}
	catch (CogniTAOException& e) {
		std::cerr << e.what() << std::endl;
	}

    // Print WM
    cout<< "WORLD MODEL" <<endl;
    cout<<WorldModel::serializeJson() << endl;    
            
	cout << "CogniTao Sever shutting down" <<endl;
	return 1;
}
