/**
 * @brief 
 * 
 * @file example_state_machine.cpp
 * 
 * @author your name (you@domain.com)
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

#include <cognitao/CogniTao.h>


int main(int argc, char* argv[]) {


	try
	{
        std::ostringstream strs;
        std::map<std::string,std::string> params;
        auto s1 = new StateEmpty("locked",params);
        auto s2 = new StateEmpty("unlocked",params);

        MachineFSM *machineHandler = new MachineFSM();
        auto E1 = new ProtocolTransition ({"PUSH"});
        auto E2 = new ProtocolTransition ({"COIN"});
        machineHandler->setInitialTask(s1);
        s1->addEdge(s2,E2);
        s2->addEdge(s1,E1);
        
        machineHandler->start();
        // the machine start is async
        // waiting a second to make shure the machine started
        // so that we will not miss the first events
        std::this_thread::sleep_for(std::chrono::seconds(1));

        int i =0;
        while(i<1000){
                if(i%2){
                    // this will use the machine events subscription
                    // through the WM event manager 
                    WorldModel::setVar("PUSH", "event"); 
                    // this will use the machine direct events subscription
                    //machineHandler->raiseEvent("PUSH");
                }

                else{ 
                    // this will use the machine events subscription
                    // through the WM event manager 
                    WorldModel::setVar("COIN", "event");
                    // this will use the machine direct events subscription
                    //machineHandler->raiseEvent("PUSH");
                }
                i++;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
        cout<< "STATE LOCKED VISITED " << s1->getVisited() <<endl;
        cout<< "STATE UNLOCKED VISITED " << s2->getVisited() <<endl;
        machineHandler->stop();
        delete machineHandler; 
	}
	catch (std::exception const& e) {
		std::cerr << "ERROR: " << e.what() << std::endl;
	}
	return 1;
}
