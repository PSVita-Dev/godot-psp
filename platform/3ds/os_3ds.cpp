/*************************************************************************/
/*  OS_3DS.cpp                                                        */
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

#include "os_3ds.h"
#include "print_string.h"
#include "servers/physics/physics_server_sw.h"
#include "servers/visual/rasterizer_dummy.h"

#include "servers/visual/visual_server_raster.h"
#include <stdio.h>
#include <stdlib.h>
#include "drivers/unix/mutex_posix.h"
#include "drivers/unix/thread_posix.h"
#include "drivers/unix/semaphore_posix.h"
#include "drivers/unix/file_access_unix.h"
#include "drivers/unix/dir_access_unix.h"
#include "core/os/thread_dummy.h"
#include "drivers/unix/memory_pool_static_malloc.h"
#include "os/memory_pool_dynamic_static.h"

#include "main/main.h"

#include <unistd.h>
// #include <GL/gl3ds.h>
#include <sys/time.h>


int OS_3DS::get_video_driver_count() const {

	return 1;
}
const char *OS_3DS::get_video_driver_name(int p_driver) const {

	return "picaGL";
}
OS::VideoMode OS_3DS::get_default_video_mode() const {

	return OS::VideoMode(800, 480, false);
}
static MemoryPoolStaticMalloc *mempool_static=NULL;
static MemoryPoolDynamicStatic *mempool_dynamic=NULL;

void OS_3DS::initialize_core() {

	printf("init core\n");
 	ThreadPosix::make_default();
 	SemaphoreDummy::make_default();
 	MutexPosix::make_default();
	printf("init fs\n");

	ticks_start = 0;
	ticks_start = get_ticks_usec();

	mempool_static = new MemoryPoolStaticMalloc;
	mempool_dynamic = memnew( MemoryPoolDynamicStatic );

	FileAccess::make_default<FileAccessUnix>(FileAccess::ACCESS_RESOURCES);
	FileAccess::make_default<FileAccessUnix>(FileAccess::ACCESS_USERDATA);
	FileAccess::make_default<FileAccessUnix>(FileAccess::ACCESS_FILESYSTEM);
	//FileAccessBufferedFA<FileAccessUnix>::make_default();
	DirAccess::make_default<DirAccessUnix>(DirAccess::ACCESS_RESOURCES);
	DirAccess::make_default<DirAccessUnix>(DirAccess::ACCESS_USERDATA);
	DirAccess::make_default<DirAccessUnix>(DirAccess::ACCESS_FILESYSTEM);
}

void OS_3DS::finalize_core() {
	if (mempool_dynamic)
		memdelete( mempool_dynamic );
	delete mempool_static;
}


void OS_3DS::alert(const String &p_alert, const String &p_title) {
// 	printf(p_alert);
}
String OS_3DS::get_stdin_string(bool p_block) {
	return "";
}

Error OS_3DS::execute(const String &p_path, const List<String> &p_arguments, bool p_blocking, ProcessID *r_child_id, String *r_pipe, int *r_exitcode, bool read_stderr) {
	return FAILED;
}

Error OS_3DS::kill(const ProcessID &p_pid) {
	return FAILED;
}
bool OS_3DS::has_environment(const String &p_var) const {
	return false;
}
String OS_3DS::get_environment(const String &p_var) const {
	return "";
}

OS::Date OS_3DS::get_date(bool local) const {
	Date ret;
	return ret;
}

OS::Time OS_3DS::get_time(bool local) const {
	Time ret;
	return ret;
}

OS::TimeZoneInfo OS_3DS::get_time_zone_info() const{
	TimeZoneInfo ret;
	return ret;
}

void OS_3DS::delay_usec(uint32_t p_usec) const{
// 	printf("delay_usec: %lu\n", p_usec);
	usleep(p_usec);
}

uint64_t OS_3DS::get_ticks_usec() const{
	struct timeval tv_now;
	gettimeofday(&tv_now, NULL);

	uint64_t longtime = (uint64_t)tv_now.tv_usec + (uint64_t)tv_now.tv_sec * 1000000L;
	longtime -= ticks_start;

	return longtime;
}

void OS_3DS::vprint(const char* p_format, va_list p_list,bool p_stder)
{
	if (p_stder) {
		vfprintf(stderr,p_format,p_list);
		fflush(stderr);
	} else {
		vprintf(p_format,p_list);
		fflush(stdout);
	}
}

