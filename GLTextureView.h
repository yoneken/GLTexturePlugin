#include <cnoid/View>
#include <QGLWidget>
#include <QTimer>

class TextureScene : public QGLWidget
{
	Q_OBJECT        // must include this if you use Qt signals/slots
public:
    TextureScene(QWidget* parent = 0);
    ~TextureScene(void);
    bool setTime(double time);

protected:
	QTimer timer;
    virtual void initializeGL();
    virtual void resizeGL(int width, int height);
    virtual void paintGL();
    
protected Q_SLOTS:
	virtual void idle();

protected:
	int display_width, display_height;
	int tex_width, tex_height;
	unsigned int texture;
};

    
class GLTextureView : public cnoid::View
{
public:
    GLTextureView();

protected:
    virtual void onActivated();
    virtual void onDeactivated();
    
private:
    TextureScene* textureScene;
};
