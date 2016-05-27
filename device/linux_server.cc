//
// Created by 刘优 on 16/5/25.
//

#include "linux_server.h"
#include "device/device_interface.h"
#include "common/utils.h"

#include <memory>

namespace device {

class LinuxServer : public DeviceInterface {

public:
  LinuxServer(const SnmpV12Credential& cra){}

  bool get_sys_uptime(int seconds) override;

  bool get_cuputil(double* usage) override;

  bool get_memutil(double* usage) override;

  bool ping(PingStatus* status) override;

  bool get_intf_info(NetworkInfo* info) override;

  bool get_arp_list(ArpInfo* arps) override;

  bool get_dot1_list() override;

};

bool LinuxServer::get_sys_uptime(int seconds) {
  return false;
}

bool LinuxServer::get_cuputil(double *usage) {
  return false;
}

bool LinuxServer::get_memutil(double *usage) {
  return false;
}

bool LinuxServer::ping(PingStatus *status) {
  return false;
}

bool LinuxServer::get_intf_info(NetworkInfo *info) {
  return false;
}

bool LinuxServer::get_arp_list(ArpInfo *arps) {
  return false;
}

bool LinuxServer::get_dot1_list() {
  return false;
}

std::unique_ptr<DeviceInterface> LinuxServerPtr(const SnmpV12Credential& cra) {

#if __cplusplus < 201402L
  using elon::make_unique;
#else
  using std::make_unique;
#endif
  return  make_unique<LinuxServer>(cra);
}

}