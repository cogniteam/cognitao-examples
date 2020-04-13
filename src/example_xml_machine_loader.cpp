//============================================================================
// Name        : example_xml_machine_loader.cpp
// Author      : Cogniteam
// Version     :
// Copyright   : MIT
// Description :
//============================================================================

#include <cognitao/CogniTao.h>


int main(int argc, char* argv[]) {

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
        WorldModel::setDataSource( PluginLoader::getDataSource(argv[2]));
    }
    else
    {
         WorldModel::setDataSource( PluginLoader::getDataSource("udp"));
    }
    
    if(argc > 3)
    {    
        logPath = argv[3];
    }
    
    // cout<< "Loading machine " << argv[1] << endl;

    try {
        if(logPath != "")
        {
            WorldModelLog::init(logPath + ".log_wm");
            TaskLog::init(logPath + ".log_task");
        }
        shared_ptr<Machine> machine(MachineXMLReader::read(argv[1]));

        MachineWebServer webServer("0.0.0.0", "1234", machine.get());

        auto startTime = chrono::system_clock::now(); 
        machine->start();
  
        
        cout << " - machine: " << argv[1] << endl;
        cout << " - name: " << machine->getName() << endl;
        cout << " - web ui: " << "\033[1;32mhttp://" << "127.0.0.1" << ":" << "1234\033[0m" << endl;
        cout << endl;

        cout << machine->getName() << " is running..." << endl << endl;

        while (!machine->isFinished() && 
               !machine->isStopRequested() &&
               machine->getState() != TaskState::error) {
            cout << MachineTraceWriter::writeExecutionTrace(machine.get()) << endl;

            this_thread::sleep_for(chrono::milliseconds(100));
        }

        auto endTime = chrono::system_clock::now();
        chrono::duration<double> elapsedTime = endTime - startTime;

        if(machine->getState() == TaskState::error)
        {
            cout << machine->getName() 
                    << " finished with error, elapsed time: " 
                    << elapsedTime.count() << "s" << endl;
            cout << machine->getExceptionMessage() <<endl;

        }
        else
        {      
            cout << machine->getName() 
                    << " finished without error, elapsed time: " 
                    << elapsedTime.count() << "s return (" <<machine->getReturn() << ")" <<  endl;
        }

        try
        {
            cout  << "Resetting DataSource (ROS)" <<endl;
            WorldModel::resetDataSource(); 
        }
        catch(...)
        {};    
        
        return machine->getReturn() ? 0 : 1;

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
