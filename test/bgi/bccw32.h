#ifndef __BCCW32_H
#define __BCCW32_H

/* W32 like delay() function */

void delay(int ms) {
  sleep(ms);
}

#endif
