#include "GLTextureView.h"
#include <cnoid/MessageView>
#include <QVBoxLayout>
#include <malloc.h>
#include <boost/bind.hpp>

using namespace cnoid;

#define STDERR(str) cnoid::MessageView::mainInstance()->putln(str)
//#define STDERR(str) OutputDebugString(L##str)
//#define STDERR(str) 

TextureScene::TextureScene(QWidget* parent)
    : QGLWidget(parent), tex_width(320), tex_height(280)
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

TextureScene::~TextureScene(void)
{
	glDeleteTextures(1, &texture);
}

bool TextureScene::setTime(double time)
{
    if(time < 33.0){
        update();
        return true;
    }
    return false;
}


void TextureScene::initializeGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	connect( &timer, SIGNAL(timeout()), this, SLOT(idle()) );
	if(!timer.isActive()){
		timer.start(33);
		MessageView::mainInstance()->putln(tr("[GLTexturePlugin] Initialize.\n"));
	}
}


void TextureScene::resizeGL(int width, int height)
{
	display_width = width;
	display_height = height;
}


void TextureScene::paintGL()
{
	//glClearColor(0.0f, 1.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, display_width, display_height);
	gluOrtho2D(0.0, (double)display_width, 0.0, (double)display_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	GLshort vertices[] = {
		0,	  0,
		display_width,	  0,
		0,	display_height,
		display_width,	display_height,
	};
	GLfloat texCoords[] = {
		(GLfloat)display_width/tex_width,	(GLfloat)display_height/tex_height,
		0.0f,	(GLfloat)display_height/tex_height,
		(GLfloat)display_width/tex_width,	0.0f,
		0.0f,	0.0f,
	};
	glDisable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_FLAT);
	glDisableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_SHORT, 0, vertices);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoords);

	glBindTexture(GL_TEXTURE_2D, texture);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisable(GL_TEXTURE_2D);
}


void TextureScene::idle()
{
	unsigned char *tmp = (unsigned char *)malloc(tex_width*tex_height*4*sizeof(unsigned char));

	typedef struct t_RGBA{
		byte r;
		byte g;
		byte b;
		byte a;
	};
	t_RGBA *p = (t_RGBA *)tmp;
	for(int y=0;y<tex_height;y++){
		for(int x=0;x<tex_width;x++){
			if(y<tex_height/2){
				p->r = (unsigned char)255;
			}else{
				p->r = (unsigned char)0;
			}
			if(x<tex_width/2){
				p->g = (unsigned char)255;
			}else{
				p->g = (unsigned char)0;
			}
			p->b = (unsigned char)255;
			p->a = (unsigned char)255;
			p++;
		}
	}

	glBindTexture(GL_TEXTURE_2D, texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
		tex_width,  tex_height,
		0, GL_RGBA, GL_UNSIGNED_BYTE, tmp);

	free(tmp);
	update();
}

GLTextureView::GLTextureView() : textureScene(false)
{
    setName("Texture");

    textureScene = new TextureScene(this);
    QVBoxLayout* vbox = new QVBoxLayout();
    vbox->addWidget(textureScene);
    setLayout(vbox);
}


void GLTextureView::onActivated()
{
}


void GLTextureView::onDeactivated()
{
}
