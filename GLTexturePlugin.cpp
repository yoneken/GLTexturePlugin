
#include "GLTextureView.h"
#include <cnoid/Plugin>

using namespace cnoid;

class GLTexturePlugin : public Plugin
{
public:
    
    GLTexturePlugin() : Plugin("GLTexture") { }
    
    virtual bool initialize() {

        addView(new GLTextureView());
            
        return true;
    }
};

CNOID_IMPLEMENT_PLUGIN_ENTRY(GLTexturePlugin);
