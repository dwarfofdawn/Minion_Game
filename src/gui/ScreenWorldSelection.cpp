/*
 * WidgetWorldSelection.cpp
 *
 *  Created on: Aug 26, 2015
 *      Author: dwarf
 */

#include "CommandHandler.h"
#include "Game.h"
#include "AudioPlayer.h"
#include <os.h>

#include <dirent.h>
#include <ScreenWorldSelection.h>


namespace gui {

WidgetWorldSelection::WidgetWorldSelection() : Widget() {

	DIR *dir;
	struct dirent *ent;
	if((dir = opendir(toOsFileString("./level/").c_str())) != NULL){
		while((ent = readdir(dir)) != NULL){
			std::string name = std::string(ent->d_name);
			if(name.size() > 4 && name.substr(name.size() - 4) == ".cmp"){
				WidgetButton *b = new WidgetButton(name.substr(0, name.size() - 4));
				buttons.push_back(b);
				nameToPath[name.substr(0, name.size() - 4)] = "./level/" + name;
				guiLogger.log("found campaign " + name);
			}
		}
		closedir (dir);
	}
	else{
		guiLogger.log("unable to search for campaign files!");
	}

	buttons.push_back(new WidgetButton("back"));


	this->setWidth(6);
	this->setHeight((buttons.size()) * 2);

	int i  = 0;
	for(WidgetButton *b : buttons){
		b->setLocation(0, i * 2);
		b->setWidth(6);
		b->setHeight(1);
		b->setListener(this);
		addWidget(b);
		i++;
	}
	guiLogger.log("found " + std::to_string(buttons.size()) + " campaigns");
}

WidgetWorldSelection::~WidgetWorldSelection() {
	for(WidgetButton *b : buttons){
		delete b;
	}
}

void WidgetWorldSelection::render(float updateFactor){
	Widget::render(updateFactor);
}


void WidgetWorldSelection::onButtonPressed(std::string text){
	if(text == "back"){
		game::getHomescreenWidget()->toMainMenu();
	}
	else if(nameToPath.find(text) != nameToPath.end()){
		handleCommand("loadCampaign " + nameToPath[text]);
		handleCommand("setEditMode a");
		game::toGameWidget();
		audio::clearMusicQueue();
		audio::queueMusic("sfx/temp/moorland.wav");
		audio::nextSong();
	}
}

} /* namespace gui */
