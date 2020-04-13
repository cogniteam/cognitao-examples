/**
 * @brief 
 * 
 * @file example_runner_plugin.cpp
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
#include <cognitao/CogniTao.h>
#include <cognitao/Runner.h>

using namespace std;

class SharedLibDummyRunner : public Runner {

public:

	SharedLibDummyRunner() : Runner() {

	}

    /**
     * @brief Here the actual logic for this runner should be implemented
     * @return true returned if the action logic succedded
     * @return false otherwise
     */
	virtual bool run() override {

		stopRequested_ = false;
		cout << "IN SHARED LIBRARY RUNNER " << getAction() << endl;
		cout << "PARAMETERS :" << endl;
		for(auto param : getParameters())
		{
			cout<< param.first << " " << param.second << endl;
		}
			
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		return true;
	}

    /**
     * @brief in Stop you should implement what should happen when the runner
     * is asked to be stopped
     */
	virtual void stop() override {
        // can be used to check in the run method if the runner run method needs interruption
		stopRequested_ = true;
	}

    /**
     * @brief Gets the Type , used to identify the runner by CogniTao
     * (for example when loading a plan from xml)
     * @return std::string 
     */
	virtual std::string getType() { 
		return "shared_dummy";
	};
	
public:

	atomic<bool> stopRequested_;
};

// RUNNER PLUGIN INTERFACE 
// runners compiled as plugins (so or dll) can be load using 
// PluginLoader::loadPlugins();

extern "C" Runner* create_runner()
{
  return new SharedLibDummyRunner();
}

extern "C" void destroy_runner(Runner* object)
{
  delete object;
}

extern "C" const char* get_runner_type()
{
  return "shared_dummy";
}

/**
 * @brief this main will not be used as this is an example of a shared object 
 * that implements a new runner and will be loaded at runtime by CogniTao
 * @return int 
 */
int main() {
	cout << "shared lib dummy runner example" << endl;
	return 0;
}
