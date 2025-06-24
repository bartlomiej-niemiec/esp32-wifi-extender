/*  WiFi softAP Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "wifi_access_point/wifi_access_point_factory.hpp"
#include "wifi_access_point/wifi_access_point.hpp"
#include "nvs/nvs.hpp"

extern "C" void app_main(void)
{
    Hw::Nvs::Nvs::Init();
    Hw::WifiAccessPoint::WifiAccessPoint* wifi_ap = Hw::WifiAccessPoint::WifiAccessPointFactory::GetWifiAP();
    wifi_ap->Init("ssid", "password");
    wifi_ap->Startup();
}