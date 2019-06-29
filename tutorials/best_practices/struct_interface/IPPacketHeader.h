#include "HLS/ac_int.h"
#include <iostream>

const int PACKET_LENGTH    = 160;
const int BITS_VERSION     = 4;
const int BITS_IHL         = 4;
const int BITS_DSCP        = 6;
const int BITS_ECN         = 2;
const int BITS_TOTAL_LENGTH= 16;
const int BITS_ID          = 16;
const int BITS_FLAG        = 3;
const int BITS_FO          = 13;
const int BITS_TTL         = 8;
const int BITS_PROTOCAL    = 8;
const int BITS_CHECKSUM    = 16;
const int BITS_SRC_IP      = 32;
const int BITS_DST_IP      = 32;

const int OFFSET_VERSION     = PACKET_LENGTH      - BITS_VERSION;
const int OFFSET_IHL         = OFFSET_VERSION     - BITS_IHL;
const int OFFSET_DSCP        = OFFSET_IHL         - BITS_DSCP;
const int OFFSET_ECN         = OFFSET_DSCP        - BITS_ECN;
const int OFFSET_TOTAL_LENTH = OFFSET_ECN         - BITS_TOTAL_LENGTH ;
const int OFFSET_ID          = OFFSET_TOTAL_LENTH - BITS_ID;
const int OFFSET_FLAG        = OFFSET_ID          - BITS_FLAG;
const int OFFSET_FO          = OFFSET_FLAG        - BITS_FO ;
const int OFFSET_TTL         = OFFSET_FO          - BITS_TTL;
const int OFFSET_PROTOCAL    = OFFSET_TTL         - BITS_PROTOCAL;
const int OFFSET_CHECKSUM    = OFFSET_PROTOCAL    - BITS_CHECKSUM;
const int OFFSET_SRC_IP      = OFFSET_CHECKSUM    - BITS_SRC_IP;
const int OFFSET_DST_IP      = OFFSET_SRC_IP      - BITS_DST_IP;

typedef ac_int<4, false>    version_t;
typedef ac_int<4, false>    ILH_t;
typedef ac_int<6, false>    DSCP_t;
typedef ac_int<2, false>    ECN_t;
typedef ac_int<16, false>   length_t;
typedef ac_int<16, false>   ID_t;
typedef ac_int<3, false>    flag_t;
typedef ac_int<13, false>   FO_t;
typedef ac_int<8, false>    TTL_t;
typedef ac_int<8, false>    protocal_t;
typedef ac_int<16, false>   checksum_t;
typedef ac_int<32 , false>  source_ip_t;
typedef ac_int<32 , false>  destination_ip_t;
typedef ac_int<160, false>  whole_packet_t;
/*
IPv4 Header struct:

Octet|Bits| 
    0|   0|   0 - 3   |   4 - 7   |    8 - 13    |14-15|           16 - 31                     |
     |    |  version  |    IHL    |     DSCP     | ECN |        total length                   |
    4|  32|                0 - 15                      |16-18|          19 - 31                |
     |    |         Identification (ID)                |Flags|       Fragment Offset           |
    8|  64|        0 - 7          |       8 - 15       |           16 - 31                     |
     |    |   Time to Live(TTL)   |     Protocol       |          Checksum                     |
   12|  96|                                      0 - 31                                        |
     |    |                       Source Addresss                                              |
   16| 128|                                      0 - 31                                        |
     |    |                       Destination Address                                          |
*/

#pragma pack(1)
class IPPacketHeader_padding {
private:
  version_t         version;
  ILH_t             ilh;
  DSCP_t            dscp;
  ECN_t             ecn;
  length_t          length;
  ID_t              ID;
  flag_t            flag;
  FO_t              FO;
  TTL_t             TTL;
  protocal_t        protocal;
  checksum_t        checksum;
  source_ip_t       s_ip;
  destination_ip_t  d_ip;

public:
  IPPacketHeader_padding(const char * str){
    whole_packet_t temp(str);
    set_version (temp.slc<BITS_VERSION     >(OFFSET_VERSION  ));
    set_ilh     (temp.slc<BITS_IHL         >(OFFSET_IHL      ));
    set_dscp    (temp.slc<BITS_DSCP        >(OFFSET_DSCP     ));
    set_ecn     (temp.slc<BITS_ECN         >(OFFSET_ECN      ));
    set_length  (temp.slc<BITS_TOTAL_LENGTH>(OFFSET_TOTAL_LENTH  ));
    set_ID      (temp.slc<BITS_ID          >(OFFSET_ID       ));
    set_flag    (temp.slc<BITS_FLAG        >(OFFSET_FLAG     ));
    set_FO      (temp.slc<BITS_FO          >(OFFSET_FO       ));
    set_TTL     (temp.slc<BITS_TTL         >(OFFSET_TTL      ));
    set_protocal(temp.slc<BITS_PROTOCAL    >(OFFSET_PROTOCAL ));
    set_checksum(temp.slc<BITS_CHECKSUM    >(OFFSET_CHECKSUM ));
    set_src_ip  (temp.slc<BITS_SRC_IP      >(OFFSET_SRC_IP   ));
    set_dst_ip  (temp.slc<BITS_DST_IP      >(OFFSET_DST_IP   ));
  }

