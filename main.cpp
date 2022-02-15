#include <gl/glut.h>
#include <gl/freeglut.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>
#include <time.h>

#include "BmpLoader.h"
#include "TextLoader.h"
#include "MAIN.h"

using namespace std;

vector <XY> tail;
vector <XY> food;


int pause = 1;
int score = 0;

bool KeyBuffer[256];
int ACTIVE_FRAME = 0;

void Display();
void Timer(int);
void KeyDown(unsigned char,int,int);
void KeyUp(unsigned char,int,int);
void ProcessFrame(int);

vector<unsigned int>TextureID;

void loadTexture(const char*filename){
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


int main(int argc, char*argv[]){

    srand(time(0));
    glutInit(&argc,argv);
    glutInitDisplayMode( GL_RGB | GL_DOUBLE | GL_DEPTH | GLUT_BORDERLESS);
    glutInitWindowPosition(600,200);
    glutInitWindowSize(700,700);
    glutCreateWindow("");

    glutDisplayFunc(Display);
    glutTimerFunc(0,Timer,0);
    glutKeyboardFunc(KeyDown);
    glutKeyboardUpFunc(KeyUp);


    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(26.0f,1.0f,0.1,2000);
    glClearColor(107.0f/255.0f,55.0f/255.0f,55.0f/255.0f,1.0f);
    glMatrixMode(GL_MODELVIEW);

    snake.speed.x = 3;
    snake.pos.z = 250;

    for(int i = 0; i<12; i++)food.push_back(XY());

    food[0].z = 250;food[1].z = 250;
    food[2].z = -250;food[3].z = -250;
    food[4].x = -250;food[5].x = -250;
    food[6].x = 250;food[7].x = 250;
    food[8].y = 250;food[9].y = 250;
    food[10].y= -250;food[11].y = -250;

    food[0].x = rand()%400 - 200;food[0].y = rand()%400 - 200;
    food[1].x = rand()%400 - 200;food[1].y = rand()%400 - 200;
    food[2].x = rand()%400 - 200;food[2].y = rand()%400 - 200;
    food[3].x = rand()%400 - 200;food[3].y = rand()%400 - 200;
    food[4].z = rand()%400 - 200;food[4].y = rand()%400 - 200;
    food[5].z = rand()%400 - 200;food[5].y = rand()%400 - 200;
    food[6].z = rand()%400 - 200;food[6].y = rand()%400 - 200;
    food[7].z = rand()%400 - 200;food[7].y = rand()%400 - 200;
    food[8].x = rand()%400 - 200;food[8].z = rand()%400 - 200;
    food[9].x = rand()%400 - 200;food[9].z = rand()%400 - 200;
    food[10].x = rand()%400 - 200;food[10].z = rand()%400 - 200;
    food[11].x = rand()%400 - 200;food[11].z = rand()%400 - 200;

    for(int i = 0; i<15; i++){
            tail.push_back(XY());
            tail.push_back(XY());
    }
    loadTexture("snake.bmp");
    loadTexture("apple.bmp");
    loadTexture("grass.bmp");
    loadTexture("body.bmp");

    glutMainLoop();
}


void Display(){
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glLoadIdentity();

    glTranslatef(0,0,-1500);
    glRotatef(angle.x,0,1,0);
    glRotatef(angle.y,1,0,0);

    //tlo
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,TextureID[2]);
    glColor3f(1,1,1);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f,0.0f);    glVertex3f(-250,-250,261);
        glTexCoord2f(1.0f,0.0f);    glVertex3f(250,-250,261);
        glTexCoord2f(1.0f,1.0f);    glVertex3f(250,250,261);
        glTexCoord2f(0.0f,1.0f);    glVertex3f(-250,250,261);

        glTexCoord2f(0.0f,0.0f);    glVertex3f(-250,-250,-240);
        glTexCoord2f(1.0f,0.0f);    glVertex3f(250,-250,-240);
        glTexCoord2f(1.0f,1.0f);    glVertex3f(250,250,-240);
        glTexCoord2f(0.0f,1.0f);    glVertex3f(-250,250,-240);

        glTexCoord2f(0.0f,0.0f);    glVertex3f(250,-250,-240);
        glTexCoord2f(0.0f,1.0f);    glVertex3f(250,250,-240);
        glTexCoord2f(1.0f,1.0f);    glVertex3f(250,250,261);
        glTexCoord2f(1.0f,0.0f);    glVertex3f(250,-250,261);

        glTexCoord2f(0.0f,0.0f);    glVertex3f(-250,-250,-240);
        glTexCoord2f(0.0f,1.0f);    glVertex3f(-250,250,-240);
        glTexCoord2f(1.0f,1.0f);    glVertex3f(-250,250,261);
        glTexCoord2f(1.0f,0.0f);    glVertex3f(-250,-250,261);

        glTexCoord2f(0.0f,0.0f);    glVertex3f(-250,250,-240);
        glTexCoord2f(0.0f,1.0f);    glVertex3f(250,250,-240);
        glTexCoord2f(1.0f,1.0f);    glVertex3f(250,250,261);
        glTexCoord2f(1.0f,0.0f);    glVertex3f(-250,250,261);

        glTexCoord2f(0.0f,0.0f);    glVertex3f(-250,-250,-240);
        glTexCoord2f(0.0f,1.0f);    glVertex3f(250,-250,-240);
        glTexCoord2f(1.0f,1.0f);    glVertex3f(250,-250,261);
        glTexCoord2f(1.0f,0.0f);    glVertex3f(-250,-250,261);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    ///jedzenie

    for(unsigned int i = 0; i<food.size(); i++){
        glColor3f(1,1,1);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,TextureID[1]);
        glBegin(GL_QUADS);

        glTexCoord2f(0.0,0.0);      glVertex3f(food[i].x-12.5f,food[i].y-12.5f,food[i].z);
        glTexCoord2f(1.0,0.0);      glVertex3f(food[i].x+12.5f,food[i].y-12.5f,food[i].z);
        glTexCoord2f(1.0,1.0);      glVertex3f(food[i].x+12.5f,food[i].y+12.5f,food[i].z);
        glTexCoord2f(0.0,1.0);      glVertex3f(food[i].x-12.5f,food[i].y+12.5f,food[i].z);
        glTexCoord2f(0.0,0.0);      glVertex3f(food[i].x-12.5f,food[i].y-12.5f,food[i].z+25);
        glTexCoord2f(1.0,0.0);      glVertex3f(food[i].x+12.5f,food[i].y-12.5f,food[i].z+25);
        glTexCoord2f(1.0,1.0);      glVertex3f(food[i].x+12.5f,food[i].y+12.5f,food[i].z+25);
        glTexCoord2f(0.0,1.0);      glVertex3f(food[i].x-12.5f,food[i].y+12.5f,food[i].z+25);
        glTexCoord2f(0.0,0.0);      glVertex3f(food[i].x-12.5f,food[i].y-12.5f,food[i].z);
        glTexCoord2f(1.0,0.0);      glVertex3f(food[i].x+12.5f,food[i].y-12.5f,food[i].z);
        glTexCoord2f(1.0,1.0);      glVertex3f(food[i].x+12.5f,food[i].y-12.5f,food[i].z+25);
        glTexCoord2f(0.0,1.0);      glVertex3f(food[i].x-12.5f,food[i].y-12.5f,food[i].z+25);
        glTexCoord2f(0.0,0.0);      glVertex3f(food[i].x+12.5f,food[i].y-12.5f,food[i].z);
        glTexCoord2f(1.0,0.0);      glVertex3f(food[i].x+12.5f,food[i].y+12.5f,food[i].z);
        glTexCoord2f(1.0,1.0);      glVertex3f(food[i].x+12.5f,food[i].y+12.5f,food[i].z+25);
        glTexCoord2f(0.0,1.0);      glVertex3f(food[i].x+12.5f,food[i].y-12.5f,food[i].z+25);
        glTexCoord2f(0.0,0.0);      glVertex3f(food[i].x+12.5f,food[i].y+12.5f,food[i].z);
        glTexCoord2f(1.0,0.0);      glVertex3f(food[i].x-12.5f,food[i].y+12.5f,food[i].z);
        glTexCoord2f(1.0,1.0);      glVertex3f(food[i].x-12.5f,food[i].y+12.5f,food[i].z+25);
        glTexCoord2f(0.0,1.0);      glVertex3f(food[i].x+12.5f,food[i].y+12.5f,food[i].z+25);
        glTexCoord2f(0.0,0.0);      glVertex3f(food[i].x-12.5f,food[i].y+12.5f,food[i].z);
        glTexCoord2f(1.0,0.0);      glVertex3f(food[i].x-12.5f,food[i].y-12.5f,food[i].z);
        glTexCoord2f(1.0,1.0);      glVertex3f(food[i].x-12.5f,food[i].y-12.5f,food[i].z+25);
        glTexCoord2f(0.0,1.0);      glVertex3f(food[i].x-12.5f,food[i].y+12.5f,food[i].z+25);

        glEnd();
       glDisable(GL_TEXTURE_2D);
    }

    ///glowa
    glColor3f(1,1,1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,TextureID[0]);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0,0.0);      glVertex3f(snake.pos.x-12.5f,snake.pos.y-12.5f,snake.pos.z);
        glTexCoord2f(1.0,0.0);      glVertex3f(snake.pos.x+12.5f,snake.pos.y-12.5f,snake.pos.z);
        glTexCoord2f(1.0,1.0);      glVertex3f(snake.pos.x+12.5f,snake.pos.y+12.5f,snake.pos.z);
        glTexCoord2f(0.0,1.0);      glVertex3f(snake.pos.x-12.5f,snake.pos.y+12.5f,snake.pos.z);
        glTexCoord2f(0.0,0.0);      glVertex3f(snake.pos.x-12.5f,snake.pos.y-12.5f,snake.pos.z+25);
        glTexCoord2f(1.0,0.0);      glVertex3f(snake.pos.x+12.5f,snake.pos.y-12.5f,snake.pos.z+25);
        glTexCoord2f(1.0,1.0);      glVertex3f(snake.pos.x+12.5f,snake.pos.y+12.5f,snake.pos.z+25);
        glTexCoord2f(0.0,1.0);      glVertex3f(snake.pos.x-12.5f,snake.pos.y+12.5f,snake.pos.z+25);
        glTexCoord2f(0.0,0.0);      glVertex3f(snake.pos.x-12.5f,snake.pos.y-12.5f,snake.pos.z);
        glTexCoord2f(1.0,0.0);      glVertex3f(snake.pos.x+12.5f,snake.pos.y-12.5f,snake.pos.z);
        glTexCoord2f(1.0,1.0);      glVertex3f(snake.pos.x+12.5f,snake.pos.y-12.5f,snake.pos.z+25);
        glTexCoord2f(0.0,1.0);      glVertex3f(snake.pos.x-12.5f,snake.pos.y-12.5f,snake.pos.z+25);
        glTexCoord2f(0.0,0.0);      glVertex3f(snake.pos.x+12.5f,snake.pos.y-12.5f,snake.pos.z);
        glTexCoord2f(1.0,0.0);      glVertex3f(snake.pos.x+12.5f,snake.pos.y+12.5f,snake.pos.z);
        glTexCoord2f(1.0,1.0);      glVertex3f(snake.pos.x+12.5f,snake.pos.y+12.5f,snake.pos.z+25);
        glTexCoord2f(0.0,1.0);      glVertex3f(snake.pos.x+12.5f,snake.pos.y-12.5f,snake.pos.z+25);
        glTexCoord2f(0.0,0.0);      glVertex3f(snake.pos.x+12.5f,snake.pos.y+12.5f,snake.pos.z);
        glTexCoord2f(1.0,0.0);      glVertex3f(snake.pos.x-12.5f,snake.pos.y+12.5f,snake.pos.z);
        glTexCoord2f(1.0,1.0);      glVertex3f(snake.pos.x-12.5f,snake.pos.y+12.5f,snake.pos.z+25);
        glTexCoord2f(0.0,1.0);      glVertex3f(snake.pos.x+12.5f,snake.pos.y+12.5f,snake.pos.z+25);
        glTexCoord2f(0.0,0.0);      glVertex3f(snake.pos.x-12.5f,snake.pos.y+12.5f,snake.pos.z);
        glTexCoord2f(1.0,0.0);      glVertex3f(snake.pos.x-12.5f,snake.pos.y-12.5f,snake.pos.z);
        glTexCoord2f(1.0,1.0);      glVertex3f(snake.pos.x-12.5f,snake.pos.y-12.5f,snake.pos.z+25);
        glTexCoord2f(0.0,1.0);      glVertex3f(snake.pos.x-12.5f,snake.pos.y+12.5f,snake.pos.z+25);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    ///cialo snejka
    for(unsigned int i = 12; i<tail.size()-1; i++){

    glColor3f(1,1,1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,TextureID[3]);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0,0.0);      glVertex3f(tail[i].x-12.5f,tail[i].y-12.5f,tail[i].z);
        glTexCoord2f(1.0,0.0);      glVertex3f(tail[i].x+12.5f,tail[i].y-12.5f,tail[i].z);
        glTexCoord2f(1.0,1.0);      glVertex3f(tail[i].x+12.5f,tail[i].y+12.5f,tail[i].z);
        glTexCoord2f(0.0,1.0);      glVertex3f(tail[i].x-12.5f,tail[i].y+12.5f,tail[i].z);
        glTexCoord2f(0.0,0.0);      glVertex3f(tail[i].x-12.5f,tail[i].y-12.5f,tail[i].z+25);
        glTexCoord2f(1.0,0.0);      glVertex3f(tail[i].x+12.5f,tail[i].y-12.5f,tail[i].z+25);
        glTexCoord2f(1.0,1.0);      glVertex3f(tail[i].x+12.5f,tail[i].y+12.5f,tail[i].z+25);
        glTexCoord2f(0.0,1.0);      glVertex3f(tail[i].x-12.5f,tail[i].y+12.5f,tail[i].z+25);
        glTexCoord2f(0.0,0.0);      glVertex3f(tail[i].x-12.5f,tail[i].y-12.5f,tail[i].z);
        glTexCoord2f(1.0,0.0);      glVertex3f(tail[i].x+12.5f,tail[i].y-12.5f,tail[i].z);
        glTexCoord2f(1.0,1.0);      glVertex3f(tail[i].x+12.5f,tail[i].y-12.5f,tail[i].z+25);
        glTexCoord2f(0.0,1.0);      glVertex3f(tail[i].x-12.5f,tail[i].y-12.5f,tail[i].z+25);
        glTexCoord2f(0.0,0.0);      glVertex3f(tail[i].x+12.5f,tail[i].y-12.5f,tail[i].z);
        glTexCoord2f(1.0,0.0);      glVertex3f(tail[i].x+12.5f,tail[i].y+12.5f,tail[i].z);
        glTexCoord2f(1.0,1.0);      glVertex3f(tail[i].x+12.5f,tail[i].y+12.5f,tail[i].z+25);
        glTexCoord2f(0.0,1.0);      glVertex3f(tail[i].x+12.5f,tail[i].y-12.5f,tail[i].z+25);
        glTexCoord2f(0.0,0.0);      glVertex3f(tail[i].x+12.5f,tail[i].y+12.5f,tail[i].z);
        glTexCoord2f(1.0,0.0);      glVertex3f(tail[i].x-12.5f,tail[i].y+12.5f,tail[i].z);
        glTexCoord2f(1.0,1.0);      glVertex3f(tail[i].x-12.5f,tail[i].y+12.5f,tail[i].z+25);
        glTexCoord2f(0.0,1.0);      glVertex3f(tail[i].x+12.5f,tail[i].y+12.5f,tail[i].z+25);
        glTexCoord2f(0.0,0.0);      glVertex3f(tail[i].x-12.5f,tail[i].y+12.5f,tail[i].z);
        glTexCoord2f(1.0,0.0);      glVertex3f(tail[i].x-12.5f,tail[i].y-12.5f,tail[i].z);
        glTexCoord2f(1.0,1.0);      glVertex3f(tail[i].x-12.5f,tail[i].y-12.5f,tail[i].z+25);
        glTexCoord2f(0.0,1.0);      glVertex3f(tail[i].x-12.5f,tail[i].y+12.5f,tail[i].z+25);
    glEnd();
    }


    glColor3f(0.6f, 0.8f, 0.8f);
    string score_ = to_string(score);
    string wynik = "WYNIK: ";

    TextLoader(wynik.data(), wynik.size(), 210, 480);
    TextLoader(score_.data(), score_.size(), 285, 480);

    if(pause%2==0){
        glColor3f(0, 0, 1);
        string pauza = "GRA ZATRZYMANA";
        TextLoader(pauza.data(), pauza.size(), 195, 250);
    }
    glutSwapBuffers();
}

