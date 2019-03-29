/* Copyright (C) 2019 Marko Nippula.
   All Rights Reserved.

   kernel_id.h: "Micro Industrial TRON" configuration.
   This file decides how many resources of each kind are available.
 */

#ifndef INCLUDE_GUARD_KERNEL_ID_DEFINED
#define INCLUDE_GUARD_KERNEL_ID_DEFINED 1

/* Note: all of the settings may be overridden via Makefile. */

/* Number of tasks to support.
   Defaults to 4 (+ dispatcher). */
#ifndef ITRON_TASKS
#define ITRON_TASKS 4
#endif /* ITRON_TASKS */

/* Number of tasks to support.
   Defaults to 2. */
#ifndef ITRON_MAILBOXES
#define ITRON_MAILBOXES 2
#endif /* ITRON_MAILBOXES */

/* Other configuration: */

/* Uncomment next line to remove interrupt handler capability.
   The interrupt handler capability is included by default for
   compliance with minimum specification of mitron-400e.pdf.
   However, in practice there are many uses where it is not needed. */
/* #define NO_IRQ_TICK */

#endif /* INCLUDE_GUARD_KERNEL_ID_DEFINED */