  version_t get_version()      { return version;  }
  ILH_t get_ilh()              { return ilh;      }
  DSCP_t get_dscp()            { return dscp;     }
  ECN_t get_ecn()              { return ecn;      }
  length_t get_length()        { return length;   }
  ID_t get_ID()                { return ID;       }
  flag_t get_flag()            { return flag;     }
  FO_t get_FO()                { return FO;       }
  TTL_t get_TTL()              { return TTL;      }
  protocal_t get_protocal()    { return protocal; }
  checksum_t get_checksum()    { return checksum; }
  source_ip_t get_src_ip()     { return s_ip;     }
  destination_ip_t get_dst_ip(){ return d_ip;     }

  void set_version(version_t v)       { version = v;  }
  void set_ilh(ILH_t v)               { ilh = v;      }
  void set_dscp(DSCP_t v)             { dscp = v;     }
  void set_ecn(ECN_t v)               { ecn = v;      }
  void set_length(length_t v)         { length = v;   }
  void set_ID(ID_t v)                 { ID = v;       }
  void set_flag(flag_t v)             { flag = v;     }
  void set_FO(FO_t v)                 { FO = v;       }
  void set_TTL(TTL_t v)               { TTL = v;      }
  void set_protocal(protocal_t v)     { protocal = v; }
  void set_checksum(checksum_t v)     { checksum = v; }
  void set_src_ip(source_ip_t v)      { s_ip = v;     }
  void set_dst_ip(destination_ip_t v) { d_ip = v;     }

  unsigned long long hash_val(){
    return get_version().to_uint64()
         + get_ilh().to_uint64()
         + get_dscp().to_uint64()
         + get_ecn().to_uint64()
         + get_length().to_uint64()
         + get_ID().to_uint64()
         + get_flag().to_uint64()
         + get_FO().to_uint64()
         + get_TTL().to_uint64()
         + get_protocal().to_uint64()
         + get_checksum().to_uint64()
         + get_src_ip().to_uint64()
         + get_dst_ip().to_uint64();
  }

  void dump_info(){
    std::cout << "IPv4 Header:\n";
    std::cout << "VERSION: " << get_version().to_string(AC_HEX) << std::endl;
    std::cout << "ILH: " << get_ilh().to_string(AC_HEX) << std::endl;
    std::cout << "DSCP: " << get_dscp().to_string(AC_HEX) << std::endl;
    std::cout << "ECN: " << get_ecn().to_string(AC_HEX) << std::endl;
    std::cout << "LENGTH: " << get_length().to_string(AC_HEX) << std::endl;
    std::cout << "ID: " << get_ID().to_string(AC_HEX) << std::endl;
    std::cout << "FLAG: " << get_flag().to_string(AC_HEX) << std::endl;
    std::cout << "FO: " << get_FO().to_string(AC_HEX) << std::endl;
    std::cout << "TTL: " << get_TTL().to_string(AC_HEX) << std::endl;
    std::cout << "PROTOCAL: " << get_protocal().to_string(AC_HEX) << std::endl;
    std::cout << "CHECKSUM: " << get_checksum().to_string(AC_HEX) << std::endl;
    std::cout << "SOURCE IP: "<< get_src_ip().to_string(AC_HEX) << std::endl;
    std::cout << "DESTINATION IP: " << get_dst_ip().to_string(AC_HEX) << std::endl;
  }

};


class IPPacketHeader_no_padding{
private:
  whole_packet_t h;

public:
  IPPacketHeader_no_padding   (const char * str){
    whole_packet_t temp(str);
    h = temp;
  }

