//
// Created by 刘优 on 16/5/25.
//

#include "snmp_util.h"

#include <mutex>
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <arpa/inet.h>

#include "common/utils.h"

#ifdef _WIN32
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#endif

using elon::file_exists;
#if __cplusplus < 201402L
using elon::make_unique;
#else
using std::make_unique;
#endif

namespace snmp {

//Sometimes the octet string contains all printable charachters and this
// produces unexpected output when it is not translated.
const std::string IF_PHY_ADDR_OID = "1.3.6.1.2.1.2.2.1.6";
const std::string IP_NET_TO_MEDIA_PHY_ADDR_OID= ".1.3.6.1.2.1.4.22.1.2";

namespace {

static bool match_phys_address(const std::string& oid) {
  return (oid.find(IF_PHY_ADDR_OID) != oid.npos ||
      oid.find(IP_NET_TO_MEDIA_PHY_ADDR_OID) != oid.npos);
}
static std::string snmp_hex_to_string(const struct variable_list *pvars);

static std::string oid_to_string(const oid *poid, size_t len);
static void parser_snmp_values(const struct variable_list *pvars, nlohmann::json* js);
static void device_to_col_and_index(const std::string& name,
                                    std::string* colname,
                                    std::string* index);
static size_t find_in_vec(const std::vector<std::string>& vec, const std::string& name);

class MibLoader {
public:
  MibLoader() noexcept {
    static std::once_flag init_flag;
    std::call_once(init_flag,init_once);
  }
  ~MibLoader() {
    shutdown_mib();
    SOCK_CLEANUP;
  }

private:

