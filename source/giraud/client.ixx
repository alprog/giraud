export module client;

import native_window;
import gfx_renderer;
import gui_system;
import configuration;
import network;
import database;
import selection;
import edit_api;

// for intellisense

export class Client
{
public:
	static Client& GetInstance();

	void RunEventLoop();

private:
	Client();
	void update();

	NativeWindow window;
	GfxRenderer renderer;
	GuiSystem gui;
	Configuration config;
	Network network;
	Database db;
	Selection selection;
	EditAPI api;
};