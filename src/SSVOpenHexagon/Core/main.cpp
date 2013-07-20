// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include <vector>
#include <string>
#include <SSVUtils/SSVUtils.h>
#include <SSVStart/SSVStart.h>
#include "SSVOpenHexagon/Online/Online.h"
#include "SSVOpenHexagon/Online/Server.h"
#include "SSVOpenHexagon/Core/HexagonGame.h"
#include "SSVOpenHexagon/Core/MenuGame.h"
#include "SSVOpenHexagon/Global/Assets.h"
#include "SSVOpenHexagon/Global/Config.h"

using namespace std;
using namespace ssvs;
using namespace ssvu;
using namespace ssvu::FileSystem;
using namespace hg;

void createProfilesFolder()
{
	if(exists("Profiles/")) return;

	log("Profiles folder does not exist, creating", "::createProfilesFolder");
	createFolder("Profiles/");
}

int main(int argc, char* argv[])
{
	vector<string> overrideIds; for(int i{0}; i < argc; ++i) overrideIds.push_back(string{argv[i]});

	if(contains(overrideIds, "server"))
	{
		Online::initializeServer();
		return 0;
	}

	createProfilesFolder();

	Online::initializeClient();
	Online::tryConnectToServer();

	//Online::startCheckUpdates();
	setRandomSeed();
	loadConfig(overrideIds);

	string title{"Open Hexagon " + toStr(getVersion()) + " - by vittorio romeo"};
	GameWindow window{title, createDynamicTimer(window), getWidth(), getHeight(), getPixelMultiplier(), getFullscreen()};
	window.setVsync(getVsync());
	window.setMouseCursorVisible(false);

	HGAssets assets;
	HexagonGame hg{assets, window};
	MenuGame mg{assets, hg, window};
	hg.mgPtr = &mg;

	assets.refreshVolumes();
	window.setGameState(mg.getGame()); mg.init();

	Online::tryLogin("vittorioromeo", "vittro1234");

	window.run();

	//Online::terminateAll();

	saveConfig(); assets.saveCurrentProfile(); saveLogToFile("log.txt");
	return 0;
}