void Timer(int){

    glutTimerFunc(1000/60,Timer,0);
    if(KeyBuffer['1'])angle.x+=4;
    if(KeyBuffer['3'])angle.x-=4;
    if(KeyBuffer['5'])angle.y+=4;
    if(KeyBuffer['2'])angle.y-=4;
    if(ACTIVE_FRAME == 0){
        if(angle.x >= 0.0f && angle.x <=180.0f)angle.x -= (angle.x-0.0f)/10.0f;
        if(angle.x <= 360.0f && angle.x > 180.0f)angle.x +=(360.0f-angle.x)/10.0f;
        if(angle.y >= 0.0f && angle.y <=180.0f)angle.y -= (angle.y-0.0f)/10.0f;
        if(angle.y <= 360.0f && angle.y > 180.0f)angle.y +=(360.0f-angle.y)/10.0f;
    }
    if(ACTIVE_FRAME == 1){
        if(angle.x >= 180.0f && angle.x <=360.0f)angle.x -= (angle.x-180.0f)/10.0f;
        if(angle.x < 180.0f && angle.x >=0.0f)angle.x += (180.0f-angle.x)/10.0f;
        if(angle.y >= 0.0f && angle.y <=180.0f)angle.y -= (angle.y-0.0f)/10.0f;
        if(angle.y <= 360.0f && angle.y > 180.0f)angle.y +=(360.0f-angle.y)/10.0f;
    }
    if(ACTIVE_FRAME == 2){
        if(angle.x >= 90.0f && angle.x <= 270.0f)angle.x -= (angle.x-90.0f)/10.0f;
        if(angle.x > 270.0f && angle.x <=360.0f)angle.x += ((360.0f-angle.x)+90.0f)/10.0f;
        if(angle.x <= 90.0f && angle.x >=0.0f)angle.x += (90.0f-angle.x)/10.0f;
        if(angle.y >= 0.0f && angle.y <=180.0f)angle.y -= (angle.y-0.0f)/10.0f;
        if(angle.y <= 360.0f && angle.y > 180.0f)angle.y +=(360.0f-angle.y)/10.0f;
    }
    if(ACTIVE_FRAME == 3){
        if(angle.x >= 90.0f && angle.x <= 270.0f)angle.x += (270.0f-angle.x)/10.0f;
        if(angle.x >=270.0f && angle.x <=360.0f)angle.x -= (angle.x-270.0f)/10.0f;
        if(angle.x >=0.0f && angle.x <90.0f)angle.x -= (angle.x+90.0f)/10.0f;
        if(angle.y >= 0.0f && angle.y <=180.0f)angle.y -= (angle.y-0.0f)/10.0f;
        if(angle.y <= 360.0f && angle.y > 180.0f)angle.y +=(360.0f-angle.y)/10.0f;
    }
    if(ACTIVE_FRAME == 4){
        if(angle.x >= 0.0f && angle.x <=180.0f)angle.x -= (angle.x-0.0f)/10.0f;
        if(angle.x <= 360.0f && angle.x > 180.0f)angle.x +=(360.0f-angle.x)/10.0f;
        if(angle.y >= 90.0f && angle.y <= 270.0f)angle.y -= (angle.y-90.0f)/10.0f;
        if(angle.y > 270.0f && angle.y <=360.0f)angle.y += ((360.0f-angle.y)+90.0f)/10.0f;
        if(angle.y <= 90.0f && angle.y >=0.0f)angle.y += (90.0f-angle.y)/10.0f;
    }
    if(ACTIVE_FRAME == 5){
        if(angle.x >= 0.0f && angle.x <=180.0f)angle.x -= (angle.x-0.0f)/10.0f;
        if(angle.x <= 360.0f && angle.x > 180.0f)angle.x +=(360.0f-angle.x)/10.0f;
        if(angle.y >= 90.0f && angle.y <= 270.0f)angle.y += (270.0f-angle.y)/10.0f;
        if(angle.y >=270.0f && angle.y <=360.0f)angle.y -= (angle.y-270.0f)/10.0f;
        if(angle.y >=0.0f && angle.y <90.0f)angle.y -= (angle.y+90.0f)/10.0f;
    }
    if(angle.x>=360)angle.x=angle.x-360;
    if(angle.x<0)angle.x=360+angle.x;
    if(angle.y>=360)angle.y=angle.y-360;
    if(angle.y<0)angle.y=360+angle.y;
    if(pause%2){
        snake.pos.x += snake.speed.x;
        snake.pos.y += snake.speed.y;
        snake.pos.z += snake.speed.z;
        ProcessFrame(ACTIVE_FRAME);
        tail[0].x=snake.pos.x;
        tail[0].y=snake.pos.y;
        tail[0].z=snake.pos.z;
        for(int i = tail.size()-1; i>=1; i--){
            tail[i].x = tail[i-1].x;
            tail[i].y = tail[i-1].y;
            tail[i].z = tail[i-1].z;
        }
        for(unsigned int i = 0; i<2; i++){
            if(snake.pos.x>food[i].x-25 && snake.pos.x<food[i].x+25 && snake.pos.y>food[i].y-25 && snake.pos.y<food[i].y+25 && snake.pos.z>food[i].z-25 && snake.pos.z<food[i].z+25){
                food[i].x = rand()%400;food[i].x-=200;
                food[i].y = rand()%400;food[i].y-=200;
                food[i].z = 250;
                for(int i = 0; i<=8; i++)tail.push_back(tail[tail.size()]);
                score ++;
            }
        }
        for(unsigned int i = 2; i<4; i++){
            if(snake.pos.x>food[i].x-25 && snake.pos.x<food[i].x+25 && snake.pos.y>food[i].y-25 && snake.pos.y<food[i].y+25 && snake.pos.z>food[i].z-25 && snake.pos.z<food[i].z+25){
                food[i].x = rand()%400;food[i].x-=200;
                food[i].y = rand()%400;food[i].y-=200;
                food[i].z = -250;
                for(int i = 0; i<=8; i++)tail.push_back(tail[tail.size()]);
                score ++;
            }
        }
        for(unsigned int i = 4; i<6; i++){
            if(snake.pos.x>food[i].x-25 && snake.pos.x<food[i].x+25 && snake.pos.y>food[i].y-25 && snake.pos.y<food[i].y+25 && snake.pos.z>food[i].z-25 && snake.pos.z<food[i].z+25){
                food[i].y = rand()%400;food[i].y-=200;
                food[i].z = rand()%400;food[i].z-=200;
                food[i].x = -250;
                for(int i = 0; i<=8; i++)tail.push_back(tail[tail.size()]);
                score ++;
            }
        }
        for(unsigned int i = 6; i<8; i++){
            if(snake.pos.x>food[i].x-25 && snake.pos.x<food[i].x+25 && snake.pos.y>food[i].y-25 && snake.pos.y<food[i].y+25 && snake.pos.z>food[i].z-25 && snake.pos.z<food[i].z+25){
                food[i].y = rand()%400;food[i].y-=200;
                food[i].z = rand()%400;food[i].z-=200;
                food[i].x = 250;
                for(int i = 0; i<=8; i++)tail.push_back(tail[tail.size()]);
                score ++;
            }
        }
        for(unsigned int i = 8; i<10; i++){
            if(snake.pos.x>food[i].x-25 && snake.pos.x<food[i].x+25 && snake.pos.y>food[i].y-25 && snake.pos.y<food[i].y+25 && snake.pos.z>food[i].z-25 && snake.pos.z<food[i].z+25){
                food[i].x = rand()%400;food[i].x-=200;
                food[i].z = rand()%400;food[i].z-=200;
                food[i].y = 250;
                for(int i = 0; i<=8; i++)tail.push_back(tail[tail.size()]);
                score ++;
            }
        }
        for(unsigned int i = 10; i<12; i++){
            if(snake.pos.x>food[i].x-25 && snake.pos.x<food[i].x+25 && snake.pos.y>food[i].y-25 && snake.pos.y<food[i].y+25 && snake.pos.z>food[i].z-25 && snake.pos.z<food[i].z+25){
                food[i].x = rand()%400;food[i].x-=200;
                food[i].z = rand()%400;food[i].z-=200;
                food[i].y = -250;
                for(int i = 0; i<=8; i++)tail.push_back(tail[tail.size()]);
                score ++;
            }
        }
    }

    if(KeyBuffer['q']){
        cout<<"Twoj wynik to: "<< score;
        exit(0);
    }
    glutPostRedisplay();
}

