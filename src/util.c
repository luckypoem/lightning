#include "util.h"
#include <uv.h>
#include "log/log.h"

void log_ipv4_and_port(void *ipv4, int port, const char *msg) {
  char data[INET_ADDRSTRLEN];
  uv_inet_ntop(AF_INET, ipv4, data, INET_ADDRSTRLEN);
  LOG_V("%s: %s:%d", msg, data, port);
}

void log_ipv6_and_port(void *ipv6, int port, const char *msg) {
  char data[INET6_ADDRSTRLEN];
  uv_inet_ntop(AF_INET6, ipv6, data, INET6_ADDRSTRLEN);
  LOG_V("%s: [%s]:%d", msg, data, port);
}

int fill_ipaddr(IPAddr *ipaddr, int port,  char *ipstr, 
    int ipstr_len, struct addrinfo *ai) {
  if (ai->ai_family == AF_INET) {
    ipaddr->addr4 = *(struct sockaddr_in *)ai->ai_addr;
    ipaddr->addr4.sin_port = port;
    uv_inet_ntop(ipaddr->addr.sa_family, &ipaddr->addr4.sin_addr, ipstr, ipstr_len);

  } else if (ai->ai_family == AF_INET6) {
    ipaddr->addr6 = *(struct sockaddr_in6 *)ai->ai_addr;
    ipaddr->addr6.sin6_port = port;
    uv_inet_ntop(ipaddr->addr.sa_family, &ipaddr->addr6.sin6_addr, ipstr, ipstr_len);

  } else {
    LOG_W("unexpected ai_family: %d", ai->ai_family);
    return -1;
  }

  return 0;
}

void copy_ipv4_addr(uint32_t *intip, const char *ip) {
  *intip = (ip[0] << 24) + (ip[1] << 16) + (ip[2] << 8) + ip[3];
}

int is_ipv4_addr_any(uint32_t ip) {
  return ip == 0;
}

int is_ipv4_addr_local(uint32_t ip) {
  return ip == 0x7f000001;
}

int is_ipv6_addr_any(const char *ip) {
  for (int i = 0; i < 16; ++i) {
    if (ip[i] != 0) {
      return 0;
    }
  }
  return 1;
}

int is_ipv6_addr_local(const char *ip) {
  for (int i = 0; i < 15; ++i) {
    if (ip[i] != 0) {
      return 0;
    }
  }
  return ip[15] == 1;
}
