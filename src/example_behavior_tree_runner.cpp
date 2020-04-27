/**
 * @brief 
 * 
 * @file example_behavior_tree_runner.cpp
 * 
 * @author ari (ari@cogniteam.com)
 * @date 2020-04-21
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

	UserRunner(string action, std::map<std::string, std::string> parameters) : Runner(){
		this->setAction(action);
		this->setParameters(parameters);
	}

	virtual bool run() override {
		cout << "USER CODE THAT EXECUTES "<< getAction() <<" TASK REMOTELY START" << endl;
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
		cout << "USER CODE THAT EXECUTES "<< getAction() <<" TASK REMOTELY STOP" << endl;
		// return the remote task return value
		// here
		return true;
	}

	virtual void stop() override 
    {
		stopRequested = true;
	}

     /**
     * @brief Gets the Type, used in the XML in the form of
     *  <task name="name" runner="user_remote" param_time="3.0" />
     *  Runner parameters are in the form of param_<name>="value"
     * @return std::string 
     */
	virtual std::string getType() override
    { 
        return "user_remote";
    };

};

// BehaviourTaskFactoryMethod enables to register new FactoryMethod
// thus enabling to instantiate new BehaviuorTasks with runners created outside the CogniTAO library

class UserRunnerFactoryMethod : public RunnerFactoryMethod {
public:
	UserRunnerFactoryMethod(){}
	virtual ~UserRunnerFactoryMethod(){}

	virtual Runner::Ptr createRunner(
			std::string action,
			Task *t
	) {

		cout << "USER RUNNER CREATED" << endl;
		return Runner::Ptr(new UserRunner(action,t->getParameters()));
	}
};

int main(int argc, char* argv[])
{

	try
	{
	
		
		// Registering a runner factory method that uses an alternative runner
		// For example in the following tag in the XML
		// <task name="Report" runner="user-remote" action="wait" param_time="2.0" />
		RunnerFactory::registerFactoryMethod("user_remote",new UserRunnerFactoryMethod());

		if (argc < 2) {
			cout<<"-- MISSING XML FILE --"<<endl;
			return 1;
		}
		else{

        
			cout<< "Loading XML " << argv[1] << endl;

			Machine* machineHandler =  MachineXMLReader::read(argv[1]);

            auto startTime = chrono::system_clock::now(); 
            machineHandler->start();
           
            cout << " - machine: " << argv[1] << endl;
            cout << " - name: " << machineHandler->getName() << endl;
            cout << endl;

            while (!machineHandler->isFinished() &&
                machineHandler->getState() != TaskState::error) {
                cout << MachineTraceWriter::writeExecutionTrace(machineHandler) << endl;

                this_thread::sleep_for(chrono::milliseconds(100));
            }

            auto endTime = chrono::system_clock::now();
            chrono::duration<double> elapsedTime = endTime - startTime;


            if(machineHandler->getState() == TaskState::error)
            {
                cout << machineHandler->getName() 
                        << " finished with error, elapsed time: " 
                        << elapsedTime.count() << "s" << endl;
                cout << machineHandler->getExceptionMessage() <<endl;

            }
            else
            {      
                cout << machineHandler->getName() 
                        << " finished successfully, elapsed time: " 
                        << elapsedTime.count() << "s" << endl;
            }
        
            machineHandler->stop();
            delete machineHandler;
		}
	}
	catch (std::exception const& e) {
		std::cerr << "ERROR: " << e.what() << std::endl;
	}
	cout << "CogniTao Sever shutting down" <<endl;
	return 1;
}
