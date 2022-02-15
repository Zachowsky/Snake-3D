#include "TextureLoader.h"
#include "BmpLoader.h"
#include <vector>
#include <gl/glut.h>
#include <gl/freeglut.h>

TextureLoader::TextureLoader(const char* filename, std::vector <unsigned int> TextureID){
	BmpLoader bl(filename);
    TextureID.push_back(int());
    glGenTextures(1,&TextureID[TextureID.size()-1]);
    glBindTexture(GL_TEXTURE_2D, TextureID[TextureID.size()-1]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, bl.iWidth, bl.iHeight, GL_RGB, GL_UNSIGNED_BYTE, bl.textureData);
}
