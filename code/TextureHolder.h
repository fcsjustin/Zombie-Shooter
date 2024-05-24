#pragma once
#ifndef TEXTURE_HOLDER_H
#define TEXTURE_HOLDER_H

#include <SFML/Graphics.hpp>
#include <map>

using namespace std;

class TextureHolder
{
private:
    // a map container from the STL,
    // that holds related pairs of string and texture
    map<string, sf::Texture> m_Textures;

    // a pointer of the same type as the class itself
    // the one and only instance
    static TextureHolder* m_s_Instance;

public:
    TextureHolder();
    static sf::Texture& GetTexture(string const& filename);
};



#endif 
