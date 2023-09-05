/**
 * If not stated otherwise in this file or this component's LICENSE
 * file the following copyright and licenses apply:
 *
 * Copyright 2023 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 **/

#include "RIoTControl.h"
#include "AvahiClient.h"
#include "RIoTConnector.h"

#include <sstream>

#define API_VERSION_NUMBER_MAJOR 1
#define API_VERSION_NUMBER_MINOR 0
#define API_VERSION_NUMBER_PATCH 0

namespace WPEFramework
{
    namespace Plugin
    {

        std::string getRemoteAddress()
        {
            std::string address = "";
            if (avahi::initialize())
            {
                std::list<std::shared_ptr<avahi::RDKDevice>> devices;
                if (avahi::discoverDevices(devices) > 0)
                {
                    // Assumption is avahi is running on the link local ipv4 address.
                    std::shared_ptr<avahi::RDKDevice> device = devices.front();
                    address = "tcp://" + device->ipAddress + ":" + std::to_string(device->port);
                    std::cout << "Found ipv4 device " << device->ipAddress << ":" << device->port << std::endl;
                }
                else
                {
                    std::cout << " Failed to identify RDK IoT Gateway." << std::endl;
                }
                avahi::unInitialize();
                std::cout << " Failed to identify IPV4 RDK IoT Gateway." << std::endl;
            }
            else
            {
                std::cout << " Failed to initialize avahi " << std::endl;
            }

            return address;
        }

        RIoTControl::RIoTControl()
            : m_apiVersionNumber(API_VERSION_NUMBER_MAJOR), connectedToRemote(false)
        {
            riotConn = new iotbridge::RIoTConnector();
        }

        RIoTControl::~RIoTControl()
        {
        }

        bool RIoTControl::connectToRemote()
        {
            if (remote_addr.empty())
            {
                remote_addr = getRemoteAddress();
            }

            if (!remote_addr.empty())
                connectedToRemote = riotConn->initializeIPC(remote_addr);
            std::cout << "[RIoTControl::connectToRemote] completed .Remote address: " << remote_addr.c_str() << std::endl;
            return connectedToRemote;
        }
        // Supported methods
        bool RIoTControl::refreshIoTBridgeConnection()
        {

            if (!connectedToRemote)
                connectToRemote();

            return connectedToRemote;
        }

        bool RIoTControl::getAvailableDevicesWrapper()
        {
            bool success = false;
            if (connectedToRemote)
            {
                std::list<std::shared_ptr<WPEFramework::iotbridge::IOTDevice>> deviceList;

                if (riotConn->getDeviceList(deviceList) > 0)
                {

                    for (const auto &device : deviceList)
                    {

                        std::cout << "deviceName " << device->deviceName << std::endl;
                        std::cout << "uuid " << device->deviceId << std::endl;
                        std::cout << "type " << device->devType << std::endl;
                    }
                }

                success = true;
            }
            else
            {
                std::cout << "Failed to connect to IoT Gateway" << std::endl;
            }
            return (success);
        }

        bool RIoTControl::getDeviceProperties(const std::string &uuid, std::list<std::string> &properties)
        {
            bool success = false;

            if (connectToRemote())
            {
                std::list<std::string> properties;
                riotConn->getDeviceProperties(uuid, properties);
                std::cout << "Value returned is " << properties.size() << std::endl;

                for (std::string const &property : properties)
                {
                    std::stringstream stream(property);
                    std::string key, value;
                    std::getline(stream, key, '=');
                    std::getline(stream, value, '=');
                    std::cout <<"\t"<< key << " : " << value << std::endl;
                }

                success = true;
            }
            else
            {

                std::cout << "Failed to connect to IoT Gateway" << std::endl;
            }

            return (success);
        }
        bool RIoTControl::getDeviceProperty(const std::string &uuid, const std::string &prop)
        {
            bool success = false;

            if (connectToRemote())
            {
                std::string propVal = riotConn->getDeviceProperty(uuid, prop);
                std::cout << "IOT gateway returned " << propVal << std::endl;
                success = true;
            }
            else
            {
                std::cout << "Failed to connect to IoT Gateway" << std::endl;
            }

            return (success);
        }
        bool RIoTControl::sendCommand(const std::string &uuid, const std::string &cmd)
        {
            bool success = false;

            if (connectedToRemote)
            {
                if (0 == riotConn->sendCommand(uuid, cmd))
                    success = true;
            }
            else
            {
                std::cout << "Failed to connect to IoT Gateway" << std::endl;
            }

            return (success);
        }
        void RIoTControl::deInitialize()
        {
            riotConn->cleanupIPC();
            delete riotConn;
            riotConn = nullptr;

            connectedToRemote = false;
            remote_addr = "";
        }

    } // namespace Plugin
} // namespace WPEFramework

int main(int argc, char const *argv[])
{
    /* code */

    using WPEFramework::Plugin::RIoTControl;

    RIoTControl *client = new RIoTControl();
    std::list<std::string> properties;
    while(!client->refreshIoTBridgeConnection());
    client->getAvailableDevicesWrapper();

    client->getDeviceProperties("000d6f000ef0a7a8", properties);
    client->getDeviceProperty("000d6f000ef0a7a8", "hardwareVersion");
    client->sendCommand("000d6f000ef0a7a8", "isOn=false");
    int x;
    std::cout <<"Light turned off"<<std::endl;
    std::cout << "Enter a number :" << std::flush;
    std::cin>>x;
    client->sendCommand("000d6f000ef0a7a8", "isOn=true");
    std::cout <<"Light turned on"<<std::endl;
    std::cout << "Enter a number :" << std::flush;
    std::cin>>x;
    client->sendCommand("000d6f000ef0a7a8", "isOn=false");
    std::cout <<"Light turned off"<<std::endl;
    client->deInitialize();
    delete client;
    return 0;
}