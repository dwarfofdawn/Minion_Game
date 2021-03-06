/*
 * RenderableSprite.cpp
 *
 *  Created on: Dec 2, 2014
 *      Author: dwarf
 */

#include "RenderableSprite.h"

#include <iostream>
#include <SDL2/SDL.h>
//#include <SDL2/SDL_opengl.h>
#include "glewWrapper.h"


#include "MatrixHandler.h"
#include "ShaderHandler.h"

namespace render{

RenderableSprite::RenderableSprite(float x, float y, float width, float height, float rot, std::string textureName) : Renderable(x, y), width(width), height(height), rot(rot), textureName(textureName), texture(NULL){//textureName

}

RenderableSprite::RenderableSprite(float x, float y, float width, float height, float rot, render::Texture *texture) : Renderable(x, y), width(width), height(height), rot(rot), textureName(""), texture(texture){

}

RenderableSprite::~RenderableSprite() {
}

void RenderableSprite::render(){
	if(texture == NULL){
		texture = getTexture(textureName);
	}
	if(texture != NULL){
		glBindTexture(GL_TEXTURE_2D, texture->id);

		matrix::loadIdentity();
		matrix::quickTranslate(x, y);
		matrix::rotate(rot);
		matrix::scale(width / 2, height / 2);
		matrix::uploadModelViewMatrix();

		shader::setShaderUseTexture(true);

		glBindVertexArray(texture->vao->id);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else{
		warpLogger.log(textureName + " is not a registered texture");
	}

}

void RenderableSprite::setTransform(float x, float y, float width, float height, float rot){
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->rot = rot;

}
void RenderableSprite::setTransform(float x, float y, float width, float height){
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}

RenderableSprite *RenderableSprite::clone(){
	if(texture != NULL)
		return new RenderableSprite(x, y, width, height, rot, texture);
	else
		return new RenderableSprite(x, y, width, height, rot, textureName);
}

RenderableSprite *RenderableSprite::cloneWithTransforms(float x, float y, float width, float height, float rot){
	if(texture != NULL)
		return new RenderableSprite(x, y, width, height, rot, texture);
	else
		return new RenderableSprite(x, y, width, height, rot, textureName);
}


}
