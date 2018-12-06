/**
 * @file
 *
 * @brief This is the main entry point to the ARC Server application.
 *
 * @author Eddie Sasagawa
 * @date November 4, 2018
 *
 */

#include <stdio.h>
#include <iostream>
#include <string>

#include "arc_host.hh"

int main(int argc, char** argv) {
  if (argc != 5) {
    printf("usage: arc_host <MY_IP_ADDR> <MY_PORT> <ARC_IP_ADDR> <ARC_PORT>\n");
    return -1;
  }

  arc::host::ArcHost arc_host(argv[1], argv[2], argv[3], argv[4]);
  arc_host.Spin();

  return 0;
}