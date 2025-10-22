#include "wifi_manager.hpp"
#include "esp_wifi.h"
#include "esp_log.h"

namespace Hw
{

namespace Platform
{

namespace WifiExtender
{

bool WifiManager::Init()
{
    esp_err_t err;

    err = esp_netif_init();
    if (err != ESP_OK && err != ESP_ERR_INVALID_STATE)
    {
        ESP_LOGE("WifiManager", "esp_netif_init failed: %s", esp_err_to_name(err));
        return false;
    }

    err = esp_event_loop_create_default();
    if (err != ESP_OK && err != ESP_ERR_INVALID_STATE)
    {
        ESP_LOGE("WifiManager", "esp_event_loop_create_default failed: %s", esp_err_to_name(err));
        return false;
    }

    m_WifiManagerContext.m_WifiSta.Init();
    m_WifiManagerContext.m_WifiSta.SetState(WifiSta::State::INITIALIZED);

    m_WifiManagerContext.m_WifiAp.Init();
    m_WifiManagerContext.m_WifiAp.SetState(WifiAp::State::INITIALIZED);

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    err = esp_wifi_init(&cfg);
    if (err != ESP_OK && err != ESP_ERR_WIFI_INIT_STATE)
    {
        ESP_LOGE("WifiManager", "esp_wifi_init failed: %s", esp_err_to_name(err));
        return false;
    }

    err = esp_wifi_set_mode(WIFI_MODE_APSTA);

    if (err != ESP_OK && err != ESP_ERR_WIFI_INIT_STATE)
    {
        ESP_LOGE("WifiManager", "esp_wifi_set_mode failed: %s", esp_err_to_name(err));
        return false;
    }

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                            ESP_EVENT_ANY_ID,
                                                            &wifi_ip_event_handler,
                                                            this,
                                                            &m_wifiAnyInst));
    ESP_LOGI("WifiManager", "Registering WIFIEVENT handler");

    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                            ESP_EVENT_ANY_ID,
                                                            &wifi_ip_event_handler,
                                                            this,
                                                            &m_ipAnyInst));
    ESP_LOGI("WifiManager", "Registering IPEVENT handler");

    return true;
}

void WifiManager::WifiManagerMain(void *pArg)
{
    assert(nullptr != pArg);
    WifiManager * pWifiManager = reinterpret_cast<WifiManager *>(pArg);
    MessageQueue::Message recvMsg;
    while (true)
    {
        if (pWifiManager->m_MessageQueue.Receive(recvMsg))
        {
            pWifiManager->OnMessage(recvMsg);
        }
    } 

}

void WifiManager::OnMessage(const MessageQueue::Message & msg)
{
    switch (msg.event) {
      case MessageQueue::EventType::EspWifiEvent: {
        switch (msg.espEventCode) {
          case WIFI_EVENT_AP_START:
            m_WifiManagerContext.OnApStart();
            break;
          case WIFI_EVENT_AP_STOP:
            m_WifiManagerContext.OnApStop();
            break;
          case WIFI_EVENT_AP_STACONNECTED:
            m_WifiManagerContext.OnApStaConnected();
            break;
          case WIFI_EVENT_AP_STADISCONNECTED:
            m_WifiManagerContext.OnApStaDisconnected();
            break;
          case WIFI_EVENT_STA_START:
            m_WifiManagerContext.OnStaStart();
            break;
          case WIFI_EVENT_STA_CONNECTED:
            m_WifiManagerContext.OnStaConnected();
            break;
          case WIFI_EVENT_STA_DISCONNECTED:
            m_WifiManagerContext.OnStaDisconnected();
            break;
          case WIFI_EVENT_STA_STOP:
            m_WifiManagerContext.OnStaStop();
            break;
        }
      }
      break;
      case MessageQueue::EventType::EspIpEvent: {
        if (msg.espEventCode == IP_EVENT_STA_GOT_IP) 
        {
            m_WifiManagerContext.OnStaGotIp();
        }
        else if (msg.espEventCode == IP_EVENT_STA_LOST_IP)
        {
            m_WifiManagerContext.OnStaLostIp();
        }
      }
      break;

      case MessageQueue::EventType::StaTimerReconnect:
        break;
      default:
      break;
    }

    ESP_LOGI("WifiManager", "MsgEvent %s", MessageQueue::GetStringOfEventType(msg.event).data());

    Snapshot s = makeSnapshot();

    auto dec = reduce(s, msg);
    if (dec.newState)
    {
        m_WifiManagerContext.m_WifiManagerState = dec.next;
        ESP_LOGI("WifiManager", "Changing to state %s", Hw::WifiExtender::WifiExtenderHelpers::WifiExtenderStaToString(m_WifiManagerContext.m_WifiManagerState).data());
    }
    
    for (uint8_t i = 0; i < dec.count; ++i) {
        runEffect(dec.effects[i], msg);
    }

    /// Check if new configuration is pending and start up extender with pending config
    MaybeFinalize(makeSnapshot());
}

