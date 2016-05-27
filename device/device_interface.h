//
// Created by 刘优 on 16/5/25.
//

#ifndef COMMON_DEVICEINTERFACE_H_
#define COMMON_DEVICEINTERFACE_H_

#include <string>

namespace device {

struct PingStatus {
  //time on millseconds
  double max_time{0.0};
  double avg_time{0.0};
  double min_time{0.0};
  double loss_pct{0.0};
};

struct NetworkInfo {

  long index{ 0 };
  std::string name{};
  std::string ip{};
  std::string desc{};
  std::string mask{};
  std::string mac{};

};

struct ArpInfo {
  std::string index{};
  long ipNetToMediaIfIndex{0};
  std::string ipNetToMediaPhysAddress{};
  std::string ipNetToMediaNetAddress{};
  long ipNetToMediaType{};

};

class DeviceInterface {

public:

  virtual ~DeviceInterface(){}

  virtual bool get_sys_uptime(int seconds) = 0;

  virtual bool get_cuputil(double* usage) = 0;

  virtual bool get_memutil(double* usage) = 0;

  virtual bool ping(PingStatus* status) = 0;

  virtual bool get_intf_info(NetworkInfo* info) = 0;

  virtual bool get_arp_list(ArpInfo* arps) = 0;

  virtual bool get_dot1_list() = 0;

};

}




#endif //COMMON_DEVICEINTERFACE_H_
