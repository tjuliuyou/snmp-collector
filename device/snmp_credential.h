//
// Created by 刘优 on 16/5/25.
//

#ifndef DEVICE_SNMP_CREDENTIAL_H_
#define DEVICE_SNMP_CREDENTIAL_H_

#include <string>

namespace device {

class SnmpV12Credential {
  std::string ip{};
  int port{161};
  std::string pwd{};
};

} //namespace device

#endif //DEVICE_SNMP_CREDENTIAL_H_