void WifiManager::MaybeFinalize(const Snapshot& s)
{
    /// Check if new configuration is pending and start up extender with pending config
    if ((s.mgrState == WifiExtenderState::STOPPING || s.mgrState == WifiExtenderState::NEW_CONFIGURATION_PENDING) &&
        s.apState == WifiAp::State::STOPPED &&
        s.staState == WifiSta::State::STOPPED)
    {
        MessageQueue::Message msg(MessageQueue::EventType::InternalStop, -1);
        m_MessageQueue.Add(msg);

        if (s.updateConfig)
        {
            m_StartUpInProgress = true;
            m_WifiManagerContext.m_PendingNewConfiguration = false;
            MessageQueue::Message msg(MessageQueue::EventType::StartReq, -1);
            m_MessageQueue.Add(msg);
        }
    }
}

WifiManager::WifiManager():
    m_pEventListener(nullptr),
    m_StartUpInProgress(false),
    m_StaConnectionTimer(nullptr),
    m_timerArgs()
{

    assert(esp_timer_init() != ESP_ERR_NO_MEM );
    m_timerArgs.callback = RetryConnectToNetwork;
    m_timerArgs.arg = this;
    m_timerArgs.dispatch_method = ESP_TIMER_TASK;
    m_timerArgs.name = "STA_CONNECTION_TIMER";
    m_timerArgs.skip_unhandled_events = true;

    ESP_ERROR_CHECK(esp_timer_create(&m_timerArgs, &m_StaConnectionTimer));

    assert(true == Init());

    xTaskCreate(
        WifiManagerMain,
        m_pTaskName,
        TASK_STACK_SIZE,
        this,
        TASK_PRIO,
        &m_TaskHandle
    );
    assert(nullptr != m_TaskHandle);
    ESP_LOGI("WifiManager", "Init Done");
}

WifiManager::~WifiManager()
{
    esp_timer_delete(m_StaConnectionTimer);
    esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, m_wifiAnyInst);
    esp_event_handler_instance_unregister(IP_EVENT, ESP_EVENT_ANY_ID, m_ipAnyInst);
    vTaskDelete(m_TaskHandle);
    esp_netif_deinit();
}

bool WifiManager::Startup(const WifiExtenderConfig & config)
{
    bool res = false;
    if (false == m_WifiManagerContext.m_PendingNewConfiguration && m_StartUpInProgress == false)
    {
        m_StartUpInProgress = true;
        m_PendingConfig = config;
        MessageQueue::Message msg(MessageQueue::EventType::StartReq, -1);
        res = m_MessageQueue.Add(msg);
    }
    return res;
}

bool WifiManager::Shutdown()
{
    MessageQueue::Message msg(MessageQueue::EventType::StopReq, -1);
    return m_MessageQueue.Add(msg);
}

bool WifiManager::UpdateConfig(const WifiExtenderConfig & config)
{
    bool res = false;
    if (false == m_WifiManagerContext.m_PendingNewConfiguration && m_StartUpInProgress == false)
    {
        m_WifiManagerContext.m_PendingNewConfiguration = true;
        m_PendingConfig = config;
        MessageQueue::Message msg(MessageQueue::EventType::UpdateConfigReq, -1);
        res = m_MessageQueue.Add(msg);
    }
    return res;
}

bool WifiManager::RegisterListener(EventListener * pEventListener)
{
    assert(m_pEventListener == nullptr);
    m_pEventListener = pEventListener;
    return true;
}

void WifiManager::Stop()
{
    esp_wifi_disconnect();
    esp_wifi_stop();
}

