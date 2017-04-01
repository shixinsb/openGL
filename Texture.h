/*
 * Texture.h
 *
 *  Created on: 2017年4月1日
 *      Author: xshi
 */

#ifndef TEXTURE_H_
#define TEXTURE_H_
#include <SOIL.h>
#include <GL/glew.h>
#include <string>
#include <iostream>
class Texture {
	GLuint texture;
public:
	Texture(const GLchar* texturePath) {
		int width, height;
		unsigned char* image = SOIL_load_image(texturePath, &width, &height, 0,
				SOIL_LOAD_RGB);
		if (image == 0) {
			std::cerr << "ERROR::TEXTURE::SOIL::IMAGE_PATH_NOT_FOUND"
					<< std::endl;

		}
		glGenTextures(1, &this->texture);
		glBindTexture(GL_TEXTURE_2D, this->texture);
		// Set our texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Set texture wrapping to GL_REPEAT
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// Set texture filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
		GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	void use(GLuint program, unsigned int loc) {
		glBindTexture(GL_TEXTURE_2D, this->texture);
		std::string s = "ourTexture" + std::to_string(loc + 1);
		glUniform1i(
		glGetUniformLocation(program, s.c_str()), loc);
	}
};

#endif /* TEXTURE_H_ */
