//
//  GlslProgHelper.h
//  GlslProgHelper
//
//  Created by Simon Geilfus on 12/03/13.
//
//

#pragma once

#include "cinder/gl/GlslProg.h"

class GlslProgHelper {
public:
    
    static ci::gl::GlslProgRef create( ci::DataSourceRef vertexShader, ci::DataSourceRef fragmentShader = ci::DataSourceRef(), std::initializer_list<std::string> defines = std::initializer_list<std::string>() );
    static ci::gl::GlslProgRef create( const char *vertexShader, const char *fragmentShader = 0, std::initializer_list<std::string> defines = std::initializer_list<std::string>() );
    
    /*static ci::gl::GlslProg create( ci::DataSourceRef vertexShader, ci::DataSourceRef fragmentShader = ci::DataSourceRef(), ci::DataSourceRef geometryShader = ci::DataSourceRef(), GLint geometryInputType = GL_POINTS, GLint geometryOutputType = GL_TRIANGLES, GLint geometryOutputVertices = 0, std::initializer_list<std::string> defines = std::initializer_list<std::string>() );
    static ci::gl::GlslProg create( const char *vertexShader, const char *fragmentShader = 0, const char *geometryShader = 0, GLint geometryInputType = GL_POINTS, GLint geometryOutputType = GL_TRIANGLES, GLint geometryOutputVertices = 0, std::initializer_list<std::string> defines = std::initializer_list<std::string>() );*/
    
    static void addVirtualFile( const std::string& filename, const std::string& source );
    
protected:
    
    static std::string preprocessIncludes( const std::string& source, int level = 0 );
    static std::string preprocessVersion( const std::string& source );
    
    static std::map< std::string, std::string > mVirtualFiles;
    
};