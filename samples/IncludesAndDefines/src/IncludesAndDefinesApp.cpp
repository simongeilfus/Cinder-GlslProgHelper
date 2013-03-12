#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "GlslProgHelper.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class IncludesAndDefinesApp : public AppNative {
  public:
	void setup();
	void draw();
    
    gl::GlslProg mShader;
};

void IncludesAndDefinesApp::setup()
{
    
    try {
        mShader = GlslProgHelper::create( DataSourceRef(), loadAsset( "simple.frag" ), { "USE_COLOR1" } );
    }
    catch( gl::GlslProgCompileExc exc ){
        cout << exc.what() << endl;
    }
}


void IncludesAndDefinesApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    
    if( mShader ){
        mShader.bind();
        gl::drawSolidRect( getWindowBounds() );
        mShader.unbind();
    }
}

CINDER_APP_NATIVE( IncludesAndDefinesApp, RendererGl )
