#ifndef _UTIL_H_
#define _UTIL_H_

#if defined(__GNUC__)
#if defined(__GCC_HAVE_SYNC_COMPARE_AND_SWAP) 
#define CAS  __sync_bool_compare_and_swap 
#endif
//CAS2 
#if defined(__x86_64__)
#if  defined(__GCC_HAVE_SYNC_COMPARE_AND_SWAP_16) 
#define CAS2(p,o,n)  __sync_bool_compare_and_swap_16((long long *)(p), *(long long *)(&o), *(long long *)(&n)) 
#else 
#define CAS2(p,o,n)  CAS2_IMPL((long long *)(p), (long *)(&o), (long *)(&n)) 
inline 
bool CAS2_IMPL(long *addr, 
	       long * old_value, 
	       long * new_value) 
{ 
  bool  ret; 
  __asm__ __volatile__( 
            "lock cmpxchg16b %1;\n" 
            "sete %0;\n" 
            :"=m"(ret),"+m" (*(volatile long long *) (addr)) 
            :"a" (old_value[0]), "d" (old_value[1]), "b" (new_value[0]), "c" (new_value[1])); 
  return ret; 
} 
#endif
#else 
#if !defined(__GCC_HAVE_SYNC_COMPARE_AND_SWAP_8) 
#define CAS2(p,o,n)  __sync_bool_compare_and_swap_8((long long *)(p), *(long long *)(&o), *(long long *)(&n)) 
#else 
#define CAS2(p,o,n)  CAS2_IMPL((long long *)(p), (long *)(&o), (long *)(&n)) 
inline 
bool CAS2_IMPL(long long *addr, 
	       long * old_value, 
	       long * new_value) 
{ 
  bool  ret; 
  __asm__ __volatile__( 
            "lock cmpxchg8b %1;\n" 
            "sete %0;\n" 
            :"=m"(ret),"+m" (*(volatile long long *) (addr)) 
            :"a" (old_value[0]), "d" (old_value[1]), "b" (new_value[0]), "c" (new_value[1])); 
  return ret; 
}
#endif
#endif //CAS2
#endif //__GNUC__

#endif
