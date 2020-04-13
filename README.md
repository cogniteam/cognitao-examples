# CogniTao Examples

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