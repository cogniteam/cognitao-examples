/*
 * wmExample.cpp
 *
 *  Created on: Jan 26, 2020
 *      Author: ari
 */
#include <cognitao/CogniTao.h>

int main(int argc, char* argv[])
{

	WM::setVar("/A1/power","100");
	cout<<WM::toString() <<endl;
	for(int i =100;i;i--)
	{
		WM::setVar("/A1/power","100");
		cout<<WM::toString() <<endl;
	}
	cout<<WM::toString() <<endl;
	return 1;
}


