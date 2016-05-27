//
// Created by sven on 10/30/15.
//


#include "snmp/snmp_util.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <memory>
using namespace std;
using namespace snmp;

int main() {

  Opt opt;
  opt.name = "systemUptime";
  opt.oid = ".1.3.6.1.6.3.10.2.1.3.0";

  Session session("116.213.79.50","cnlink");

  nlohmann::json j;

  snmp_get(opt,&session,&j);
  cout << j.dump() << endl;

  opt.oid = ".1.3.6.1.2.1.2.2";


  j.clear();

  snmp_bulkwalk(opt,&session,&j);

  cout << j.dump(2) << endl;

  j.clear();
  j = nlohmann::json::array();
  snmp_table(opt,&session,&j);

  cout << j.dump(2) << endl;

  return 0;
  
}