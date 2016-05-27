//
// Created by 刘优 on 16/5/25.
//

#ifndef SNMPCOLLECTOR_DEVICE_LINUX_SERVER_H_
#define SNMPCOLLECTOR_DEVICE_LINUX_SERVER_H_


#include <memory>
#include "device/snmp_credential.h"

namespace device {

class DeviceInterface;

std::unique_ptr<DeviceInterface> LinuxServerPtr(const SnmpV12Credential& cra);

} //namespace

#endif //SNMPCOLLECTOR_DEVICE_LINUX_SERVER_H_
