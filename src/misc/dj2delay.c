/* Copyright (C) 2002 DJ Delorie, see COPYING.DJ for details */
/* Copyright (C) 1995 DJ Delorie, see COPYING.DJ for details */
#include <dos.h>
#include <dpmi.h>

#define TICK_PER_DAY (24*60*60*10000/182)

void delay(unsigned msec)
{
  __dpmi_regs r;

  while (msec)
  {
    unsigned usec;
    unsigned msec_this = msec;
    if (msec_this > 4000)
      msec_this = 4000;
    usec = msec_this * 1000;
    r.h.ah = 0x86;
    r.x.cx = usec>>16;
    r.x.dx = usec & 0xffff;
    __dpmi_int(0x15, &r);
    if ((r.x.flags & 1) || (r.h.ah == 0x83))
    {
      /* INT 15 FAILED, so fall back to the Time Of Day Tick */
      unsigned long start_tick;
      unsigned long end_tick;
 
      r.h.ah = 0x00;
      __dpmi_int(0x1A, &r);
 
      start_tick = (r.x.cx << 16) + (r.x.dx & 0xffff);
      end_tick = (msec*182)/10000 + start_tick;
 
      if ((msec%10000/182) > (5000/182)) /* Manual round ticks */
      {
	end_tick++;
      }
      if (end_tick > TICK_PER_DAY)  /* Tick time past midnight */
      {
	/* check for midnight */
	while (r.h.al == 0)
	{
	  r.h.ah = 0x00;
	  __dpmi_int(0x1A, &r);
	  __dpmi_yield();
	}
	end_tick = end_tick - TICK_PER_DAY;
      }
 
      while (((r.x.cx << 16) + (r.x.dx & 0xffffUL)) <= end_tick)
      {
	r.h.ah = 0x00;
	__dpmi_int(0x1A, &r);
	__dpmi_yield();
      }
      msec = 0;  /* waited the required time */
    }
    else
    {
      msec -= msec_this;
    }
  }
}
