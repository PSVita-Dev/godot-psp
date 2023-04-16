/*************************************************************************/
/*  semaphore_posix.cpp                                                  */
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

#include "semaphore_psp.h"
#include "os/memory.h"
#include <errno.h>
#include <stdio.h>

Error SemaphorePSP::wait() {

	while (sceKernelWaitSema(sem, 1, 0)) {
		if (errno == EINTR) {
			errno = 0;
			continue;
		} else {
			perror("sem waiting");
			return ERR_BUSY;
		}
	}
	return OK;
}

Error SemaphorePSP::post() {

	return (sceKernelSignalSema(sem, 1) == 0) ? OK : ERR_BUSY;
}
int SemaphorePSP::get() const {

// 	int val;
	sceKernelPollSema(sem, 1);

	return 1;
}

Semaphore *SemaphorePSP::create_semaphore_psp() {

	return memnew(SemaphorePSP);
}

void SemaphorePSP::make_default() {

	create_func = create_semaphore_psp;
}

SemaphorePSP::SemaphorePSP() {

	sem = sceKernelCreateSema("godot sema", 0, 1, 1, 0);
}

SemaphorePSP::~SemaphorePSP() {

	sceKernelDeleteSema(sem);
}