  static void init_once() {
#ifndef NDEBUG
//    assert(file_exists("../mibs"));
#endif
    init_snmp("snmp local session");
    add_mibdir("../mibs");
    read_all_mibs();
    SOCK_STARTUP;
  }
};

} //namespace


struct NetSnmpSession {
  netsnmp_session session;
private:
  const static MibLoader __loader;
};

const MibLoader NetSnmpSession::__loader;

Session::Session(const std::string &peer, const std::string &pwd)
    :_net_session(make_unique<NetSnmpSession>()) {
  snmp_sess_init(&_net_session->session);
  _net_session->session.version = SNMP_VERSION_2c;
  //session.peername = peer.c_str();
#ifdef _WIN32
  _net_session->session.peername = _strdup(ip.c_str());
#else
  _net_session->session.peername = strdup(peer.c_str());
#endif
  _net_session->session.community = (u_char *) pwd.c_str();
  _net_session->session.community_len = pwd.length();
  _net_session->session.timeout = 1000 * 500;
  _ss = snmp_sess_open(&_net_session->session);
}

Session::~Session() {
  if(_ss) snmp_sess_close(_ss);
  free(_net_session->session.peername);
  snmp_close(&_net_session->session);
  _ss = nullptr;
}

void *Session::get() const {
  return _ss;
}

bool snmp_get(const Opt& opt, const Session *session,
              nlohmann::json *values) {

  auto ret = false;

  auto pdu = snmp_pdu_create(SNMP_MSG_GET);
  oid root[MAX_OID_LEN] = {0};
  size_t rootlen = MAX_OID_LEN;

  if(read_objid(opt.oid.c_str(),root,&rootlen)) {
    snmp_add_null_var(pdu, root, rootlen);
    snmp_pdu *resp = nullptr;
    auto status = snmp_sess_synch_response(session->get(), pdu, &resp);

    switch (status) {
      case STAT_SUCCESS: {
        if(resp->errstat == SNMP_ERR_NOERROR ) {
          for (auto vars = resp->variables; vars; vars = vars->next_variable) {
            if ((vars->type != SNMP_ENDOFMIBVIEW) &&
                (vars->type != SNMP_NOSUCHOBJECT) &&
                (vars->type != SNMP_NOSUCHINSTANCE) &&
                (vars->type != ASN_NULL)) {

              if(match_phys_address(opt.oid)) {
                (*values)[opt.name] = snmp_hex_to_string(vars);
              }else {
                auto js = nlohmann::json::array();
                parser_snmp_values(vars, &js);
                (*values)[opt.name] = js[0];
              }
              ret = true;
            }
            else
              (*values)["error"] = "an exception value";
          }
        } else {
          (*values)["error"] = "Error in packet Reason:" +
              std::string(snmp_errstring(resp->errstat));
        }
        break;
      }
      case STAT_TIMEOUT: {
        auto *sptr = snmp_sess_session(session->get());
        (*values)["error"] = std::string("Timeout: No Response from ") + sptr->peername;
        break;
      }
      default: {
        (*values)["error"] = "Other error occurs when get: " + opt.oid;
      }
    }
  } else {
    (*values)["error"] = std::string("Read_objid error! oid: ") + opt.oid;
  }
  return ret;
}

size_t snmp_walk(const Opt& opt, const Session *session,
                 nlohmann::json *columns) {
  return 0;
}

size_t snmp_bulkwalk(const Opt& opt, const Session *session,
                     nlohmann::json *columns) {
  size_t ret = 0;

  oid name[MAX_OID_LEN] = {0};
  size_t name_length = MAX_OID_LEN;
  if (read_objid(opt.oid.c_str(), name, &name_length)) {
    oid root[MAX_OID_LEN] = {0};
    size_t rootlen = MAX_OID_LEN;
    read_objid(opt.oid.c_str(), root, &rootlen);

    auto running = true;
    snmp_pdu *resp = nullptr;
    char buf[MAX_OID_LEN] = {0};
    while (running) {
      auto pdu = snmp_pdu_create(SNMP_MSG_GETBULK);
      pdu->non_repeaters = 0;
      pdu->max_repetitions = 20;
      snmp_add_null_var(pdu, name, name_length);

      auto status = snmp_sess_synch_response(session->get(), pdu, &resp);

      switch (status) {
        case STAT_SUCCESS: {

          if (resp->errstat == SNMP_ERR_NOERROR) {

            for(auto vars = resp->variables; vars; vars = vars->next_variable) {
              //not part of this subtree
              if ((vars->name_length < rootlen)
                  || (memcmp(root,vars->name, rootlen * sizeof(oid) ) != 0)) {
                running = false; continue;
              }
              //not an exception value
              if (vars->type != SNMP_ENDOFMIBVIEW &&
                  vars->type != SNMP_NOSUCHOBJECT &&
                  vars->type != SNMP_NOSUCHINSTANCE &&
                  vars->type != ASN_NULL) {
                snprint_objid(buf,MAX_OID_LEN,vars->name,vars->name_length);
                std::string subname = buf;
                auto pos = subname.find("::");
                if(pos != subname.npos) subname = subname.substr(pos+2);

                auto v = nlohmann::json::array();
                match_phys_address(oid_to_string(vars->name, vars->name_length)) ?
                  v.push_back(snmp_hex_to_string(vars)) : parser_snmp_values(vars, &v);
                (*columns)[subname] = v[0];
                ret += 1;

                //Check if last variable, and if so, save for next request.
                memmove((char *) name, (char *) vars->name,
                        vars->name_length * sizeof(oid));
                name_length = vars->name_length;
              } else {
                (*columns)["error"] = "get an exception value";
                running = false;
              }
            }
          } else { //
            (*columns)["error"] = "error in response";
            running = false;
          }//

          break;
        }
        case STAT_TIMEOUT: {
          auto *sptr = snmp_sess_session(session->get());
          (*columns)["error"] = std::string("Timeout: No Response from ") + sptr->peername;
          break;
        }
        default: {
          (*columns)["error"] = "Other error occurs when bulkwork: " + opt.oid;
        }
      }
      if (resp) snmp_free_pdu(resp);
    }
  } else {
    (*columns)["error"] = std::string("Read_objid error! oid: ") + opt.oid;
  }
  return ret;
}

size_t snmp_table(const Opt& opt, const Session *session,
                  nlohmann::json *table) {
  nlohmann::json columns;

  auto ret = snmp_bulkwalk(opt,session,&columns);
  if(ret > 1) {
      ret = columns_to_table(columns,table);
  } else {
    *table = columns; // ret <=0; -1 == timeout
  }
  return ret;
}


size_t columns_to_table(const nlohmann::json &columns, nlohmann::json *table) {

  std::vector<std::string> fakeindex;

  for(auto it = columns.cbegin();it != columns.cend(); ++it) {

    std::string colname, rowname ;
    device_to_col_and_index(it.key(),&colname,&rowname);

    auto rowindex = find_in_vec(fakeindex,rowname);

    //did not find
    if(rowindex == rowname.npos) {
      table->push_back(nlohmann::json::object());
      fakeindex.push_back(rowname);
      rowindex = table->size() - 1;
      (*table)[rowindex]["_fake_index"] = rowname;
    }
    (*table)[rowindex][colname] = it.value();
  }

  return table->size();
}

namespace {

std::string snmp_hex_to_string(const struct variable_list *pvars) {
  std::string str;
  char buf[MAX_OID_LEN] = {0};
  for (unsigned int i = 0; i < pvars->val_len ; ++i) {
    snprintf(buf,MAX_OID_LEN,"%02X:",pvars->buf[i]) ;
    str +=buf;
  }
  str = str.substr(0,str.length()-1);
  return str;
}


static std::string oid_to_string(const oid *poid, size_t len) {
  char str[MAX_OID_LEN] = {0};
  char tmp[128] = {0};
  for (long i = 0; i < len; ++i) {
    snprintf(tmp, sizeof(tmp), ".%lu", poid[i]);
    strcat(str, tmp);
  }
  return std::string(str);
}

void parser_snmp_values(const struct variable_list *pvars, nlohmann::json* js) {
  switch (pvars->type) {
    case ASN_INTEGER:
    case ASN_UNSIGNED:
    case ASN_TIMETICKS:
    case ASN_COUNTER:
    case ASN_UINTEGER: {
      js->push_back(*pvars->val.integer);
      break;
    }
    case ASN_IPADDRESS: {
      uint32_t ipaddr;
      memcpy(&ipaddr, pvars->val.string, sizeof(ipaddr));
      struct in_addr ip;
      ip.s_addr = ipaddr;
      js->push_back(std::string(inet_ntoa(ip)));
      break;
    }
    case ASN_PRIV_IMPLIED_OBJECT_ID:
    case ASN_PRIV_INCL_RANGE:
    case ASN_PRIV_EXCL_RANGE:
    case ASN_OBJECT_ID: {
      js->push_back(oid_to_string(pvars->name, pvars->name_length));
      break;
    }
    case ASN_FLOAT: {
      js->push_back(*pvars->val.floatVal);
      break;
    }
    case ASN_DOUBLE: {
      js->push_back(*pvars->val.doubleVal);
      break;
    }
    case ASN_OPAQUE_U64:
    case ASN_OPAQUE_I64:
    case ASN_COUNTER64:
    case ASN_OPAQUE_COUNTER64: {
      js->push_back(((pvars->val.counter64->high) << 32) + pvars->val.counter64->low);
      break;
    }
    case ASN_BIT_STR:
    case ASN_OCTET_STR:
    case ASN_OPAQUE:
    case ASN_NSAP:
    case ASN_PRIV_IMPLIED_OCTET_STR:{
      char strval[256];
      memset(strval, 0, 256);
      memcpy(strval, pvars->val.string, pvars->val_len);
      js->push_back(std::string(strval));
      break;
    }
    default:
      js->push_back(std::string(""));
      break;
  }
}

static void device_to_col_and_index(const std::string& name,
                                    std::string* colname,
                                    std::string* index) {
  auto pos = name.find(".");
  if(pos != name.npos) {
    *colname = name.substr(0,pos);
    *index = name.substr(pos + 1);
  } else {
    *colname = name;
    *index = "";
  }
}

size_t find_in_vec(const std::vector<std::string>& vec, const std::string& name) {

  size_t i = 0; bool found = false;
  for (; i != vec.size(); ++i) {
    if (name == vec[i]) {
      found = true; break;
    }
  }
  if(found) return i; else return name.npos;
}

}


} //namespace



