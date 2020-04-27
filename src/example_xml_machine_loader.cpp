/**
 * @brief 
 * 
 * @file example_xml_machine_loader.cpp
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

#include <chrono> 
#include <ctime> 
#include <cognitao/CogniTao.h>
#include <cognitao/io/Environment.h>

shared_ptr<Machine> machineHandler;

using namespace std;
using namespace cognitao::data_sources;

 
/**
 * @brief Starts CogniTao server and WebServerApi
 * @note 
 * Machine xml file path must be provided as first argument
 *  
 * @par Usage example:
 * @code
 * cognitao_server ~/plans/behavior_tree.xml
 * @endcode
 *  
 */

int main(int argc, char* argv[]) {

    Environment::set("COGNITAO_DATASOURCE","udp",false);
    Environment::set("COGNITAO_IP","0.0.0.0",false);
    Environment::set("COGNITAO_PORT","1234",false);
    
    string logPath = "";
    if (argc < 2) {
        cout << "error: machine XML file path missing" << endl;
        return 1;
    }

    cout << endl;
    cout << "\033[1;33mCognitao server\033[0m" << endl;
    cout << endl;
    //
    // Load so/dll plugins (runners, data sources, etc...)
    //
    PluginLoader::loadPlugins();

    // 
    // 
    // 
    if(argc > 2)
    {    
        WorldModel::setDataSource(PluginLoader::getDataSource(argv[2]));
    }
    else
    {
        WorldModel::setDataSource(
            PluginLoader::getDataSource(
                Environment::get("COGNITAO_DATASOURCE")
                )
            );     
    }
    
    if(argc > 3)
    {    
        logPath = argv[3];
    }
   
    try {
        if(logPath != "")
        {
            WorldModelLog::init(logPath + ".log_wm");
            TaskLog::init(logPath + ".log_task");
        }
        machineHandler.reset(MachineXMLReader::read(argv[1]));

        //In order to monitor the machine
        MachineWebServer webServer(
            Environment::get("COGNITAO_IP"),
            Environment::get("COGNITAO_PORT"),
            machineHandler.get());

        auto startTime = chrono::system_clock::now(); 
        machineHandler->start();
  
        
        cout << " - machine: " << argv[1] << endl;
        cout << " - name: " << machineHandler->getName() << endl;
        cout << " - web ui: " << "\033[1;32mhttp://" 
                << Environment::get("COGNITAO_IP") << ":" 
                << Environment::get("COGNITAO_PORT") <<"\033[0m" << endl;
        cout << endl;

        cout << machineHandler->getName() << " is running..." << endl << endl;

        while (!machineHandler->isFinished() && 
               !machineHandler->isStopRequested() &&
               machineHandler->getState() != TaskState::error) {
            cout << MachineTraceWriter::writeExecutionTrace(machineHandler.get()) << endl;

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
        
        try
        {
            cout  << "Resetting DataSource (for ROS and other systems that require it)" <<endl;
            WorldModel::resetDataSource(); 
        }
        catch(...)
        {};   
        webServer.stop();   
        bool returnValue = machineHandler->getReturn() ? 0 : 1;
        machineHandler->stop(); 
        machineHandler.reset();
        return returnValue;

    } catch (std::runtime_error & e) {
        cerr <<  e.what() << endl;
        try
        {
            cout  << "Resetting DataSource (ROS)" <<endl;
            WorldModel::resetDataSource(); 
        }
        catch(...)
        {};  
        return 2;
    }

}
