#pragma once
#include <glad/glad.h>
#include <cassert>

struct Image;
struct Texture
{
    GLuint id = GL_NONE;
    int width = 0;
    int height = 0;
};

void LoadTexture(Texture* texture, int width, int height);
void UnloadTexture(Texture* texture);

// If you want a texture from file ie PNG, load it as an image then copy to texture!
void UpdateTexture(Texture texture, const Image& image);

inline void BindTexture(Texture texture, GLuint slot = 0)
{
    assert(texture.id != GL_NONE);
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, texture.id);
}

inline void UnbindTexture(Texture texture)
{
    assert(texture.id != GL_NONE);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
}