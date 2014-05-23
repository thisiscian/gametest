#ifndef game_drawer_CIAN
#define game_drawer_CIAN
	#include "opengl.h"
	#include "shader.h"
	class Drawer {
		private:
			GLuint vertexBuffer, colourBuffer;
			GLuint programID, vertexArrayID;
			GLfloat *vertexData, *colourData;
			int promise, count;
			GLFWwindow* window;
		public:
			Drawer(GLFWwindow* w, int p);
			void addTriangle(glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec3 col);
			void addSquare(glm::vec2 C, float w, float h, glm::vec3 col);
			void draw();
			bool escape();
	};

#endif
