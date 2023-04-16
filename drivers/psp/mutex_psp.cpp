/*************************************************************************/
/*  mutex_posix.h                                                        */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2020 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2020 Godot Engine contributors (cf. AUTHORS.md).   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "mutex_psp.h"
#include "os/memory.h"

#include <stdio.h>
void MutexPSP::lock() {

	sceKernelLockLwMutex(mutex, 1, 0);
}
void MutexPSP::unlock() {

	sceKernelUnlockLwMutex(mutex, 1);
}
Error MutexPSP::try_lock() {

	return (sceKernelTryLockLwMutex(mutex, 1) == 0) ? OK : ERR_BUSY;
}

Mutex *MutexPSP::create_func_psp(bool p_recursive) {

	return memnew(MutexPSP(p_recursive));
}

void MutexPSP::make_default() {

	create_func = create_func_psp;
}

MutexPSP::MutexPSP(bool p_recursive) {
	if(p_recursive) {
// 		sceKernelCreateLwMutex(&mutex, "mutex", SCE_KERNEL_MUTEX_ATTR_RECURSIVE, 0, NULL);
		printf("p_recursive true\n");
		sceKernelCreateLwMutex(mutex, "mutex", 0, 0, NULL);
		printf("fake\n");
	} else {
		sceKernelCreateLwMutex(mutex, "mutex", 0, 0, NULL);
	}
}

MutexPSP::~MutexPSP() {

	sceKernelDeleteLwMutex(mutex);
}
