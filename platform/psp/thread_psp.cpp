/*************************************************************************/
/*  thread_posix.cpp                                                     */
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

#include "thread_psp.h"
#include "script_language.h"


#include "os/memory.h"

Thread::ID ThreadPSP::get_ID() const {

	return id;
}

Thread *ThreadPSP::create_thread_psp() {

	return memnew(ThreadPSP);
}

int ThreadPSP::thread_callback(SceSize args, void *argp) {

	ThreadPSP *t = reinterpret_cast<ThreadPSP *>(argp);
//  	t->id = id;
//
 	ScriptServer::thread_enter(); //scripts may need to attach a stack
//
 	t->callback(t->user);
//
	ScriptServer::thread_exit();

	return NULL;
}

Thread *ThreadPSP::create_func_psp(ThreadCreateCallback p_callback, void *p_user, const Settings &) {

	ThreadPSP *tr = memnew(ThreadPSP);
	tr->callback = p_callback;
	tr->user = p_user;
	int id = sceKernelCreateThread("godot th", thread_callback, 0x18, 0x10000, 0, NULL);
// 	pthread_attr_init(&tr->pthread_attr);
// 	pthread_attr_setdetachstate(&tr->pthread_attr, PTHREAD_CREATE_JOINABLE);
// 	pthread_attr_setstacksize(&tr->pthread_attr, 256 * 1024);

	tr->id = (ID)id;
	sceKernelStartThread(id, 0, NULL);
	return tr;
}
Thread::ID ThreadPSP::get_thread_ID_func_psp() {

	return (ID)sceKernelGetThreadId();
}
void ThreadPSP::wait_to_finish_func_psp(Thread *p_thread) {

	ThreadPSP *tp = static_cast<ThreadPSP *>(p_thread);
	ERR_FAIL_COND(!tp);
// 	ERR_FAIL_COND(tp->pthread == 0);

	sceKernelWaitThreadEnd(tp->id, 0);
// 	tp->pthread = 0;
}

Error ThreadPSP::set_name_func_psp(const String &p_name) {

	return ERR_UNAVAILABLE;
};

void ThreadPSP::make_default() {

	create_func = create_func_psp;
	get_thread_ID_func = get_thread_ID_func_psp;
	wait_to_finish_func = wait_to_finish_func_psp;
	set_name_func = set_name_func_psp;
}

ThreadPSP::ThreadPSP() {

// 	pthread = 0;
}

ThreadPSP::~ThreadPSP() {
}

