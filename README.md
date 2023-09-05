# riot

For using with Yocto use this command

devtool add --autorev iotdd https://github.com/joseinweb/riot.git --srcbranch main

To build we have avahi-client and rbus dependencies. 
use the following commands
### Building the code
    
    git clone https://github.com/joseinweb/riot.git
    mkdir build;cd build
    cmake ../ -DRBUS_INSTALL_DIR=<path of rbus installed components>
    make

The excutables will be in riotclient and riotdaemon folder in build folder.

### Running the code
Before starting the client and server, you need to **ensure that the rbus daemon is running** in your device.
First you need to set environment variables

    export PATH=$PATH:${RBUS_INSTALL_DIR}/usr/bin
    export LD_LIBRARY_PATH=${RBUS_INSTALL_DIR}/usr/lib:${LD_LIBRARY_PATH}
    
You need to run two terminals. In the first terminal, start rtrouted

    killall -9 rtrouted; rm -fr /tmp/rtroute*; rtrouted -f -l DEBUG -s unix:///tmp/rtrouted -s tcp://<ip:port>

    Here IP is the ip address of the device, and port is the port you want RPC communication to happen

In the second terminal, start IoT Daemon

    RIoTDaemon tcp://<ip:port>

    The ip port should match the rtrouted parameters. 

On the second device launch the client app

    RiotClient
