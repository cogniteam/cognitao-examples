
#include <cognitao/CogniTao.h>


int main(int argc, char* argv[]) {

	try
	{
		// init WM interface
		WM::init(new MapThreadSafeDataSource());

		if (argc < 2) {
            
			cout<<"-- MISSING XML FILE --"<<endl;
			return 1;

		} else {


			WM::init(new MapThreadSafeDataSource());

			cout<< "Loading XML " << argv[1] << endl;
			Machine* m =  MachineXMLReader::read(argv[1]);

			string linux_path = "/usr/share/cognitao/www/";
			MachineWebServer w(linux_path, "127.0.0.1", "1234", m);
			
			m->start();

			while (true)//!m->getCurrentState()->hasReturn())
			{
				std::this_thread::sleep_for(std::chrono::seconds(1));
			}
		}
	}
	catch (std::exception const& e) {
		std::cerr << "ERROR: " << e.what() << std::endl;
	}
	return 1;
}
