//
//  GlslProgHelper.cpp
//  GlslProgHelper
//
//  Created by Simon Geilfus on 12/03/13.
//
//

#include "GlslProgHelper.h"
#include "cinder/Utilities.h"
#include "cinder/app/App.h"

#include <regex>

using namespace std;
using namespace ci;

gl::GlslProgRef GlslProgHelper::create( DataSourceRef vertexShader, DataSourceRef fragmentShader, vector<string> defines )
{
    return create( vertexShader ? loadString( vertexShader ).c_str() : 0, fragmentShader ? loadString( fragmentShader ).c_str() : 0, defines );
}
gl::GlslProgRef GlslProgHelper::create( const char *vertexShader, const char *fragmentShader, vector<string> defines )
{
    stringstream vertex;
    stringstream fragment;
    
    for( vector<string>::iterator it = defines.begin(); it != defines.end(); ++it ){
        if( vertexShader )
            vertex      << "#define " << *it << endl;
        if( fragmentShader )
            fragment    << "#define " << *it << endl;
    }
    
    if( vertexShader )
        vertex      << vertexShader;
    
    if( fragmentShader )
        fragment    << fragmentShader;
    
    return gl::GlslProg::create( vertexShader ? preprocessVersion( preprocessIncludes( vertex.str() ) ).c_str() : 0,
                        fragmentShader ? preprocessVersion( preprocessIncludes( fragment.str() ) ).c_str() : 0 );
}


gl::GlslProgRef GlslProgHelper::create( DataSourceRef vertexShader, DataSourceRef fragmentShader, ci::DataSourceRef geometryShader, GLint geometryInputType, GLint geometryOutputType, GLint geometryOutputVertices, std::vector<std::string> defines )
{
    return create(  vertexShader ? loadString( vertexShader ).c_str() : 0, fragmentShader ? loadString( fragmentShader ).c_str() : 0, geometryShader ? loadString( geometryShader ).c_str() : 0, geometryInputType, geometryOutputType, geometryOutputVertices, defines );
}
gl::GlslProgRef GlslProgHelper::create( const char *vertexShader, const char *fragmentShader, const char *geometryShader, GLint geometryInputType, GLint geometryOutputType, GLint geometryOutputVertices, std::vector<std::string> defines )
{
    stringstream vertex;
    stringstream fragment;
    stringstream geometry;
    
    for( vector<string>::iterator it = defines.begin(); it != defines.end(); ++it ){
        if( vertexShader )
            vertex      << "#define " << *it << endl;
        if( fragmentShader )
            fragment    << "#define " << *it << endl;
        if( geometryShader )
            geometry    << "#define " << *it << endl;
    }
    
    if( vertexShader )
        vertex      << vertexShader;
    
    if( fragmentShader )
        fragment    << fragmentShader;
    
    if( geometryShader )
        geometry    << geometryShader;
    
    return gl::GlslProg::create( vertexShader ? preprocessVersion( preprocessIncludes( vertex.str() ) ).c_str() : 0,
                                fragmentShader ? preprocessVersion( preprocessIncludes( fragment.str() ) ).c_str() : 0,
                                geometryShader ? preprocessVersion( preprocessIncludes( geometry.str() ) ).c_str() : 0, geometryInputType, geometryOutputType, geometryOutputVertices );
}


std::map< std::string, std::string > GlslProgHelper::mVirtualFiles;

void GlslProgHelper::addVirtualFile( const std::string& filename, const std::string& source )
{
    mVirtualFiles[ filename ] = source;
}


// From http://www.opengl.org/discussion_boards/showthread.php/169209-include-in-glsl?p=1192415&viewfull=1#post1192415
string GlslProgHelper::preprocessIncludes( const string& source, int level )
{
	if(level > 32){
        cout << "GlslProgHelper Error max include recursion level" << endl;
        return "";
    }

	static const std::regex re("^[ ]*#[ ]*include[ ]+[\"<](.*)[\">].*");
	stringstream input;
	stringstream output;
	input << source;
    
	size_t lineNumber = 1;
	std::smatch matches;
    
    string version;
	string line;
	while( getline( input, line ) ) {
        if (std::regex_search( line, matches, re ) ) {
			string includeFile = matches[1];
			string includeString;

			try {
                includeString = loadString( app::loadAsset( includeFile ) );
			}
			catch ( exception e ) { cout << "GlslProgHelper Error including " << includeFile << endl; }
            
            if( includeString.empty() ){
                try {
                    includeString = loadString( app::loadResource( includeFile ) );
                }
                catch ( exception e ) { cout << "GlslProgHelper Error including " << includeFile << endl; }
            }
            
            if( includeString.empty() && mVirtualFiles.count( includeFile ) ){
                includeString = mVirtualFiles[ includeFile ];
            }
            
            if( !includeFile.empty() )
                output << preprocessIncludes(includeString, level + 1) << endl;
		}
		else {
			output << "#line "<< lineNumber << endl; //" \"" << filename << "\""  << endl;
			output <<  line << endl;
		}
		++lineNumber;
	}
	return output.str();
}

string GlslProgHelper::preprocessVersion( const string& source )
{
	stringstream input;
	stringstream output;
	input << source;
    
    string version;
	size_t lineNumber = 1;
	string line;
	while( getline( input, line ) )
	{
        if( line.find( "#version" ) != string::npos ){
            version = line;
        }
		else {
			output <<  line << endl;
		}
		++lineNumber;
	}
    
	stringstream temp;
    temp << version << endl << output.str();
	return temp.str();
}