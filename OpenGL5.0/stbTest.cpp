#include<iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
int main() {
	int width, height, nrChannels;
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	for (size_t i = 0; i < 50; i++)
	{
		std::cout << (int)data[i] << std::endl;
	}
	return 0;
}