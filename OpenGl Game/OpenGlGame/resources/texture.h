#pragma once

#include <glad/glad.h>
#include <stb_image.h>
#include <iostream>

namespace glg {
	enum class WrappingOption {
		Repeat = GL_REPEAT,
		MirroredRepeat = GL_MIRRORED_REPEAT,
		ClampToEdge = GL_CLAMP_TO_EDGE,
		ClampToBorder = GL_CLAMP_TO_BORDER
	};

	enum class FilterOption
	{
		Nearest = GL_NEAREST,
		Linear = GL_LINEAR,
		NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
		LinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
		NearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
		LinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR
	};

	enum class FilterType {
		Minifying = GL_TEXTURE_MIN_FILTER,
		Magnifying = GL_TEXTURE_MAG_FILTER
	};

	class Texture2D
	{
	public:
		unsigned int ID;
		union
		{
			unsigned int textureUnit;
			unsigned int TU;
		};

		Texture2D() {

		}

		Texture2D(const char* texturePath, int textureUnit, WrappingOption wrappingOption = WrappingOption::Repeat, FilterOption minFilterOption = FilterOption::NearestMipmapNearest, FilterOption magFilterOption = FilterOption::Nearest, int bias = -2) {
			this->textureUnit = textureUnit;
			unsigned int texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			setWrappingOption(wrappingOption);
			setfilterOptions(minFilterOption, FilterType::Minifying);
			setfilterOptions(magFilterOption, FilterType::Magnifying);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, bias);

			int width, height, nrChannels;
			stbi_set_flip_vertically_on_load(true);
			unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);

			if (data) {
				int format = -1;
				if (nrChannels == 1)
				{
					format = GL_RED;
				}
				else if (nrChannels == 3) {
					format = GL_RGB;
				}
				else if (nrChannels == 4) {
					format = GL_RGBA;
				}
				else {
					std::cout << "Failed to load texture: Invailed amount of color channels";
					return;
				}

				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			else {
				std::cout << "Failed to load texture: " << std::string(texturePath) << std::endl;
				return;
			}
			stbi_image_free(data);
			ID = texture;
			this->width = width;
			this->height = height;
			colorChannels = nrChannels;
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void activate() const {
			glActiveTexture(GL_TEXTURE0 + TU);
			glBindTexture(GL_TEXTURE_2D, ID);
		}

		void activate(int textureUnit) const {
			glActiveTexture(GL_TEXTURE0 + textureUnit);
			glBindTexture(GL_TEXTURE_2D, ID);
		}

		int getWidth() {
			return width;
		}
		
		int getHeight() {
			return height;
		}

		int getColorChannels() {
			return colorChannels;
		}

	private:
		unsigned int width, height, colorChannels;
		void setWrappingOption(WrappingOption option) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (int)option);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (int)option);
			if (option == WrappingOption::ClampToBorder) {
				float borderColor[] = { 0, 0, 0, 0 };
				glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
			}
		}

		void setfilterOptions(FilterOption option, FilterType filterType) {
			glTexParameteri(GL_TEXTURE_2D, (int)filterType, (int)option);
		}
	};
}