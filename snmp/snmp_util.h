//
// Created by 刘优 on 16/5/25.
//

#ifndef SNMP_SNMP_UTIL_H_
#define SNMP_SNMP_UTIL_H_

#include <string>
#include "device/json.hpp"

namespace snmp {

struct NetSnmpSession;

struct Opt {
  std::string name{};
  std::string oid{};
};

class Session {

public:
  Session(const std::string &peer, const std::string &pwd);

  ~Session();

  void *get() const;

private:
  std::unique_ptr<NetSnmpSession> _net_session;
  void *_ss{nullptr};
};


bool snmp_get(const Opt& opt, const Session *session,
              nlohmann::json *values);

size_t snmp_walk(const Opt& opt, const Session *session,
                 nlohmann::json *columns);

size_t snmp_bulkwalk(const Opt& opt, const Session *session,
                     nlohmann::json *columns);

size_t snmp_table(const Opt& opt, const Session *session,
                  nlohmann::json *table);

size_t columns_to_table(const nlohmann::json &columns, nlohmann::json *table);

}

#endif //SNMP_SNMP_UTIL_H_
