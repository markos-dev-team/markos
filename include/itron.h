/* Copyright (C) 2019 Marko Nippula.
   All Rights Reserved.

   itron.h: "Micro Industrial TRON" basic definitions file.
   Provides partial compatibility with micro-itron 4.0 specification.
 */

#ifndef INCLUDE_GUARD_ITRON_DEFINED
#define INCLUDE_GUARD_ITRON_DEFINED 1

/* Get all standard header definitions.
   This header depends only on stddef.h and stdint.h,
   but single include is easier to replace globally.
   The includes should be before this point as this header will
   define preprocessor identifiers that are normally found in stddef.h. */
#include "stdheader.h"

/* Data types. */
typedef int8_t B;
typedef int16_t H;
typedef int32_t W;
typedef int64_t D;
typedef uint8_t UB;
typedef uint16_t UH;
typedef uint32_t UW;
typedef uint64_t UD;
typedef uint8_t VB;
typedef uint16_t VH;
typedef uint32_t VW;
typedef uint64_t VD;
typedef void *VP;
typedef intptr_t VP_INT;
typedef void (*FP)(VP_INT);
typedef int ATR;
typedef int ID;
typedef int INHNO;
typedef int PRI;
typedef size_t SIZE;
typedef int ER; /* Use full width value instead of 8 bits. */

/* Preprocessor identifiers, some of these overlap/conflict
   with stddef.h or other libraries. */
#ifndef NULL
#define NULL 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef E_OK
/* Error codes, according to micro-itron 4.0 specification.
   Not all of error codes are used. */
#define E_OK 0 /* Successful function execution. */
#define E_SYS -5 /* System error. */
#define E_NOSPT -9 /* Unsupported function. */
#define E_RSFN -10 /* Reserved function code. */
#define E_RSATR -11 /* Reserved attribute. */
#define E_PAR -17 /* Parameter error. */
#define E_ID -18 /* Invalid ID number. */
#define E_CTX -25 /* Wrong context. */
#define E_MACV -26 /* Memory access violation. */
#define E_OACV -27 /* Object access violation. */
#define E_ILUSE -28 /* Illegal service call use. */
#define E_NOMEM -33 /* Memory allocation failure. */
#define E_NOID -34 /* No ID number available. */
#define E_OBJ -41 /* Object state error. */
#define E_NOEXS -42 /* Non-existent object. */
#define E_QOVR -43 /* Queue overflow. */
#define E_RLWAI -49 /* Forced release from waiting. */
#define E_TMOUT -50 /* Polling failure or timeout. */
#define E_DLT -51 /* Waiting object deleted. */
#define E_CLS -52 /* Waiting object state changed. */
#define E_WBLK -57 /* Non-blocking call accepted. */
#define E_BOVR -58 /* Buffer overflow. */
#endif

#ifndef TA_NULL
#define TA_NULL 0 /* Object attribute unspecified. */
#endif

/* ITRON timeout */
#define TMO_POL 0 /* Polling */
#define TMO_FEVR -1 /* Waiting forever */
#define TMO_NBLK -2 /* Non-blocking */

#endif /* INCLUDE_GUARD_ITRON_DEFINED */
