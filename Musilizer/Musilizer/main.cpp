#include <iostream>
#include <raylib.h>

void callback(void* bufferData, unsigned int frames) 
{
	(void)bufferData;
	// std::cout << "callback(frames = " << frames << ")\n";
}

int main(void)
{
	int factor = 100;
	int screen_height = 9 * factor;
	int screen_width = 16 * factor;
	InitWindow(screen_width, screen_height, "Musilizer");

	InitAudioDevice();
	Music music = LoadMusicStream("Verve - Benjamin Martins.mp3");
	
	std::cout << "music.frameCount = " << music.frameCount << "\n";
	std::cout << "music.stream.sampleRate = " << music.stream.sampleRate << "\n";
	std::cout << "music.stream.sampleSize = " << music.stream.sampleSize << "\n";
	std::cout << "music.stream.channels = " << music.stream.channels << "\n";

	PlayMusicStream(music);
	SetMusicVolume(music, 0.5f);
	AttachAudioStreamProcessor(music.stream, callback);

	while (!WindowShouldClose()) {
		UpdateMusicStream(music);

		while (IsKeyPressed(KEY_SPACE)) {
			if (IsMusicStreamPlaying(music)) {
				PauseMusicStream(music);
			}
			else {
				ResumeMusicStream(music);
			}
		}

		BeginDrawing();
			ClearBackground(RED);
		EndDrawing();
	}

	CloseWindow();

	return 0;
}