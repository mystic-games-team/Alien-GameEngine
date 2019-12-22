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
	return App->ui->panel_game->height;
}

int Screen::GetWidth()
{
	return App->ui->panel_game->width;
}