  version_t get_version()      { return h.slc<BITS_VERSION     >(OFFSET_VERSION  );     }
  ILH_t get_ilh()              { return h.slc<BITS_IHL         >(OFFSET_IHL      );     }
  DSCP_t get_dscp()            { return h.slc<BITS_DSCP        >(OFFSET_DSCP     );     }
  ECN_t get_ecn()              { return h.slc<BITS_ECN         >(OFFSET_ECN      );     }
  length_t get_length()        { return h.slc<BITS_TOTAL_LENGTH>(OFFSET_TOTAL_LENTH  ); }
  ID_t get_ID()                { return h.slc<BITS_ID          >(OFFSET_ID       );     }
  flag_t get_flag()            { return h.slc<BITS_FLAG        >(OFFSET_FLAG     );     }
  FO_t get_FO()                { return h.slc<BITS_FO          >(OFFSET_FO       );     }
  TTL_t get_TTL()              { return h.slc<BITS_TTL         >(OFFSET_TTL      );     }
  protocal_t get_protocal()    { return h.slc<BITS_PROTOCAL    >(OFFSET_PROTOCAL );     }
  checksum_t get_checksum()    { return h.slc<BITS_CHECKSUM    >(OFFSET_CHECKSUM );     }
  source_ip_t get_src_ip()     { return h.slc<BITS_SRC_IP      >(OFFSET_SRC_IP   );     }
  destination_ip_t get_dst_ip(){ return h.slc<BITS_DST_IP      >(OFFSET_DST_IP   );     }

  void set_version(version_t v)               { h.set_slc(OFFSET_VERSION, v);     }
  void set_ilh(ILH_t v)                       { h.set_slc(OFFSET_IHL, v);         }
  void set_dscp(DSCP_t v)                     { h.set_slc(OFFSET_DSCP, v);        }
  void set_ecn(ECN_t v)                       { h.set_slc(OFFSET_ECN, v);         }
  void set_length(length_t v)                 { h.set_slc(OFFSET_TOTAL_LENTH, v); }
  void set_ID(ID_t v)                         { h.set_slc(OFFSET_ID, v);          }
  void set_flag(flag_t v)                     { h.set_slc(OFFSET_FLAG, v);        }
  void set_FO(FO_t v)                         { h.set_slc(OFFSET_FO, v);          }
  void set_TTL(TTL_t v)                       { h.set_slc(OFFSET_TTL, v);         }
  void set_protocal(protocal_t v)             { h.set_slc(OFFSET_PROTOCAL, v);    }
  void set_checksum(checksum_t v)             { h.set_slc(OFFSET_CHECKSUM, v);    }
  void set_src_ip(source_ip_t v)              { h.set_slc(OFFSET_SRC_IP, v);      }
  void set_dst_ip(destination_ip_t v)         { h.set_slc(OFFSET_DST_IP, v);      }

  unsigned long long hash_val(){
    return get_version().to_uint64()
    + get_ilh().to_uint64()
    + get_dscp().to_uint64()
    + get_ecn().to_uint64()
    + get_length().to_uint64()
    + get_ID().to_uint64()
    + get_flag().to_uint64()
    + get_FO().to_uint64()
    + get_TTL().to_uint64()
    + get_protocal().to_uint64()
    + get_checksum().to_uint64()
    + get_src_ip().to_uint64()
    + get_dst_ip().to_uint64();
  }

  void dump_info(){
    std::cout << "IPv4 Header:\n";
    std::cout << "VERSION: " << get_version().to_string(AC_HEX) << std::endl;
    std::cout << "ILH: " << get_ilh().to_string(AC_HEX) << std::endl;
    std::cout << "DSCP: " << get_dscp().to_string(AC_HEX) << std::endl;
    std::cout << "ECN: " << get_ecn().to_string(AC_HEX) << std::endl;
    std::cout << "LENGTH: " << get_length().to_string(AC_HEX) << std::endl;
    std::cout << "ID: " << get_ID().to_string(AC_HEX) << std::endl;
    std::cout << "FLAG: " << get_flag().to_string(AC_HEX) << std::endl;
    std::cout << "FO: " << get_FO().to_string(AC_HEX) << std::endl;
    std::cout << "TTL: " << get_TTL().to_string(AC_HEX) << std::endl;
    std::cout << "PROTOCAL: " << get_protocal().to_string(AC_HEX) << std::endl;
    std::cout << "CHECKSUM: " << get_checksum().to_string(AC_HEX) << std::endl;
    std::cout << "SOURCE IP: "<< get_src_ip().to_string(AC_HEX) << std::endl;
    std::cout << "DESTINATION IP: " << get_dst_ip().to_string(AC_HEX) << std::endl;
  }

};