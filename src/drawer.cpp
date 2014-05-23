#include"drawer.h"
using namespace glm;

Drawer::Drawer(GLFWwindow* w, int p) {
	promise=p;
	window=w;
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);
	programID=LoadShaders("shaders/simplevert.sdr", "shaders/simplefrag.sdr");

	vertexData=new GLfloat[promise*9];
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	colourData=new GLfloat[promise*9];
	glGenBuffers(1, &colourBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colourData), colourData, GL_STATIC_DRAW);
}

void Drawer::addTriangle(vec2 a, vec2 b, vec2 c, vec3 col) {
	if(count >= promise) { return; }
	vertexData[9*count+0]=a[0]; vertexData[9*count+1]=a[1]; vertexData[9*count+2]=0;
	vertexData[9*count+3]=b[0]; vertexData[9*count+4]=b[1]; vertexData[9*count+5]=0;
	vertexData[9*count+6]=c[0]; vertexData[9*count+7]=c[1]; vertexData[9*count+8]=0;
	for(int i=0; i<3; i++) {
		for(int j=0; j<3; j++) {
			colourData[9*count+3*i+j]=col[j];
		}
	}
	count++;
}
void Drawer::addSquare(vec2 C, float w, float h, vec3 col) {
	vec2 a= vec2(C[0]+w/2, C[1]+h/2);
	vec2 b= vec2(C[0]+w/2, C[1]-h/2);
	vec2 c= vec2(C[0]-w/2, C[1]-h/2);
	vec2 d= vec2(C[0]-w/2, C[1]+h/2);
	addTriangle(a,b,d,col);
	addTriangle(a,c,d,col);
}
void Drawer::draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(programID);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0 );

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
	glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0 );

	glDrawArrays(GL_TRIANGLES, 0, count*3);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glfwSwapBuffers(window);
	glfwPollEvents();
	count=0;
}
bool Drawer::escape() {
	return glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0;
};
