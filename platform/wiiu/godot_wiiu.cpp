/*************************************************************************/
/*  godot_server.cpp                                                     */
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

// #define Thread Lib3dsThread
// #include <3ds.h>
// #include <GL/picaGL.h>
// #undef Thread

#include <whb/proc.h>
#include <whb/gfx.h>

#include "main/main.h"

#include <whb/gfx.h>
#include <whb/log_console.h>
#include <coreinit/debug.h>
#include <whb/sdcard.h>

#include "os_wiiu.h"
#include "CafeGLSLCompiler.h"

int main(int argc, char *argv[]) {
	WHBProcInit();
	WHBGfxInit();
	WHBLogConsoleInit();
	WHBMountSdCard();
	GLSL_Init();
    // ramfsInit();

	OSReport("Godot wiiu\n");
	OS_WIIU os;

	OSReport("Godot wiiu OS init\n");
	char* args[] = {"-path", WHBGetSdCardMountPath()};
	OSReport("setup\n");

	Error err = Main::setup("wiiu", 2, args, true);
    OSReport("setup\n");


	if (err==OK)
	{
		OSReport("Running...\n");

		if (Main::start())
	 		os.run(); // it is actually the OS that decides how to run
	 	Main::cleanup();
	}
	GLSL_Shutdown();
	WHBLogConsoleFree();
    WHBProcShutdown();

	return 0;
}
