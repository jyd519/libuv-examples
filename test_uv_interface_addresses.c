#include <stdio.h>
#include <stdlib.h>
#ifdef OS_UNIX
#include <unistd.h>
#endif
#include <assert.h>

#include <uv.h>

int main(int argc, char *argv[])
{
  char buffer[512];
  uv_interface_address_t* interfaces; 
  uv_cpu_info_t* cpus;
  int count;
  int i;
  int err;


  err = uv_cpu_info(&cpus, &count);
  assert(err == 0);

  printf("uv_cpu_info:\n");
  for (i = 0; i < count; i++) {
    printf("  model: %s\n", cpus[i].model);
    printf("  speed: %d\n", cpus[i].speed);
    printf("  times.sys: %llu\n", (unsigned long long) cpus[i].cpu_times.sys);
    printf("  times.user: %llu\n",
           (unsigned long long) cpus[i].cpu_times.user);
    printf("  times.idle: %llu\n",
           (unsigned long long) cpus[i].cpu_times.idle);
    printf("  times.irq: %llu\n",  (unsigned long long) cpus[i].cpu_times.irq);
    printf("  times.nice: %llu\n",
           (unsigned long long) cpus[i].cpu_times.nice);
  }
  uv_free_cpu_info(cpus, count);

  err = uv_interface_addresses(&interfaces, &count);
  assert(err == 0);

  printf("uv_interface_addresses:\n");
  for (i = 0; i < count; i++) {
    printf("  name: %s\n", interfaces[i].name);
    printf("  internal: %d\n", interfaces[i].is_internal);
    printf("  physical address: ");
    printf("%02x:%02x:%02x:%02x:%02x:%02x\n",
        (unsigned char)interfaces[i].phys_addr[0],
        (unsigned char)interfaces[i].phys_addr[1],
        (unsigned char)interfaces[i].phys_addr[2],
        (unsigned char)interfaces[i].phys_addr[3],
        (unsigned char)interfaces[i].phys_addr[4],
        (unsigned char)interfaces[i].phys_addr[5]);

    if (interfaces[i].address.address4.sin_family == AF_INET) {
      uv_ip4_name(&interfaces[i].address.address4, buffer, sizeof(buffer));
    } else if (interfaces[i].address.address4.sin_family == AF_INET6) {
      uv_ip6_name(&interfaces[i].address.address6, buffer, sizeof(buffer));
    }

    printf("  address: %s\n", buffer);

    if (interfaces[i].netmask.netmask4.sin_family == AF_INET) {
      uv_ip4_name(&interfaces[i].netmask.netmask4, buffer, sizeof(buffer));
      printf("  netmask: %s\n", buffer);
    } else if (interfaces[i].netmask.netmask4.sin_family == AF_INET6) {
      uv_ip6_name(&interfaces[i].netmask.netmask6, buffer, sizeof(buffer));
      printf("  netmask: %s\n", buffer);
    } else {
      printf("  netmask: none\n");
    }
  }
  uv_free_interface_addresses(interfaces, count);
  return 0;
}
