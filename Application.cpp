#include <iostream>
#include <chrono>
#include <Windows.h>

int screenWidth = 240; 		// console screen width
int screenHeight = 63;		// console screen height
int mapWidth = 16;		// map textfile width
int mapHeight = 16;		// map textfile height

float playerX = 5.0f;		// start pos x
float playerY = 4.0f;		// start pos y
float playerSpeed = 2.0f;

bool facingRight = true;

std::wstring map;
int main()
{
	wchar_t* screen = new wchar_t[screenWidth * screenHeight];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;
	map += L"################";
	map += L"#..............#";
	map += L"#.............##";
	map += L"###..####...#..#";
	map += L"#.......#...#..#";
	map += L"#.......#...##.#";
	map += L"#.......#...#..#";
	map += L"#######.#...#.##";
	map += L"#...........#..#";
	map += L"#...######..##.#";
	map += L"#...#....#..#..#";
	map += L"#...#....#..#.##";
	map += L"#...##DD###.#..#";
	map += L"#....##...####.#";
	map += L"#.....#........#";
	map += L"################";
	
	auto frameTime1 = std::chrono::system_clock::now();
	auto frameTime2 = std::chrono::system_clock::now();

	while (true)
	{
		frameTime2 = std::chrono::system_clock::now();
		std::chrono::duration<float> frameTime = frameTime2 - frameTime1;
		frameTime1 = frameTime2;
		float deltatime = frameTime.count();

		// Handle CCW Rotation
		if (GetAsyncKeyState((unsigned short)'A') & 0x8000 || (GetAsyncKeyState(VK_LEFT) & 0x8000))
		{
			playerX -= playerSpeed * deltatime;
			if (map.c_str()[(int)playerY * mapWidth + (int)playerX] == '#')
			{
				playerX += playerSpeed * deltatime;
			}
		}

		// Handle CW Rotation
		if (GetAsyncKeyState((unsigned short)'D') & 0x8000 || (GetAsyncKeyState(VK_RIGHT) & 0x8000))
		{
			playerX += playerSpeed * deltatime;
			if (map.c_str()[(int)playerY * mapWidth + (int)playerX] == '#')
			{
				playerX -= playerSpeed * deltatime;
			}
		}

		// Handle Forwards movement & collision
		if (GetAsyncKeyState((unsigned short)'W') & 0x8000 || (GetAsyncKeyState(VK_UP) & 0x8000))
		{
			playerY -= playerSpeed * deltatime;
			if (map.c_str()[(int)playerY * mapWidth + (int)playerX] == '#')
			{
				playerY += playerSpeed * deltatime;
			}
		}

		// Handle backwards movement & collision
		if ((GetAsyncKeyState((unsigned short)'S') & 0x8000) || (GetAsyncKeyState(VK_DOWN) & 0x8000))
		{
			playerY += playerSpeed * deltatime;
			if (map.c_str()[(int)playerY * mapWidth + (int)playerX] == '#')
			{
				playerY -= playerSpeed * deltatime;
			}
		}
		
		for (int x = 0; x < screenWidth; x++)
		{
			for (int y = 0; y < screenHeight; y++)
			{
				if(map.c_str()[int(playerY - 2.6665 + y / (9 * 1.5)) * mapWidth + int(playerX - 5 + x / (16 * 1.5))] == '#')
				{
					screen[y * screenWidth + x] = 0x2588;
				}
				else if (map.c_str()[int(playerY - 2.6665 + y / (9 * 1.5)) * mapWidth + int(playerX - 5 + x / (16 * 1.5))] == 'D')
				{
					screen[y * screenWidth + x] = '=';
				}
				else {
					screen[y * screenWidth + x] = ' ';
				}
				
			}
		}

		swprintf_s(screen, 40, L"X=%3.2f, Y=%3.2f FPS=%3.2f ", playerX, playerY, 1.0f / deltatime);
		for (int mapx = 0; mapx < mapWidth; mapx++)
		{
			for (int mapy = 0; mapy < mapWidth; mapy++)
			{
				screen[(mapy + 1) * screenWidth + mapx] = map[mapy * mapWidth + mapx];
			}
		}
		for (int i = -3; i < 6; i++)
		{
			for (int l = -2; l < 3; l++)
			{
				if (i < -1)
				{
					screen[(screenHeight / 2 + i) * screenWidth + screenWidth / 2 + l] = '#';
				}
				else {
					screen[(screenHeight / 2 + i) * screenWidth + screenWidth / 2 + l] = 0x2592;
				}
				
			}
		}
		screen[((int)playerY+1) * screenWidth + (int)playerX] = 'O';
		screen[screenWidth * screenHeight] = '\0';
		WriteConsoleOutputCharacter(hConsole, screen, screenWidth * screenHeight, { 0,0 }, &dwBytesWritten);
	}

	return 0;
}