#include <iostream>
#include <assert.h>
#include <string>
#include <stdint.h>
#include <stdlib.h>
#include <raylib.h>
#include <cmath>
#include <complex>
#include <dlfcn.h>

#include "plug.h"

#define N 256
#define ARRAY_LEN(xs) sizeof(xs) / sizeof(xs[0])

typedef struct
{
	float left;
	float right;
} Frame;

Frame global_frames[4800 * 2] = {0};
size_t global_frames_count = 0;

std::complex<float> I(0.0, 1.0);
float pi = atan2f(1, 1) * 4;

float in[N];
std::complex<float> out[N];
float max_amp;

void fft(float in[], size_t stride, std::complex<float> out[], size_t n)
{
	assert(n > 0);

	if (n == 1)
	{
		out[0] = in[0];
		return;
	}

	fft(in, stride * 2, out, n / 2);
	fft(in + stride, stride * 2, out + n / 2, n / 2);

	for (size_t k = 0; k < n / 2; ++k)
	{
		float t = (float)k / n;
		std::complex<float> z = I * (-2 * pi * t);
		std::complex<float> v = std::exp(z) * out[k + n / 2];
		std::complex<float> e = out[k];

		out[k] = e + v;
		out[k + n / 2] = e - v;
	}
}

float amp(std::complex<float> z)
{
	float real = fabsf(z.real());
	float imag = fabsf(z.imag());

	if (real > imag)
		return real;
	return imag;
}

void callback(void *bufferData, unsigned int frames)
{
	if (frames < N)
		return;

	Frame *fs = static_cast<Frame *>(bufferData);

	for (size_t i = 0; i < frames; ++i)
	{
		in[i] = fs[i].left;
	}

	fft(in, 1, out, N);

	max_amp = 0.0f;
	for (size_t i = 0; i < frames; ++i)
	{
		float a = amp(out[i]);
		if (max_amp < a)
			max_amp = a;
	}
}

int main(void)
{
	const char* libplug_file_name = "./libplug.so";
	
	void *libplug = dlopen(libplug_file_name, RTLD_NOW);

	if(!libplug){
		std::cerr << "ERROR: could no load " << libplug_file_name << " : " << dlerror() << "\n";
		return 1;
	}

	using PlugFunc = void(*)();
	PlugFunc plug_hello = reinterpret_cast<PlugFunc>(dlsym(libplug, "plug_hello"));

	if(!plug_hello){
		std::cerr << "ERROR: could not find plug_hello symbol in " << libplug_file_name << " : " << dlerror() << "\n";
	}

	std::cout << "Working\n";

	plug_hello();

    return 0;

	int factor = 100;
	int screen_width = 16 * factor;
	int screen_height = 9 * factor;
	InitWindow(screen_width, screen_height, "Musilizer");

	InitAudioDevice();
	Music music = LoadMusicStream("./assets/NOOR — Background Nasheed.ogg");

	assert(music.stream.sampleSize == 16);
	assert(music.stream.channels == 2);

	std::cout << "music.frameCount = " << music.frameCount << "\n";
	std::cout << "music.stream.sampleRate = " << music.stream.sampleRate << "\n";
	std::cout << "music.stream.sampleSize = " << music.stream.sampleSize << "\n";
	std::cout << "music.stream.channels = " << music.stream.channels << "\n";

	PlayMusicStream(music);
	SetMusicVolume(music, 0.5f);
	AttachAudioStreamProcessor(music.stream, callback);

	while (!WindowShouldClose())
	{
		UpdateMusicStream(music);

		if (IsKeyPressed(KEY_SPACE))
		{
			if (IsMusicStreamPlaying(music))
			{
				PauseMusicStream(music);
			}
			else
			{
				ResumeMusicStream(music);
			}
		}

		BeginDrawing();
		ClearBackground(CLITERAL(Color){0x18, 0x18, 0x18, 0xFF});

		int w = GetRenderWidth();
		int h = GetRenderHeight();

		float cell_width = (float)2 * w / N;

		for (size_t i = 0; i < N / 2; ++i)
		{
			float t = amp(out[i]) / max_amp;
			DrawRectangle(i * cell_width, h / 2 - h / 2 * t, cell_width, h / 2 * t, RED);
		}

		/*for (size_t i = 0; i < global_frames_count; ++i) {
			float t = global_frames[i].left;

			if (t > 0) {
				DrawRectangle(i*cell_width, h/2 - h/2*t, 1, h/2*t, RED);
			}
			else {
				DrawRectangle(i*cell_width, h/2, 1, h/2*t, RED);
			}
		}*/
		EndDrawing();
	}

	CloseWindow();

	return 0;
}