void OS_3DS::initialize(const VideoMode &p_desired, int p_video_driver, int p_audio_driver) {

// 	args = OS::get_singleton()->get_cmdline_args();
	current_videomode = p_desired;
	main_loop = NULL;

	init_keys();

// 	rasterizer_3DS = memnew(Rasterizer3DS);

	rasterizer = memnew(Rasterizer3DS);


	visual_server = memnew(VisualServerRaster(rasterizer));

// 	AudioDriverManagerSW::add_driver(&audio_driver);
 	AudioDriverManagerSW::initialize(p_audio_driver);


	sample_manager = memnew(SampleManagerMallocSW);
	audio_server = memnew(AudioServerSW(sample_manager));
	audio_server->init();
	spatial_sound_server = memnew(SpatialSoundServerSW);
	spatial_sound_server->init();
	spatial_sound_2d_server = memnew(SpatialSound2DServerSW);
	spatial_sound_2d_server->init();

	ERR_FAIL_COND(!visual_server);

	visual_server->init();

	//
	physics_server = memnew(PhysicsServerSW);
	physics_server->init();
	physics_2d_server = Physics2DServerWrapMT::init_server<Physics2DServerSW>();
	physics_2d_server->init();

	input = memnew(InputDefault);


	_ensure_data_dir();
}
void OS_3DS::finalize() {

	if (main_loop)
		memdelete(main_loop);
	main_loop = NULL;

	spatial_sound_server->finish();
	memdelete(spatial_sound_server);
	spatial_sound_2d_server->finish();
	memdelete(spatial_sound_2d_server);

	//if (debugger_connection_console) {
	//		memdelete(debugger_connection_console);
	//}

	memdelete(sample_manager);

	audio_server->finish();
	memdelete(audio_server);

	visual_server->finish();
	memdelete(visual_server);
	memdelete(rasterizer);

	physics_server->finish();
	memdelete(physics_server);

	physics_2d_server->finish();
	memdelete(physics_2d_server);

	memdelete(input);

	args.clear();
}

void OS_3DS::set_mouse_show(bool p_show) {
}
void OS_3DS::set_mouse_grab(bool p_grab) {

	grab = p_grab;
}
bool OS_3DS::is_mouse_grab_enabled() const {

	return grab;
}

int OS_3DS::get_mouse_button_state() const {

	return 0;
}

Point2 OS_3DS::get_mouse_pos() const {

	return Point2();
}

void OS_3DS::set_window_title(const String &p_title) {
}

void OS_3DS::set_video_mode(const VideoMode &p_video_mode, int p_screen) {
}
OS::VideoMode OS_3DS::get_video_mode(int p_screen) const {

	return current_videomode;
}

Size2 OS_3DS::get_window_size() const {

	return Vector2(current_videomode.width, current_videomode.height);
}

void OS_3DS::get_fullscreen_mode_list(List<VideoMode> *p_list, int p_screen) const {
}

const char *OS_3DS::get_audio_driver_name(int p_driver) const{
	return "3DS";
}
int OS_3DS::get_audio_driver_count() const{
	return 0;
}

MainLoop *OS_3DS::get_main_loop() const {

	return main_loop;
}

void OS_3DS::init_keys() {
	// sceCtrlSetSamplingCycle(0);
	// sceCtrlSetSamplingMode(3DS_CTRL_MODE_ANALOG);
}

// 3DSCtrlButtons buttons[16] = {
// 		3DS_CTRL_CROSS,
// 		3DS_CTRL_CIRCLE,
// 		3DS_CTRL_SQUARE,
// 		3DS_CTRL_TRIANGLE,
// 		(3DSCtrlButtons)0,
// 		(3DSCtrlButtons)0,
// 		3DS_CTRL_LTRIGGER,
// 		3DS_CTRL_RTRIGGER,
// 		(3DSCtrlButtons)0,
// 		(3DSCtrlButtons)0,
// 		3DS_CTRL_SELECT,
// 		3DS_CTRL_START,
// 		3DS_CTRL_UP,
// 		3DS_CTRL_DOWN,
// 		3DS_CTRL_LEFT,
// 		3DS_CTRL_RIGHT
// };
/*
void OS_3DS::process_keys() {
	// sceCtrlReadBufferPositive(&pad, 1);

	last++;

	for(int i = 0; i < 16; i++) {
		if (pad.Buttons & buttons[i]) {
			last = input->joy_button(last, 0, i, true);
		} else {
			last = input->joy_button(last, 0, i, false);
		}
	}

	InputDefault::JoyAxis ly, lx;

	lx.value = (pad.Lx - 128);
	ly.value = (pad.Ly - 128);

	// printf("%d %d\n", lx.value, pad.Lx);
	// printf("%d\n", ly.value);

	input->joy_axis(0, 0, 0, lx);
	input->joy_axis(0, 0, 1, ly);

	// if(pad.Buttons & 3DS_CTRL_HOME)
		// sceKernelExitGame();
}*/

void OS_3DS::delete_main_loop() {

	if (main_loop)
		memdelete(main_loop);
	main_loop = NULL;
}

void OS_3DS::set_main_loop(MainLoop *p_main_loop) {

	main_loop = p_main_loop;
	input->set_main_loop(p_main_loop);
}

bool OS_3DS::can_draw() const {

	return true; //can draw
};

String OS_3DS::get_name() {

	return "Nintendo 3DS";
}

void OS_3DS::move_window_to_foreground() {
}

void OS_3DS::set_cursor_shape(CursorShape p_shape) {
}

void OS_3DS::set_custom_mouse_cursor(const RES &p_cursor, CursorShape p_shape, const Vector2 &p_hotspot) {
}

void OS_3DS::run() {

	force_quit = false;

	if (!main_loop)
		return;

	main_loop->init();

	while (aptMainLoop()) {

		// process_keys();
// 		visual_server->draw();

		if (Main::iteration() == true)
			break;
	};

	main_loop->finish();
}

void OS_3DS::swap_buffers() {
	pglSwapBuffers();
}

OS_3DS::OS_3DS() {
	osSetSpeedupEnable(true);
	// AudioDriverManagerSW::add_driver(&audio_driver);
	//adriver here
 	_render_thread_mode=RENDER_THREAD_UNSAFE;;
	grab = false;

	// set_low_processor_usage_mode(true);
};
