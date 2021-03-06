#ifndef windows_h
#define windows_h
#include<string>
#include<ncurses.h>
#include<menu.h>
#include"iConstInt.h"

class myColor;
class myPair;
class creature;
class gameobjectmovable;

class window{
protected:
        short width, height;
        bool updated;
        bool focused;
        WINDOW* win;
public:
	window(short width, short height);
	virtual ~window();
        virtual void Clear();
	virtual void Draw();
        void Resize(short newx, short newy);
        void SetUpdated(bool key);
	void SetFocused(bool key);
        bool IsUpdated();
        bool IsFocused();
        short GetWidth();
        short GetHeight();
};

class worldwindow:public window{
public:
        worldwindow(short width, short heigth);
        ~worldwindow();
        using window::Draw;
        void Draw(short x, short y, char ch);
        void Draw(short x, short y, char ch, short color);
	void Draw(short x, short y, char ch, myPair* color);
};

struct mainmenuitem{
	iConstInt code;
	std::string str;
};
class mainmenu:public window{
        std::string message;
	ITEM** items;
	ushort itemscount;
	std::string menuchar;
        std::string* strings;
	MENU* mymenu;
public:
	mainmenu(short width, short height, std::vector<mainmenuitem*> menuitems);
	~mainmenu();
	void FocusUp();
	void FocusDown();
	void Draw();
	mainmenuitem* GetFocused();
};

class window_bordered:public window{
protected:
        WINDOW* subwin;
        myPair* focusedbox;
	myPair* regularbox;
        short x, y;
	void DrawBox();
public:
        window_bordered(short x, short y, short width, short height, myPair* focusedbox, myPair* regularbox);
        virtual ~window_bordered();
        void Draw()override;
        void Clear()override;
        short GetX();
        short GetY();
        void MoveAt(short dx, short dy);
        void MoveTo(short x, short y);
};

class window_chat:public window_bordered{
        std::string *mes;
        short pointer;
        short chatsize;
        short mescount;
public:
        window_chat(short x, short y, short width, short height, short chatsize, myPair* focusedbox, myPair* regularbox);
        ~window_chat();
        void Clear() override;
        void Draw() override;
        void AddMessage(std::string mes);
        void ScrollUp();
        void ScrollDown();
};

class window_playerstats:public window_bordered{
        short itemhlght;
        creature* player;
public:
        window_playerstats(short x, short y, short width, short height, myPair* focusedbox, myPair* regularbox);
        ~window_playerstats();
        void AssignPlayer(creature* player);
        void FocusLeft();
        void FocusRight();
        void PrintStats();
        short GetHighlight();
};

class attack_dialog:public window_bordered{
        std::string message;
	ITEM** items;
	ushort itemscount;
	ITEM* focused_item;
	std::string* chars;
        std::string* strings;
	MENU* mymenu;
public:
        attack_dialog(short x, short y, short width, short height, std::vector<gameobjectmovable*> targets, myPair* focusedbox, myPair* regularbox);
        ~attack_dialog();
        void Draw() override;
	void FocusUp();
	void FocusDown();
	gameobjectmovable* GetFocused();
};

#endif