void WifiManager::wifi_ip_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{

    WifiManager* pWifiManager = reinterpret_cast<WifiManager*>(arg);

    MessageQueue::Message msg = {};

    bool interesting = false;
    if (event_base == WIFI_EVENT) {
        switch (event_id) {
            case WIFI_EVENT_AP_START:
            case WIFI_EVENT_AP_STOP:
            case WIFI_EVENT_AP_STACONNECTED:
            case WIFI_EVENT_AP_STADISCONNECTED:
            case WIFI_EVENT_STA_CONNECTED:
            case WIFI_EVENT_STA_START:
            case WIFI_EVENT_STA_STOP:
            case WIFI_EVENT_STA_DISCONNECTED:
            {
                msg.event = MessageQueue::EventType::EspWifiEvent;
                msg.espEventCode = event_id;
                interesting = true; 
            }
            break;
        }
    }
    else if (event_base == IP_EVENT)
    {
        switch (event_id)
        {
            case IP_EVENT_STA_GOT_IP:
            case IP_EVENT_STA_LOST_IP:
            {
                msg.event = MessageQueue::EventType::EspIpEvent;
                msg.espEventCode = event_id;
                interesting = true;
            }
            break;
            
        };
    }

    if (interesting)
    {
       pWifiManager->m_MessageQueue.Add(msg);
    }
}

void WifiManager::RetryConnectToNetwork(void *arg)
{
    ESP_LOGI("WifiExtender", "Timer expired..");
    WifiManager* pWifiManager = reinterpret_cast<WifiManager*>(arg);
    MessageQueue::Message msg(MessageQueue::EventType::StaTimerReconnect, -1);
    pWifiManager->m_MessageQueue.Add(msg);
}

void WifiManager::StartStaBackoffTimer()
{
    ESP_ERROR_CHECK(esp_timer_start_once(m_StaConnectionTimer, TIMER_EXPIRED_TIME_US));
}

void WifiManager::StopStaBackoffTimer()
{
    if (esp_timer_is_active(m_StaConnectionTimer))
    {
        ESP_ERROR_CHECK(esp_timer_stop(m_StaConnectionTimer));
    }
}

WifiExtenderState WifiManager::GetState()
{
    return m_WifiManagerContext.m_WifiManagerState;
}

WifiManager::Snapshot WifiManager::makeSnapshot() const
{
    return {
        m_WifiManagerContext.m_WifiManagerState,
        m_WifiManagerContext.m_WifiAp.GetState(),
        m_WifiManagerContext.m_WifiSta.GetState(),
        m_WifiManagerContext.m_PendingNewConfiguration,
        m_StartUpInProgress,
        m_WifiManagerContext.m_StaConfigurationValid
    };
}

WifiManager::Decision WifiManager::reduce(const WifiManager::Snapshot& s, const MessageQueue::Message& msg) const
{
    Decision d = {};
    switch (msg.event)
    {
        case MessageQueue::EventType::StartReq:
        {
            if (s.mgrState == WifiExtenderState::STOPPED)
            {
                d.next = WifiExtenderState::STARTED;
                d.newState = true;
                push(d, Effect::ApplyConfig);
                push(d, Effect::WifiStart);
                push(d, Effect::NotifyListener);
            }
        }
        break;

        case MessageQueue::EventType::StopReq:
        {
            if (s.mgrState == WifiExtenderState::RUNNING ||
                s.mgrState == WifiExtenderState::STARTED ||
                s.mgrState == WifiExtenderState::CONNECTING ||
                s.mgrState == WifiExtenderState::STA_CANNOT_CONNECT)
            {
                d.next = WifiExtenderState::STOPPING;
                d.newState = true;
                push(d, Effect::DisableNat);
                push(d, Effect::WifiStop);
                push(d, Effect::NotifyListener);
            }
        }
        break;

        case MessageQueue::EventType::UpdateConfigReq:
        {
            d.next = WifiExtenderState::NEW_CONFIGURATION_PENDING;
            d.newState = true;
            push(d, Effect::DisableNat);
            push(d, Effect::WifiStop);
            push(d, Effect::NotifyListener);
        }
        break;

        case MessageQueue::EventType::EspWifiEvent:
        {
            switch (msg.espEventCode) {
                case WIFI_EVENT_AP_START:
                    if (s.staCfgValid == false)
                    {
                        d.next = WifiExtenderState::RUNNING;
                        d.newState = true;
                        push(d, Effect::NotifyListener);
                    }
                    break;
                case WIFI_EVENT_AP_STADISCONNECTED:
                    if (s.staState == WifiSta::State::CONNECTED)
                    {
                        push(d, Effect::StartStaBackoffTimer);
                        push(d, Effect::StaConnect);
                    }
                    break;  
                case WIFI_EVENT_STA_START:
                    if (s.staCfgValid)
                    {
                        d.next = WifiExtenderState::CONNECTING;
                        d.newState = true;
                        push(d, Effect::StaConnect);
                        push(d, Effect::StartStaBackoffTimer);
                        push(d, Effect::NotifyListener);
                    }
                    break;
        }
        }
        break;

        case MessageQueue::EventType::EspIpEvent:
        {
            switch (msg.espEventCode) {
                case IP_EVENT_STA_GOT_IP:
                    d.next = WifiExtenderState::RUNNING;
                    d.newState = true;
                    push(d, Effect::SetUpDns);
                    push(d, Effect::SetDefaultNetIf);
                    push(d, Effect::EnableNat);
                    push(d, Effect::StopStaBackoffTimer);
                    push(d, Effect::NotifyListener);
                    break;
                case IP_EVENT_STA_LOST_IP:
                    d.next = WifiExtenderState::CONNECTING;
                    d.newState = true;
                    push(d, Effect::DisableNat);
                    push(d, Effect::StartStaBackoffTimer);
                    push(d, Effect::NotifyListener);
                    break;
            }
        }
        break;

        case MessageQueue::EventType::StaTimerReconnect:
        {
            if (!s.updateConfig) {
                d.next = WifiExtenderState::STA_CANNOT_CONNECT;
                d.newState = true;
                push(d, Effect::StaConnect);
                push(d, Effect::StartStaBackoffTimer);
            }
        }
        break;

        case MessageQueue::EventType::InternalStop:
        {
            d.next = WifiExtenderState::STOPPED;
            d.newState = true;
            push(d, Effect::NotifyListener);
        }
        default:
            break;
    }

    return d;
}

