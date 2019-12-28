#include "Screen.h"
#include "PanelGame.h"

bool Screen::IsFullScreen()
{
	return App->window->fullscreen;
}

void Screen::SetFullScreen(bool fullscreen)
{
	App->window->SetFullScreen(fullscreen);
}

bool Screen::IsFullDesktop()
{
	return App->window->full_desktop;
}

void Screen::SetFullDesktop(bool fulldesktop)
{
	App->window->SetFullDesktop(fulldesktop);
}

bool Screen::IsResizable()
{
	return App->window->resizable;
}

void Screen::SetResizable(bool resizable)
{
	App->window->SetResizable(resizable);
}

bool Screen::IsBorderless()
{
	return App->window->borderless;
}

void Screen::SetBorderless(bool borderless)
{
	App->window->SetBorderless(borderless);
}

int Screen::GetHeight()
{
#ifndef GAME_VERSION
	return App->ui->panel_game->height;
#else
	return App->window->height;
#endif
}

int Screen::GetWidth()
{
#ifndef GAME_VERSION
	return App->ui->panel_game->width;
#else
	return App->window->width;
#endif
}
