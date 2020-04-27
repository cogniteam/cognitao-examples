# CogniTao Examples
![alt text](assets/images/bt.png "CogniTao Behaviour Tree")


## High level Components 
* Task : A task is the basic execution building block (task execution description) in CogniTao graph.
* Machine : A machine is a collection of tasks ordered in a graph and connected by execution protocols (flow logic description). Machines for FSM and BT logic are implemented in the library. 
* Runner : Tasks have runners in order to execute user code (actions). A local runner is used
by CogniTao in order to execute a registered action when linking directly to the library.
A remote runner is used to connect CogniTao to remote running tasks (remote to the library).
CogniTao has plugins for ROS and ROS2 runners.   
* World Model :  The world model is a process and thread safe key/value mechanism that enables subscription to information cahnge. 
Across process UDP multi casting is used to sync several worldmodels. Within the process thread safety is used. 
* DataSource : The protocol used to sync several worldmodels. The default one is udp, however ros and ros2 datasources are available as plugins in CogniTao.


## State Machine
![alt text](assets/images/FSM.jpeg "CogniTao FSM")

### Syntax
```
<machine name="Simple 3 states FSM" type="fsm">
	<state initial="A" final="C">
		<state id="A">
			<transition event="b" target="B"></transition>
		</state>
		<state id="B">
			<transition event="c" target="C"></transition>
		</state>
		<state id="C">
		</state>
	</state>
</machine>
```
### Logic
* In the example a 3 states (A->B-C) graph is described where A is an initial state and C is a final state
* Transitions can be triggered through the wordmodel on /a, /b, /c by setting any value on these paths. 
* Machines can be used heirarchically (states can have machines)


### 2 states example
* In this example a simple state machine is constructed from code [FSM](src/example_state_machine.cpp)


### Canonical turnstile example
* In this example a state machine is constructed from code [FSM](src/example_state_machine_turnstile.cpp)

## Behavior trees
### Syntax
```
<machine name="Simple Behaviour Tree" type="bt">
    <sequencer name="main sequence">
        <set name="timeout" variable="timeout" value="false"/>
        <trigger name="timeout trigger" stop_condition="timeout = true">
            <parallel name="main">
                <shell name="ping google" command="ping" args="google.com -c 10"/>
                <sequencer name="wait 1 sec and set timeout">
                    <wait name="ping wait" value="2"/>
                    <set name="timeout" variable="timeout" value="true"/>
                </sequencer>
            </parallel>
        </trigger>
    </sequencer>
</machine>
```
### Logic
* In the example a shell task is beeing invoked and managed, to ping goolge.
* Ping usually takes 4sec, however we want to stop it after 2 sec
* A trigger is set, and parallel to the ping we wait 2 sec, and set a value in the world model
* That value is used to trigger the stop condition

### Construction from code 
* In this example a behaviour tree is constructed from code [behavior tree](src/example_behavior_tree_construction.cpp)

### Using a behaviour trigger on the WorldModel  
* In this example a behaviour trigger is used to stop an endless task [behavior tree](src/example_behavior_tree_trigger.cpp). 

### Return values from machines and exception handling  
* In this example a 3 behaviour trees are constructed from code [behavior tree](src/example_behavior_tree_return_exceptions.cpp). One succeeds, one fails, and one ends with an exception. 

## Runner examples 
### Using the local runner to run your c++ functions 
* In this example a local runner is used to run  registered actions [local runner](src/example_behavior_tree_user_actions.cpp)

### Creating a custom runner
* In this example a custom runner is defined and registered [custom runner](src/example_behavior_tree_runner.cpp)
  * In order to run ./build/cognitao_example_behavior_tree_runner assets/example_behavior_tree_runner.xml 

### Creating a runner plugin for cognitao_server
* In this example a custom runner plugin is defined and compiled into a shared object [runner plugin](src/example_runner_plugin.cpp)
* From then on it can be loaded at runtime using PluginLoader::loadPlugin()
  * (Locally) cognitao_server will look for a local plugin folder, and then continue
  * (Linux) runners are defaulty installed in /usr/lib/cognitao/plugins 
  * (Windows) runners are defaulty installed in C:\Program Files\CogniTao\plugins 

## CogniTao UI
![alt text](assets/images/UI.png "CogniTao User Interface")
### Usage
* If cognitao_server is running an XML file, the default server will be on port 1234.
* Otherwise the UI server can be created by passing it a machine pointer.
```C++
 MachineWebServer webServer("0.0.0.0", "1234", machineHandle);
```


## High level Components 
* Task : A task is the basic execution building block (task execution description) in CogniTao graph.
* Machine : A machine is a collection of tasks ordered in a graph and connected by execution protocols (flow logic description). Machines for FSM and BT logic are implemented in the library. 
* Runner : Tasks have runners in order to execute user code (actions). A local runner is used
by CogniTao in order to execute a registered action when linking directly to the library.
A remote runner is used to connect CogniTao to remote running tasks (remote to the library).
CogniTao has plugins for ROS and ROS2 runners.   
* World Model :  The world model is a process and thread safe key/value mechanism that enables subscription to information cahnge. 
Across process UDP multi casting is used to sync several worldmodels. Within the process thread safety is used. 
* DataSource : The protocol used to sync several worldmodels. The default one is udp, however ros and ros2 datasources are available as plugins in CogniTao.

### Runner examples 
## build your own runner 
In this example a custom runner is used. This enables to write the following xml plan 

[custom runner example](src/example_behavior_tree_runner.cpp)

## Build status
[![pipeline status](https://git.cogni.io/cognitao/cognitao_examples/badges/master/pipeline.svg)](https://git.cogni.io/cognitao/cognitao_examples/commits/master) **master**

[![pipeline status](https://git.cogni.io/cognitao/cognitao_examples/badges/develop/pipeline.svg)](https://git.cogni.io/cognitao/cognitao_examples/commits/master) **develop**

## Description
CogniTao decision making library C++ examples