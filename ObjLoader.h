/*
 * ObjLoader.h
 *
 *  Created on: 2017年4月5日
 *      Author: xshi
 */

#ifndef OBJLOADER_H_
#define OBJLOADER_H_
#include <GL/glew.h>
#include <fstream>
#include <iostream>
#include <complex>
#include <cstdlib>
#include <string>
#include <vector>
class ObjLoader {
private:
	std::vector<GLfloat> points;
	std::vector<GLuint> indexes;
	GLuint pointNum;
	GLuint triangleNum;
public:
	ObjLoader(const GLchar* objPath) {
		std::ifstream objFile;
		objFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
			// Open files
			objFile.open(objPath);
			std::string line;
			std::getline(objFile, line);
			std::getline(objFile, line);
			int spaceLoc = line.find_first_of(' ');
			pointNum = std::stoi(
					line.substr(spaceLoc + 1, line.find(' ', spaceLoc + 1)));
			std::getline(objFile, line);
			std::getline(objFile, line);
			spaceLoc = line.find_first_of(' ');
			triangleNum = std::stoi(
					line.substr(spaceLoc + 1, line.find(' ', spaceLoc + 1)));
			while (!objFile.eof()) {
				std::getline(objFile, line);
				if (line[0] == 'g')
					break;
			}

			for (int i = 0; i < pointNum; i++) {
				std::getline(objFile, line);
				std::istringstream lines(line);
				std::string s;
				double a[3];
				lines >> s >> a[0] >> a[1] >> a[2];
				points.push_back(a[0]);
				points.push_back(a[1]);
				points.push_back(a[2]);
			}
			std::getline(objFile, line);
			for (int i = 0; i < triangleNum; i++) {
				std::getline(objFile, line);
				std::istringstream lines(line);
				std::string s;
				double a[3];
				lines >> s >> a[0] >> a[1] >> a[2];
				indexes.push_back(a[0]);
				indexes.push_back(a[1]);
				indexes.push_back(a[2]);
			}
			// close file handlers
			objFile.close();
		} catch (std::ifstream::failure e) {
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ"
					<< std::endl;
		}
	}
	GLuint getNumberOfTriangle() {
		return this->triangleNum;
	}

	GLfloat* getVertices() {
		GLfloat *vert = &this->points[0];
		return vert;
	}

	GLuint* getIndexes() {
			GLuint *index = &this->indexes[0];
			return index;
		}
};

#endif /* OBJLOADER_H_ */
