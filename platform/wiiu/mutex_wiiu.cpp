/*************************************************************************/
/*  mutex_posix.cpp                                                      */
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

#include "mutex_wiiu.h"
#include "os/memory.h"

#if defined(__WIIU__)

void MutexWiiu::lock() {

	mutex.lock();
}
void MutexWiiu::unlock() {

	mutex.unlock();
}
Error MutexWiiu::try_lock() {

	return mutex.try_lock() ? OK : ERR_BUSY;
}

Mutex *MutexWiiu::create_func_wiiu(bool p_recursive) {

	return memnew(MutexWiiu(p_recursive));
}

void MutexWiiu::make_default() {

	create_func = create_func_wiiu;
}

MutexWiiu::MutexWiiu(bool p_recursive) {

	// pthread_mutexattr_init(&attr);
	// if (p_recursive)
	// 	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	// pthread_mutex_init(&mutex, &attr);
	if(!p_recursive)
		printf("non recursive thread!\n");
}

MutexWiiu::~MutexWiiu() {

	// pthread_mutex_destroy(&mutex);
}

#endif
