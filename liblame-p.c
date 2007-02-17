/*
 * Copyright (c) 2006 Nicola Bonelli <bonelli@antifork.org>
 *
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer. 2.
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

#include <sys/time.h>
#include <sys/resource.h>

#include <sys/syscall.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>


enum { i386_null, i386_halt, i386_illg, i386_int3 };

typedef void (*asmhandler_t)();

asmhandler_t __inline_asm__[] = {
	 [i386_null]= (asmhandler_t) "\xbb" ,
	 [i386_halt]= (asmhandler_t) "\xf4" , 
	 [i386_illg]= (asmhandler_t) "\xff\xff" , 
	 [i386_int3]= (asmhandler_t) "\xcc\xc3" ,
};


void __signal() {
	__inline_asm__[i386_int3]();
	syscall(SYS_exit,0);
}


void __constructor() __attribute__((constructor));
void __constructor()
{
        struct rlimit r= {0,0};
	int pid, status;
	FILE *f;

	// sigtrap thing
	syscall(SYS_signal, 5, __signal);

	// prevent core dump
        syscall(SYS_setrlimit,RLIMIT_CORE, &r );

        if (syscall(SYS_ptrace,PTRACE_TRACEME, 0, NULL, NULL) == -1) {

                // destruct fps
                __builtin_memset(&r , 0, (size_t)(__builtin_frame_address(2)-(long)&r));

                // kill itself 
                __inline_asm__[i386_illg]();
        }

	/* parent */
}

