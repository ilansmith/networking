#ifndef _SYSCALLSMAC_H_
#define _SYSCALLSMAC_H_

#define __syscall_return(type, res) do { \
	if ((unsigned long)(res) >= (unsigned long)(-(128 + 1))) { \
		errno = -(res); \
		res = -1; \
	} \
	return (type)(res); \
} while (0)
 
#define _syscall6(type, name, type1, arg1, type2, arg2, type3, arg3, type4, \
	arg4, type5, arg5, type6, arg6) \
static inline type name(type1 arg1, type2 arg2, type3 arg3, type4 arg4, \
	type5 arg5, type6 arg6) \
{ \
	long __res; \
	__asm__ volatile ("push %%bp ; mov %%ax, %%bp ; mov %1, %%ax ; " \
		"int $0x80 ; pop %%bp" \
		: "=a" (__res) \
		: "i" (__NR_##name), "b" ((long)(arg1)), "c" ((long)(arg2)), \
		"d" ((long)(arg3)), "S" ((long)(arg4)), "D" ((long)(arg5)), \
		"0" ((long)(arg6)) : "memory"); \
	__syscall_return(type, __res); \
}
 
_syscall6(int, futex, void *, addr1, int, op, int, val1, struct timespec*,
	timeout, void*, addr2, int, val2)

#endif /* _SYSCALLSMAC_H_ */