void KeyDown(unsigned char key, int, int){
    if(key == 27){
        pause++;
    }
    KeyBuffer[key] = true;
}

void KeyUp(unsigned char key, int, int){
    KeyBuffer[key] = false;
}

void ProcessFrame(int FRAME){
    if(FRAME == 0){
        if(KeyBuffer['w'] && snake.speed.x != 0.0f){
            snake.speed.x = 0.0f;
            snake.speed.y = 3.0f;
            }
        if(KeyBuffer['s'] && snake.speed.x != 0.0f){
            snake.speed.x = 0.0f;
            snake.speed.y = -3.0f;
            }
        if(KeyBuffer['a'] && snake.speed.y != 0.0f){
            snake.speed.y = 0.0f;
            snake.speed.x = -3.0f;
            }
        if(KeyBuffer['d'] && snake.speed.y != 0.0f){
            snake.speed.y = 0.0f;
            snake.speed.x = 3.0f;
            }
        if(snake.pos.x<-262 && snake.pos.z>249){
            snake.pos.x = -261;
            snake.speed.x = 0.0f;
            snake.speed.z = -3.0f;
            ACTIVE_FRAME = 2;
        }
        if(snake.pos.x>262 && snake.pos.z>249){
            snake.pos.x = 261;
            snake.speed.x = 0.0f;
            snake.speed.z = -3.0f;
            ACTIVE_FRAME = 3;
        }
        if(snake.pos.y<-262){
            snake.pos.y = -261;
            snake.speed.y = 0.0f;
            snake.speed.z = -3.0f;
            ACTIVE_FRAME = 5;
        }
        if(snake.pos.y>262){
            snake.pos.y = 261;
            snake.speed.y = 0.0f;
            snake.speed.z = -3.0f;
            ACTIVE_FRAME = 4;
        }
    }
    if(FRAME == 1){
        if(KeyBuffer['w'] && snake.speed.x != 0.0f){
            snake.speed.x = 0.0f;
            snake.speed.y = 3.0f;
            }
        if(KeyBuffer['s'] && snake.speed.x != 0.0f){
            snake.speed.x = 0.0f;
            snake.speed.y = -3.0f;
            }
        if(KeyBuffer['a'] && snake.speed.y != 0.0f){
            snake.speed.y = 0.0f;
            snake.speed.x = 3.0f;
            }
        if(KeyBuffer['d'] && snake.speed.y != 0.0f){
            snake.speed.y = 0.0f;
            snake.speed.x = -3.0f;
            }
        if(snake.pos.x<-262 && snake.pos.z<-249){
            snake.pos.x = -261;
            snake.speed.x = 0.0f;
            snake.speed.z = 3.0f;
            ACTIVE_FRAME = 2;
        }
        if(snake.pos.x>262 && snake.pos.z<-249){
            snake.pos.x = 261;
            snake.speed.x = 0.0f;
            snake.speed.z = 3.0f;
            ACTIVE_FRAME = 3;
        }
        if(snake.pos.y<-262){
            snake.pos.y = -261;
            snake.speed.y = 0.0f;
            snake.speed.z = 3.0f;
            ACTIVE_FRAME = 5;
        }
        if(snake.pos.y>262){
            snake.pos.y = 261;
            snake.speed.y = 0.0f;
            snake.speed.z = 3.0f;
            ACTIVE_FRAME = 4;
        }
    }
    if(FRAME == 2){
        if(KeyBuffer['w'] && snake.speed.z != 0.0f){
            snake.speed.z = 0.0f;
            snake.speed.y = 3.0f;
            }
        if(KeyBuffer['s'] && snake.speed.z != 0.0f){
            snake.speed.z = 0.0f;
            snake.speed.y = -3.0f;
            }
        if(KeyBuffer['a'] && snake.speed.y != 0.0f){
            snake.speed.y = 0.0f;
            snake.speed.z = -3.0f;
            }
        if(KeyBuffer['d'] && snake.speed.y != 0.0f){
            snake.speed.y = 0.0f;
            snake.speed.z = 3.0f;
            }
        if(snake.pos.z<-262 && snake.pos.x<-249){
            snake.pos.x = -261;
            snake.speed.x = 3.0f;
            snake.speed.z = 0.0f;
            ACTIVE_FRAME = 1;
        }
        if(snake.pos.z>262 && snake.pos.x<-249){
            snake.pos.z = 261;
            snake.speed.x = 3.0f;
            snake.speed.z = 0.0f;
            ACTIVE_FRAME = 0;
        }
        if(snake.pos.y>262 && snake.pos.x<-249){
            snake.pos.y = 261;
            snake.speed.y = 0.0f;
            snake.speed.x = 3.0f;
            ACTIVE_FRAME = 4;
        }
        if(snake.pos.y<-262 && snake.pos.x<-249){
            snake.pos.y = -261;
            snake.speed.y = 0.0f;
            snake.speed.x = 3.0f;
            ACTIVE_FRAME = 5;
        }
    }
    if(FRAME == 3){
        if(KeyBuffer['w'] && snake.speed.z != 0.0f){
            snake.speed.z = 0.0f;
            snake.speed.y = 3.0f;
            }
        if(KeyBuffer['s'] && snake.speed.z != 0.0f){
            snake.speed.z = 0.0f;
            snake.speed.y = -3.0f;
            }
        if(KeyBuffer['a'] && snake.speed.y != 0.0f){
            snake.speed.y = 0.0f;
            snake.speed.z = 3.0f;
            }
        if(KeyBuffer['d'] && snake.speed.y != 0.0f){
            snake.speed.y = 0.0f;
            snake.speed.z = -3.0f;
            }
        if(snake.pos.z<-262 && snake.pos.x>249){
            snake.pos.x = 261;
            snake.speed.x = -3.0f;
            snake.speed.z = 0.0f;
            ACTIVE_FRAME = 1;
        }
        if(snake.pos.z>262 && snake.pos.x>249){
            snake.pos.z = 261;
            snake.speed.x = -3.0f;
            snake.speed.z = 0.0f;
            ACTIVE_FRAME = 0;
        }
        if(snake.pos.y>262 && snake.pos.x>249){
            snake.pos.y = 261;
            snake.speed.y = 0.0f;
            snake.speed.x = -3.0f;
            ACTIVE_FRAME = 4;
        }
        if(snake.pos.y<-262 && snake.pos.x>249){
            snake.pos.y = -261;
            snake.speed.y = 0.0f;
            snake.speed.x = -3.0f;
            ACTIVE_FRAME = 5;
        }
    }
    if(FRAME == 4){
        if(KeyBuffer['w'] && snake.speed.x != 0.0f){
            snake.speed.z = -3.0f;
            snake.speed.x = 0.0f;
            }
        if(KeyBuffer['s'] && snake.speed.x != 0.0f){
            snake.speed.z = 3.0f;
            snake.speed.x = 0.0f;
            }
        if(KeyBuffer['a'] && snake.speed.z != 0.0f){
            snake.speed.z = 0.0f;
            snake.speed.x = -3.0f;
            }
        if(KeyBuffer['d'] && snake.speed.z != 0.0f){
            snake.speed.z = 0.0f;
            snake.speed.x = 3.0f;
            }
        if(snake.pos.z>262 && snake.pos.y>249){
            snake.pos.z = 261;
            snake.speed.y = -3.0f;
            snake.speed.z = 0.0f;
            ACTIVE_FRAME = 0;
        }
        if(snake.pos.z<-262 && snake.pos.y>249){
            snake.pos.z = -261;
            snake.speed.y = -3.0f;
            snake.speed.z = 0.0f;
            ACTIVE_FRAME = 1;
        }
        if(snake.pos.x<-262 && snake.pos.y>249){
            snake.pos.x = -261;
            snake.speed.y = -3.0f;
            snake.speed.x = 0.0f;
            ACTIVE_FRAME = 2;
        }
        if(snake.pos.x>262 && snake.pos.y>249){
            snake.pos.x = 261;
            snake.speed.y = -3.0f;
            snake.speed.x = 0.0f;
            ACTIVE_FRAME = 3;
        }
    }
    if(FRAME == 5){
        if(KeyBuffer['w'] && snake.speed.x != 0.0f){
            snake.speed.z = 3.0f;
            snake.speed.x = 0.0f;
            }
        if(KeyBuffer['s'] && snake.speed.x != 0.0f){
            snake.speed.z = -3.0f;
            snake.speed.x = 0.0f;
            }
        if(KeyBuffer['a'] && snake.speed.z != 0.0f){
            snake.speed.z = 0.0f;
            snake.speed.x = -3.0f;
            }
        if(KeyBuffer['d'] && snake.speed.z != 0.0f){
            snake.speed.z = 0.0f;
            snake.speed.x = 3.0f;
            }
        if(snake.pos.z>262 && snake.pos.y<-249){
            snake.pos.z = 261;
            snake.speed.y = 3.0f;
            snake.speed.z = 0.0f;
            ACTIVE_FRAME = 0;
        }
        if(snake.pos.z<-262 && snake.pos.y<-249){
            snake.pos.z = -261;
            snake.speed.y = 3.0f;
            snake.speed.z = 0.0f;
            ACTIVE_FRAME = 1;
        }
        if(snake.pos.x<-262 && snake.pos.y<-249){
            snake.pos.x = -261;
            snake.speed.y = 3.0f;
            snake.speed.x = 0.0f;
            ACTIVE_FRAME = 2;
        }
        if(snake.pos.x>262 && snake.pos.y<-249){
            snake.pos.x = 261;
            snake.speed.y = 3.0f;
            snake.speed.x = 0.0f;
            ACTIVE_FRAME = 3;
        }
    }
}
