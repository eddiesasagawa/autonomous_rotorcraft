/**
 *
 * For now, just a simple test program for cross-compile testing
 *
 */

#include <stdio.h>

#include "arc.hh"

int main(int argc, char** argv) {
  if (argc != 5) {
    printf("usage: arc <MY_IP_ADDR> <MY_PORT> <HOST_IP_ADDR> <HOST_PORT>\n");
    return -1;
  }

  arc::arc::Arc arc_app(argv[1], argv[2], argv[3], argv[4]);
  arc_app.Spin();

  return 0;
}
