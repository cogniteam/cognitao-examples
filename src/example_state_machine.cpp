
#include <cognitao/CogniTao.h>


int main(int argc, char* argv[]) {

	try
	{

                WM::init(new MapThreadSafeDataSource());

                std::ostringstream strs;
                std::map<std::string,std::string> params;
                auto s1 = new StateEmpty("locked",params);
                auto s2 = new StateEmpty("unlocked",params);

                MachineFSM m;
                auto E1 = new ProtocolTransition ({"PUSH"});
                auto E2 = new ProtocolTransition ({"COIN"});
                m.setInitialTask(s1);
                s1->addEdge(s2,E2);
                s2->addEdge(s1,E1);
                m.start();
                std::this_thread::sleep_for(std::chrono::seconds(2));

                auto start = chrono::steady_clock::now();

                int i =0;
                while(i<1000){
                        //Sleeping here will block on the current thread
                        //and add the time to the overall time
                        //std::this_thread::sleep_for(std::chrono::milliseconds(10));
                        if(i%2){
                                WM::setVar("PUSH","event");
                        }
                        else{
                                WM::setVar("COIN","event");
                        }
                        i++;
                }
                auto end = chrono::steady_clock::now();

                m.stop();
                cout << "BENCH_STATE_CHANGE Elapsed time in nanoseconds : "
                        << chrono::duration_cast<chrono::nanoseconds>(end - start).count()
                        << " ns" << endl;

                cout << "BENCH_STATE_CHANGE Elapsed time in microseconds : "
                        << chrono::duration_cast<chrono::microseconds>(end - start).count()
                        << " �s" << endl;

                strs << "BENCH_STATE_CHANGE Elapsed time in milliseconds : "
                        << chrono::duration_cast<chrono::milliseconds>(end - start).count()
                        << " ms" << endl;

                cout << "BENCH_STATE_CHANGE Elapsed time in seconds : "
                        << chrono::duration_cast<chrono::seconds>(end - start).count()
                        << " sec";

    
	}
	catch (std::exception const& e) {
		std::cerr << "ERROR: " << e.what() << std::endl;
	}
	return 1;
}
