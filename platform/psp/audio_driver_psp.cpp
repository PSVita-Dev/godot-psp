/*************************************************************************/
/*  audio_driver_psp.cpp                                                 */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                    http://www.godotengine.org                         */
/*************************************************************************/
/* Copyright (c) 2007-2016 Juan Linietsky, Ariel Manzur.                 */
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

#include "audio_driver_psp.h"
#include "core/os/thread.h"
#include "globals.h"
#include "os/os.h"
#include <string.h>

#define PPSSPP

static int channel_num = 1;

int32_t* buffer;

Error AudioDriverPSP::init() {
	thread_exited=false;
	exit_thread=false;
	pcm_open = false;
	samples_in = NULL;

	mix_rate = 44100;
	output_format = OUTPUT_STEREO;
	channels = 2;

	int latency = GLOBAL_DEF("audio/output_latency",25);
 	buffer_size = next_power_of_2( latency * mix_rate / 1000 );

	samples_in = memnew_arr(int32_t, buffer_size*channels);
	samples_out = memnew_arr(int16_t, buffer_size*channels);

	channel_num = sceAudioChReserve(1, PSP_AUDIO_SAMPLE_ALIGN(buffer_size*channels), PSP_AUDIO_FORMAT_MONO);

	mutex = Mutex::create();
	thread = Thread::create(AudioDriverPSP::thread_func, this);


	return OK;
};

void AudioDriverPSP::thread_func(void *p_udata) {

	int buffer_index = 0;
	printf("out\n");
 	AudioDriverPSP* ad = (AudioDriverPSP*)p_udata;

	int sample_count = ad->buffer_size ;
	uint64_t usdelay = (ad->buffer_size / float(ad->mix_rate)) * 1000000;

 	while (!ad->exit_thread) {


 		if (ad->exit_thread)
 			break;
#ifndef PPSSPP
		while(sceAudioWaitInputEnd()) {
			OS::get_singleton()->delay_usec(usdelay);
		}
#endif
		if (ad->active) {
			ad->lock();

			ad->audio_server_process(ad->buffer_size, ad->samples_in);

			ad->unlock();

			for(int i = 0; i < sample_count; ++i) {
				ad->samples_out[i] = ad->samples_in[i] >> 16;
			}


		} else
		{
			for (int i = 0; i < sample_count; i++) {

				ad->samples_out[i] = 0;
			}
		}
#ifdef PPSSPP
		OS::get_singleton()->delay_usec(usdelay);
#endif
		sceAudioOutput(channel_num, 0x8000, ad->samples_out);
	}


	ad->thread_exited=true;
};

void AudioDriverPSP::start() {
	printf("psp audio start\n");
	active = true;
};

int AudioDriverPSP::get_mix_rate() const {

	return mix_rate;
};

AudioDriverSW::OutputFormat AudioDriverPSP::get_output_format() const {

	return output_format;
};
void AudioDriverPSP::lock() {

	if (!thread || !mutex)
 		return;
 	mutex->lock();
};
void AudioDriverPSP::unlock() {

	if (!thread || !mutex)
		return;
	mutex->unlock();
};

void AudioDriverPSP::finish() {
	exit_thread = true;
 	Thread::wait_to_finish(thread);

	sceAudioChRelease(channel_num);

	if (samples_in) {
 		memdelete_arr(samples_in);
 	};
	if (samples_out) {
 		memdelete_arr(samples_out);
 	};


 	if (mutex)
 		memdelete(mutex);

};

AudioDriverPSP::AudioDriverPSP() {

 	mutex = NULL;
	active = false;
	id = NULL;
};

AudioDriverPSP::~AudioDriverPSP() {

};
