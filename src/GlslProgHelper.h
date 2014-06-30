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
    
    static ci::gl::GlslProgRef create( ci::DataSourceRef vertexShader, ci::DataSourceRef fragmentShader = ci::DataSourceRef(), std::vector<std::string> defines = std::vector<std::string>() );
    static ci::gl::GlslProgRef create( const char *vertexShader, const char *fragmentShader = 0, std::vector<std::string> defines = std::vector<std::string>() );
    
    static ci::gl::GlslProgRef create( ci::DataSourceRef vertexShader, ci::DataSourceRef fragmentShader, ci::DataSourceRef geometryShader, GLint geometryInputType = GL_POINTS, GLint geometryOutputType = GL_TRIANGLES, GLint geometryOutputVertices = 0, std::vector<std::string> defines = std::vector<std::string>() );
    static ci::gl::GlslProgRef create( const char *vertexShader, const char *fragmentShader, const char *geometryShader, GLint geometryInputType = GL_POINTS, GLint geometryOutputType = GL_TRIANGLES, GLint geometryOutputVertices = 0, std::vector<std::string> defines = std::vector<std::string>() );
    
    static void addVirtualFile( const std::string& filename, const std::string& source );
    
protected:
    
    static std::string preprocessIncludes( const std::string& source, int level = 0 );
    static std::string preprocessVersion( const std::string& source );
    
    static std::map< std::string, std::string > mVirtualFiles;
    
};