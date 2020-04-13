/**
 * @brief 
 * 
 * @file example_state_machine_turnstile.cpp
 * 
 * @author lin (lin@cogniteam.com)
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
		std::ostringstream strs;
		std::map<std::string,std::string> params;
		auto s1 = new StateEmpty("locked",params);
		auto s2 = new StateEmpty("unlocked",params);

		MachineFSM *machine = new MachineFSM();
		auto E1 = new ProtocolTransition ({"PUSH"});
		auto E2 = new ProtocolTransition ({"COIN"});
		auto E3 = new ProtocolTransition ({"PUSH"});
		auto E4 = new ProtocolTransition ({"COIN"});
		machine->setInitialTask(s1);
		s1->addEdge(s2,E2);
		s2->addEdge(s1,E1);
		s1->addEdge(s1,E3);
		s2->addEdge(s2,E4);
		machine->start();

		bool lockCoinEvent_,unLockCoinEvent_,unLockPushEvent_,lockPushEvent_;

		cout<<MachineTraceWriter::writeExecutionTrace(machine) <<endl;
		machine->raiseEvent("COIN");
		cout<<MachineTraceWriter::writeExecutionTrace(machine) <<endl;

		(machine->getCurrentState()->getName().compare("unlocked")==0)?
						lockCoinEvent_=true:lockCoinEvent_=false;

		cout<<MachineTraceWriter::writeExecutionTrace(machine) <<endl;
		machine->raiseEvent("COIN");
		cout<<MachineTraceWriter::writeExecutionTrace(machine) <<endl;

		(machine->getCurrentState()->getName().compare("unlocked")==0)?
						unLockCoinEvent_=true:unLockCoinEvent_=false;
		
		cout<<MachineTraceWriter::writeExecutionTrace(machine) <<endl;
		machine->raiseEvent("PUSH");
		cout<<MachineTraceWriter::writeExecutionTrace(machine) <<endl;

		(machine->getCurrentState()->getName().compare("locked")==0)?
						unLockPushEvent_=true:unLockPushEvent_=false;
		
		cout<<MachineTraceWriter::writeExecutionTrace(machine) <<endl;
		machine->raiseEvent("PUSH");
		cout<<MachineTraceWriter::writeExecutionTrace(machine) <<endl;

		(machine->getCurrentState()->getName().compare("locked")==0)?
						lockPushEvent_=true:lockPushEvent_=false;
		
		cout<<MachineTraceWriter::writeExecutionTrace(machine) <<endl;
		machine->raiseEvent("COIN");
		cout<<MachineTraceWriter::writeExecutionTrace(machine) <<endl;

		(machine->getCurrentState()->getName().compare("unlocked")==0)?
						lockCoinEvent_=true:lockCoinEvent_=false;


		(lockCoinEvent_&&lockPushEvent_&&unLockCoinEvent_&&unLockPushEvent_)?
						strs << __func__ << " PASSED ": strs << __func__ << " FAILED ";

		std::cout<<strs.str()<<endl;
		machine->stop();
		delete machine;
	}
	catch (std::exception const& e) {
		std::cerr << "ERROR: " << e.what() << std::endl;
	}

	return 1;
}