void WifiManager::runEffect(Effect e, const MessageQueue::Message& m)
{
    switch (e) {
        case Effect::ApplyConfig: {
            m_WifiManagerContext.SetStaConfigurationValid(m_PendingConfig.staConfig.IsValid());
            m_WifiManagerContext.m_WifiAp.SetConfig(m_PendingConfig.apConfig);
            m_WifiManagerContext.m_WifiSta.SetConfig(m_PendingConfig.staConfig);

            m_WifiManagerContext.m_WifiAp.SetState(WifiAp::State::CONFIGURED);
            m_WifiManagerContext.m_WifiSta.SetState(WifiSta::State::CONFIGURED);
            if (m_StartUpInProgress)
            {
                m_StartUpInProgress = false;
            }
            break;
        }
        break;

        case Effect::WifiStart:
        {
            ESP_ERROR_CHECK(esp_wifi_start());
        }
        break;

        case Effect::WifiStop:
        {
            Stop();
        }
        break;

        case Effect::EnableNat:
        {
            m_WifiManagerContext.m_WifiAp.EnableNat();
        }
        break;

        case Effect::DisableNat:
        {
            m_WifiManagerContext.m_WifiAp.DisableNat();
        }
        break;

        case Effect::StaConnect:
        {
            (void)esp_wifi_connect();
        }
        break;

        case Effect::SetDefaultNetIf:
        {
            m_WifiManagerContext.m_WifiSta.SetDefaultNetIf();
        }
        break;

        case Effect::SetUpDns:
        {
            esp_netif_dns_info_t dnsInfo = m_WifiManagerContext.m_WifiSta.GetDnsInfo();
            ESP_LOGI("DNS_INFO", "Primary dns ip: %d.%d.%d.%d", esp_ip4_addr1(&(dnsInfo.ip.u_addr.ip4)),
                                                    esp_ip4_addr2(&(dnsInfo.ip.u_addr.ip4)),
                                                    esp_ip4_addr3(&(dnsInfo.ip.u_addr.ip4)),
                                                    esp_ip4_addr4(&(dnsInfo.ip.u_addr.ip4)));
            esp_netif_ip_info_t ipInfo = m_WifiManagerContext.m_WifiSta.GetIpInfo();

            if (ipInfo.gw.addr == dnsInfo.ip.u_addr.ip4.addr)
            {
                ESP_LOGI("DNS_INFO", "Setting default primary dns to 8.8.8.8");
                dnsInfo.ip.u_addr.ip4.addr = esp_netif_ip4_makeu32(8, 8, 8, 8);
            }
                    
            m_WifiManagerContext.m_WifiAp.SetUpDnsOnDhcpServer(dnsInfo);
        }
        break;

        case Effect::StartStaBackoffTimer:
        {
            StartStaBackoffTimer();
        }
        break;

        case Effect::StopStaBackoffTimer:
        {
            StopStaBackoffTimer();
        }
        break;

        case Effect::NotifyListener:
        if (nullptr != m_pEventListener)
        {
            m_pEventListener->Callback(m_WifiManagerContext.m_WifiManagerState);
        }
        break;

        default: break;
    }
}


}

}

}