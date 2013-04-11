//
//  GlslProgHelper.cpp
//  GlslProgHelper
//
//  Created by Simon Geilfus on 12/03/13.
//
//

#include "GlslProgHelper.h"
#include "cinder/Utilities.h"

#include <regex>

using namespace std;
using namespace ci;

gl::GlslProg GlslProgHelper::create( DataSourceRef vertexShader, DataSourceRef fragmentShader, initializer_list<string> defines )
{
    return create( vertexShader ? loadString( vertexShader ).c_str() : 0, fragmentShader ? loadString( fragmentShader ).c_str() : 0, defines );
}
gl::GlslProg GlslProgHelper::create( const char *vertexShader, const char *fragmentShader, initializer_list<string> defines )
{
    stringstream vertex;
    stringstream fragment;
    
    for( initializer_list<string>::iterator it = defines.begin(); it != defines.end(); ++it ){
        if( vertexShader )
            vertex      << "#define " << *it << endl;
        if( fragmentShader )
            fragment    << "#define " << *it << endl;
    }
    
    if( vertexShader )
        vertex      << vertexShader;
    
    if( fragmentShader )
        fragment    << fragmentShader;
    
    return gl::GlslProg( vertexShader ? preprocessVersion( preprocessIncludes( vertex.str() ) ).c_str() : 0,
                        fragmentShader ? preprocessVersion( preprocessIncludes( fragment.str() ) ).c_str() : 0 );
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
			//output << "#line "<< lineNumber << " \"" << filename << "\""  << endl;
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