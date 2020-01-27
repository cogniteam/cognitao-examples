
#include <cognitao/CogniTao.h>


int main(int argc, char* argv[]) {

	try
	{
		// init WM interface
		WM::init(new MapThreadSafeDataSource());

        Machine* m = new MachineBT();

        auto loop = new BehaviourLoop("loop",2);
        auto sequence = new BehaviourSequencer("->");
        auto b1 = new BehaviourSucceder("B1");
        auto b2 = new BehaviourSucceder("B2");
        auto wait1 = new BehaviourWait("waiting",2);
        auto wait2 = new BehaviourWait("waiting",2);

        loop->addEdge(sequence);
       
        sequence->addEdge(b1);
        sequence->addEdge(wait1);
        sequence->addEdge(b2);
        sequence->addEdge(wait2);
        
        m->setInitialTask(loop);

        MachineWebServer w("127.0.0.1", "1234", m);
        m->start();

        while (!m->isFinished())
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    
	}
	catch (std::exception const& e) {
		std::cerr << "ERROR: " << e.what() << std::endl;
	}
	return 1;
}
