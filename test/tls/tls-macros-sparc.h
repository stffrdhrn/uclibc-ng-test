#define TLS_LE(x) \
  ({ int *__l;								      \
     __asm__ ("sethi %%tle_hix22(" #x "), %0" : "=r" (__l));		      \
     __asm__ ("xor %1, %%tle_lox10(" #x "), %0" : "=r" (__l) : "r" (__l));	      \
     __asm__ ("add %%g7, %1, %0" : "=r" (__l) : "r" (__l));			      \
     __l; })

#ifdef __PIC__
# define TLS_LOAD_PIC \
  ({ register long pc __asm__ ("%o7");					      \
     long got;								      \
     __asm__ ("sethi %%hi(_GLOBAL_OFFSET_TABLE_-4), %1\n\t"			      \
	  "call .+8\n\t"						      \
	  "add %1, %%lo(_GLOBAL_OFFSET_TABLE_+4), %1\n\t"		      \
	  "add %1, %0, %1\n\t"						      \
	  : "=r" (pc), "=r" (got));					      \
     got; })
#else
# define TLS_LOAD_PIC \
   ({ long got;								      \
      __asm__ (".hidden _GLOBAL_OFFSET_TABLE_\n\t"				      \
	   "sethi %%hi(_GLOBAL_OFFSET_TABLE_), %0\n\t"			      \
	   "or %0, %%lo(_GLOBAL_OFFSET_TABLE_), %0"			      \
	   : "=r" (got));						      \
      got; })
#endif

#define TLS_IE(x) \
  ({ int *__l;								      \
     __asm__ ("sethi %%tie_hi22(" #x "), %0" : "=r" (__l));			      \
     __asm__ ("add %1, %%tie_lo10(" #x "), %0" : "=r" (__l) : "r" (__l));	      \
     __asm__ ("ld [%1 + %2], %0, %%tie_ld(" #x ")"				      \
	  : "=r" (__l) : "r" (TLS_LOAD_PIC), "r" (__l));		      \
     __asm__ ("add %%g7, %1, %0, %%tie_add(" #x ")" : "=r" (__l) : "r" (__l));    \
     __l; })

#define TLS_LD(x) \
  ({ int *__l; register void *__o0 __asm__ ("%o0");				      \
     long __o;								      \
     __asm__ ("sethi %%tldm_hi22(" #x "), %0" : "=r" (__l));		      \
     __asm__ ("add %1, %%tldm_lo10(" #x "), %0" : "=r" (__l) : "r" (__l));	      \
     __asm__ ("add %1, %2, %0, %%tldm_add(" #x ")"				      \
	  : "=r" (__o0) : "r" (TLS_LOAD_PIC), "r" (__l));		      \
     __asm__ ("call __tls_get_addr, %%tgd_call(" #x ")\n\t"			      \
	  " nop"							      \
	  : "=r" (__o0) : "0" (__o0)					      \
	  : "g1", "g2", "g3", "g4", "g5", "g6", "o1", "o2", "o3", "o4",	      \
	    "o5", "o7", "cc");						      \
     __asm__ ("sethi %%tldo_hix22(" #x "), %0" : "=r" (__o));		      \
     __asm__ ("xor %1, %%tldo_lox10(" #x "), %0" : "=r" (__o) : "r" (__o));	      \
     __asm__ ("add %1, %2, %0, %%tldo_add(" #x ")" : "=r" (__l)		      \
	  : "r" (__o0), "r" (__o));					      \
     __l; })

#define TLS_GD(x) \
  ({ int *__l; register void *__o0 __asm__ ("%o0");				      \
     __asm__ ("sethi %%tgd_hi22(" #x "), %0" : "=r" (__l));			      \
     __asm__ ("add %1, %%tgd_lo10(" #x "), %0" : "=r" (__l) : "r" (__l));	      \
     __asm__ ("add %1, %2, %0, %%tgd_add(" #x ")"				      \
	  : "=r" (__o0) : "r" (TLS_LOAD_PIC), "r" (__l));		      \
     __asm__ ("call __tls_get_addr, %%tgd_call(" #x ")\n\t"			      \
	  " nop"							      \
	  : "=r" (__o0) : "0" (__o0)					      \
	  : "g1", "g2", "g3", "g4", "g5", "g6", "o1", "o2", "o3", "o4",	      \
	    "o5", "o7", "cc");						      \
     __o0; })

