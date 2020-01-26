/*
 * wmExample.cpp
 *
 *  Created on: Jan 26, 2020
 *      Author: ari
 */
#include <cognitao/CogniTao.h>

int main(int argc, char* argv[])
{
	try{
		cout<<"----"<<endl;

		WM::init(new MapThreadSafeDataSource());
		WM::setVar("/A1/power","100");
		cout<<WM::toString() <<endl;
		for(int i =100;i;i--)
		{
			WM::setVar("/A1/power",StringUtils::itos(i));
			cout<<WM::toString() <<endl;
		}
		cout<<WM::toString() <<endl;
	}catch(CogniTAOExeption &e)
	{
		cout<< e.what()<<endl;
	}
	return 1;
}


