/**
 * @brief 
 * 
 * @file example_datasource_plugin.cpp
 * 
 * @author ari (ari@cogniteam.com)
 * @date 2020-04-27
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


class SharedLibraryPrinterDataSource  : public DataSource {

public:
	
	virtual ~SharedLibraryPrinterDataSource() {

    }

	virtual bool publishUpdateEvent(const string& name, const string& value){
		// send to datasources
		return true;
	};

	void onDataSourceEvent()
	{
		//call variableUpdated(const string& name, const string& value);
	}
};

// RUNNER PLUGIN INTERFACE 
// runners compiled as plugins (so or dll) can be load using 
// PluginLoader::loadPlugins();

extern "C" DataSource* create_datasource() {

  return new SharedLibraryPrinterDataSource();
}

extern "C" void destroy_datasource(DataSource* object) {

  delete object;
}

extern "C" const char* get_datasource_type() {
    
  return "shared_datasource";
}


int main() {
	cout << "shared lib dummy runner example" << endl;
	return 0;
}
