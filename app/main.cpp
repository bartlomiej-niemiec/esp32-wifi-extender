/*  WiFi softAP Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

// #include "access_point_network_stack/access_point_network_stack_factory.hpp"
// #include "access_point_network_stack/access_point_network_stack.hpp"
// #include "access_point_network_stack/access_point_network_stack_types.hpp"

#include "nvs/nvs.hpp"

extern "C" void app_main(void)
{
    Hw::Nvs::Nvs::Init();
    // Hw::AccessPointNetworkStack::AccessPointNetworkStack* m_ApNetworkStack = Hw::AccessPointNetworkStack::AccessPointNetworkStackFactory::GetAccessPointNetworkStack();

    // Hw::AccessPointNetworkStack::DhcpConfig dhcpConfif = {.enabled = true};
    // Hw::AccessPointNetworkStack::AccessPointConfig apConfig = {.ssid = "ESP32", .password = "dupadupa1"};

    // m_ApNetworkStack->Init(dhcpConfif, apConfig);
    // m_ApNetworkStack->Startup();
}