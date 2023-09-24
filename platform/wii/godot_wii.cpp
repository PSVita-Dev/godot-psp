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

// #include <whb/proc.h>
// #include <whb/gfx.h>

#include "main/main.h"

// #include <whb/gfx.h>
#include <gccore.h>
#include <wiiuse/wpad.h>

#include "os_wii.h"

int main(int argc, char *argv[]) {
	// WHBProcInit();
	// WHBGfxInit();
    // ramfsInit();

	VIDEO_Init();
	WPAD_Init();

	printf("Godot wii\n");
	OS_WII os;
	printf("Godot wii OS init\n");
	char* args[] = {"-path", "."};
	printf("setup\n");
	// while(1) { };

	Error err = Main::setup("wii", 2, args, true);
    printf("setup\n");

	// gx2glInit();
	// glutInit(&argc, argv);

	printf("glut UWU :3\n");

	if (err==OK)
	{
		printf("Running...\n");

		if (Main::start())
	 		os.run(); // it is actually the OS that decides how to run
	 	Main::cleanup();
	}
	// gx2glCleanup();
    // WHBProcShutdown();

	return 0;
}
