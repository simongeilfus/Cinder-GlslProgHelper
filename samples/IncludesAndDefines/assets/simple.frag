#include "common.glsl"
#include "test.glsl"

void main(){
	#ifdef USE_COLOR1
		gl_FragColor = getColor1();
	#else
		gl_FragColor = TEST;
	#endif
}