//
// Created by 刘优 on 16/5/25.
//

#ifndef SNMP_COMMOM_OID_H_
#define SNMP_COMMOM_OID_H_

#include <string>

namespace snmp {

// base info
const std::string IF_DESCR_OID = ".1.3.6.1.2.1.2.2.1.2";
const std::string IF_ALIAS_OID = "1.3.6.1.2.1.31.1.1.1.18";
const std::string IF_PHYS_ADDR_OID = ".1.3.6.1.2.1.2.2.1.6";
const std::string IF_ADMIN_OID = ".1.3.6.1.2.1.2.2.1.7";
const std::string IF_OPER_OID = ".1.3.6.1.2.1.2.2.1.8";
const std::string IFX_NAME_OID = "1.3.6.1.2.1.31.1.1.1.1";
const std::string IP_ADDR_TABLE_OID = ".1.3.6.1.2.1.4.20";

//rate info
const std::string IF_IN_OCT_OID = "1.3.6.1.2.1.31.1.1.1.6";
const std::string IF_IN_UCAST_OID = "1.3.6.1.2.1.31.1.1.1.7";
const std::string IF_IN_MCAST_OID = "1.3.6.1.2.1.31.1.1.1.8";
const std::string IF_IN_BCAST_OID = "1.3.6.1.2.1.31.1.1.1.9";
const std::string IF_OUT_OCT_OID = "1.3.6.1.2.1.31.1.1.1.10";
const std::string IF_OUT_UCAST_OID = "1.3.6.1.2.1.31.1.1.1.11";
const std::string IF_OUT_MCAST_OID = "1.3.6.1.2.1.31.1.1.1.12";
const std::string IF_OUT_BCAST_OID = "1.3.6.1.2.1.31.1.1.1.13";


const std::string IF_TABLE_OID = ".1.3.6.1.2.1.2.2";
const std::string IFXTABLE_OID = "1.3.6.1.2.1.31.1.1";

const std::string IPNET_TO_MEDIA_TABLE_OID = ".1.3.6.1.2.1.4.22";
const std::string DOT1D_TPFDB_OID = ".1.3.6.1.2.1.17.4.3"; //dot1dTpFdbAddress
const std::string IF_IN_OCTETS_OID = "1.3.6.1.2.1.2.2.1.10";
const std::string IF_OUT_OCTETS_OID = "1.3.6.1.2.1.2.2.1.16";
const std::string IF_SPEED_OID = "1.3.6.1.2.1.2.2.1.5";

const std::string RUIJIE_TEMP_OID = "1.3.6.1.4.1.4881.1.1.10.2.1.1.23.1.3";
const std::string RUIJIE_MEMPOOL_OID = ".1.3.6.1.4.1.4881.1.1.10.2.35";
const std::string RUIJIE_MEMUTIL_OID = ".1.3.6.1.4.1.4881.1.1.10.2.35.1.1.1.3.1";

const std::string RUIJIE_CPU_OID = ".1.3.6.1.4.1.4881.1.1.10.2.36";
const std::string RUIJIE_CPU5SEC_OID = "1.3.6.1.4.1.4881.1.1.10.2.36.1.1.1.0";

const std::string CISCO_CPUUTIL_OID = ".1.3.6.1.4.1.9.2.1.56.0";
const std::string CISCO_MEMUTIL_OID = ".1.3.6.1.4.1.2011.10.2.6.1.1.1.1.6";
const std::string CISCO_MEMORY_POOL_OID = ".1.3.6.1.4.1.9.9.48.1.1"; //ciscoMemoryPool

const std::string ENT_PHYSICAL_CLASS_OID = "1.3.6.1.2.1.47.1.1.1.1.5"; //entPhysicalClass

const std::string HH3C_CPUUTIL_PRE_OID = "1.3.6.1.4.1.25506.2.6.1.1.1.1.6";
const std::string HH3C_MEMUTIL_PRE_OID = "1.3.6.1.4.1.25506.2.6.1.1.1.1.8";

//system uptime on seconds
const std::string SYS_UPTIME_OID = ".1.3.6.1.6.3.10.2.1.3.0";

}

#endif //SNMP_COMMOM_OID_H_
