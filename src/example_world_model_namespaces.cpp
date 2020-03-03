/**
 * @brief CogniTao signals module test executable
 * 
 * @file cognitao_signals.cpp
 * 
 * @author Igor Makhtes (igor@cogniteam.com)
 * @date 2020-02-20
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
#include <vector>
#include <functional>

#include <cognitao/WorldModel.h>
#include <cognitao/data_sources/UdpMulticastDataSource.h>


using namespace std;
using namespace cognitao::signals;


int main(int argc, char const *argv[]) {

    WorldModel::setDataSource(UdpMulticastDataSource::create());
    WorldModel::setNamespace(string(argv[1]));

    

    while (true) {

        auto vars = WorldModel::getAllVariableNames();

        cout << "World model, namespace = /" << WorldModel::getNamespace() << ":" << endl;
        for (auto&& name : vars) {
            cout << " - " << name << ": " << WorldModel::getVar(name) << endl;
        }
        cout << endl;
        WorldModel::setVar("ready", "True");
        sleep(1);
    }

    return 0;

}