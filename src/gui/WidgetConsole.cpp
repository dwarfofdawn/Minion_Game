/*
 * WidgetConsole.cpp
 *
 *  Created on: Dec 31, 2014
 *      Author: dwarf
 */

#include "WidgetConsole.h"
#include "Renderer.h"
#include "GraphicsOptions.h"
#include "CommandHandler.h"
#include <string>
#include <map>
#include "TextHelper.h"
#include "Keyboard.h"
#include "Game.h"

namespace gui {

render::Color colorConsoleBackground(0, 0, 0.05);
render::Color colorFont(1, 1, 1);

//std::map<char, char> shiftMap = {{'1', '!'}, {'2', '@'}, {'3', '#'}, {'4', '$'}, {'5', '%'}, {'6', '^'}, {'7', '&'}, {'8', '*'}, {'9', '('}, {'0', ')'}, {'-', '_'}, {'=', '+'}, {'[', '{'}, {']', '}'}, {';', ':'}, {'\\', '|'}, {',', '<'}, {'.', '>'}, {'/', '?'}};

WidgetConsole::WidgetConsole() : Widget() {
	this->setWidth(2 * render::options::getAspectRatio());
	this->setHeight(0.5);
	this->setListenToMouse(true);
	this->setListenToKeys(true);
	this->setLocation(0, 0.75);
	left = -render::options::getAspectRatio();
	bottom = 0.5;
	blinkState = 0;
	for(int i = 0; i < numLines; i++){
		lines[i] = std::string("");
	}
	cursorPos = 0;
	historyPos = 0;
}

WidgetConsole::~WidgetConsole() {

}

void WidgetConsole::render(float updateFactor){
	if(isFocused()){
		blinkState = (blinkState + 1) % 120;
	}
	render::drawRect(getX(), getY(), getWidth(), getHeight(), 0, colorConsoleBackground);
	for(int i = numLines - 1; i >= 1; i--){
		render::drawString(left + 0.05, bottom + 0.05 + 0.06 * i, 0.05, lines[i], colorFont);
	}
	if(blinkState / 60){
		render::drawString(left + 0.05, bottom + 0.05, 0.05, std::string(">") + lines[0].substr(0, cursorPos) + "_", colorFont);
	}
	render::drawString(left + 0.05, bottom + 0.05, 0.05, std::string(">") + lines[0]);
}

void WidgetConsole::handleMouseButtonEvent(int, float, float, bool){

}

void WidgetConsole::handleKeyEvent(int k, int mod,  bool pressed){
	if(pressed){
		if(k == SDLK_BACKSPACE ){
			if(cursorPos > 0){
				lines[0].erase(cursorPos - 1, 1);
				cursorPos --;
			}
		}
		else if(k == SDLK_DELETE){
			if(cursorPos < lines[0].length()){
				lines[0].erase(cursorPos, 1);
			}
		}
		else if(k == SDLK_RETURN){
			lines[0] += " - " + handleCommand(lines[0]);
			for(int i = numLines - 1; i >= 1; i--){
				lines[i] = lines[i - 1];
			}
			lines[0] = std::string("");
			cursorPos = 0;
			historyPos = 0;
		}
		else if(k == SDLK_LEFT){
			if(cursorPos > 0){
				cursorPos --;
			}
		}
		else if(k == SDLK_RIGHT){
			if(cursorPos < lines[0].length()){
				cursorPos ++;
			}
		}
		else if(k == SDLK_UP){
			if(historyPos < numLines){
				historyPos ++;
				lines[0] = lines[historyPos].substr(0, lines[historyPos].find(" - "));
				cursorPos = lines[0].length();
			}
		}
		else if(k == SDLK_DOWN){
			if(historyPos > 0){
				historyPos --;
				lines[0] = lines[historyPos].substr(0, lines[historyPos].find(" - "));
				cursorPos = lines[0].length();
			}
		}
		else if(k == SDLK_TAB){
			for(int i = numLines - 1; i >= 2; i--){
				lines[i] = lines[i - 1];
			}
			lines[1] = getPossibleCommands(lines[0]);
			lines[0] = autocomplete(lines[0]);
			cursorPos = lines[0].length();
		}
		else if(k == SDLK_BACKQUOTE && (mod & (KMOD_SHIFT | KMOD_CAPS)) > 0){
			//do nothing to avoid reacting to ~ when opening console
		}
		else if(k != SDLK_LSHIFT && k != SDLK_RSHIFT && k != SDLK_CAPSLOCK){
//			if((mod & (KMOD_SHIFT | KMOD_CAPS)) || isKeyDown(SDL_SCANCODE_LSHIFT)){
//				if(k >= 97 && k <= 123){
//					lines[0].insert(cursorPos, std::string(1, (char)(k - 32)));
//					cursorPos ++;
//				}
//				else{
//					lines[0].insert(cursorPos, std::string(1, shiftMap[(char)k]));
//					cursorPos ++;
//				}
//			}
//			else{
//				lines[0].insert(cursorPos, std::string(1, (char)k));
//				cursorPos ++;
//			}
		}
	}

}

void WidgetConsole::handleTextEvent(std::string s){
	lines[0].insert(cursorPos, s);
	cursorPos += s.length();
}

void WidgetConsole::onFocusChanged(){

	if(!isFocused()){
		game::getMasterWidget()->setConsoleActive(false);
	}
}

} /* namespace game */
