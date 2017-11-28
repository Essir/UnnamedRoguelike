#include"interface.h"
#include"creature.h"

myColor::myColor(short r, short g, short b, std::string name, short num):
	r(r),g(g),b(b),name(name),num(num)
{
	init_color(num,r,g,b);
}
myColor::myColor(){}
myColor::~myColor(){}

short myColor::GetR(){		return r;}
short myColor::GetG(){		return g;}
short myColor::GetB(){		return b;}
short myColor::GetNum(){	return num;}
std::string myColor::GetName(){	return name;}

myPair::myPair(myColor* fg, myColor* bg, short num):
	fg(fg),bg(bg),num(num)
{
	init_pair(num,fg->GetNum(),bg->GetNum());
}
myPair::myPair(){}
myPair::~myPair(){
	fg=NULL;
	bg=NULL;
}

myColor* myPair::GetFg(){	return fg;}
myColor* myPair::GetBg(){	return bg;}

window::window(short width, short height):
	width(width),height(height)
{
	win = newwin(height,width,0,0);
	updated=false;
	focused=false;
}
window::~window(){}

void window::Clear(){
	wclear(win);
	SetUpdated(true);
}
void window::Draw(){
	if(IsUpdated()){
		wnoutrefresh(win);
		SetUpdated(false);
	}
}
void window::Resize(short newx, short newy){
	wresize(win, newy, newx);
	width = newx;
	height = newy;
	SetUpdated(true);
}
void window::SetUpdated(bool key){	updated = key;}
void window::SetFocused(bool key){	focused = key;}
bool window::IsUpdated(){		return updated;}
bool window::IsFocused(){		return focused;}
short window::GetWidth(){		return width;}
short window::GetHeight(){		return height;}

worldwindow::worldwindow(short width, short height):
	window(width,height){
	}

worldwindow::~worldwindow(){}

void worldwindow::Draw(short x, short y, char ch){
	mvwaddch(win,y,x,ch);
	SetUpdated(true);
}
void worldwindow::Draw(short x, short y, char ch, short color){
	mvwaddch(win,y,x,ch | COLOR_PAIR(color));
	SetUpdated(true);
}
window_bordered::window_bordered(short x, short y, short width, short height):
	x(x),y(y),window(width,height)
{
	win = newwin(height, width, y, x);
	subwin = derwin(win,height-2, width -2, 1, 1);
	focused=updated=false;
	luanch=ldanch=ruanch=rdanch=false;
}
window_bordered::~window_bordered(){}

void window_bordered::Draw(){
	if(IsUpdated()){
		box(win,0,0);
		wnoutrefresh(win);
		wnoutrefresh(subwin);
		SetUpdated(false);
	}
}

void window_bordered::Clear(){
	wclear(subwin);
	SetUpdated(true);
}

void window_bordered::MoveAt(short dx, short dy){
	this->x+=x;
        this->y+=y;
        mvwin(win,this->y,this->x);
        SetUpdated(true);
}

void window_bordered::MoveTo(short x, short y){
	this->x=x;
	this->y=y;
        mvwin(win,this->y,this->x);
        SetUpdated(true);
}

void window_bordered::SetLUAnch(bool key){	luanch=key;}
void window_bordered::SetLDAnch(bool key){	ldanch=key;}
void window_bordered::SetRUAnch(bool key){	ruanch=key;}
void window_bordered::SetRDAnch(bool key){	rdanch=key;}
bool window_bordered::GetLUAnch(){		return luanch;}
bool window_bordered::GetLDAnch(){		return ldanch;}
bool window_bordered::GetRUAnch(){		return ruanch;}
bool window_bordered::GetRDAnch(){		return rdanch;}

window_chat::window_chat(short x, short y, short width, short height,short chatsize):
	window_bordered(x, y, width, height)
{
	this->chatsize = chatsize;
	mes = new std::string[chatsize];
	for(short i=0; i<chatsize; i++){
		mes[i] = "";
	}
	pointer = 0;
	mescount = 0;
	SetUpdated(true);
}
window_chat::~window_chat(){
	delete[] mes;
}

void window_chat::Clear(){
	for(short i=0; i<chatsize; i++){
		mes[i] = "";
	}
	werase(subwin);
	mescount=0;
	SetUpdated(true);
}

void window_chat::Draw(){
	box(win,0,0);
//	if(IsUpdated()){
	werase(subwin);
	for(short i=0; i<GetHeight()-2; i++){
		if(i+pointer<chatsize)
			wprintw(subwin,"%s",mes[i+pointer].c_str());
	}
	wnoutrefresh(subwin);
	SetUpdated(false);
//	}
	wnoutrefresh(win);
}

void window_chat::AddMessage(std::string mes){
	short count=0;
	short linecount=0;
	short temppointer=0;
	std::string tempmes[chatsize];
	for(short i=0; i<chatsize; i++){
		tempmes[i]="";
	}
	for(short i=0; i<mes.length(); i++){
		tempmes[linecount]+=mes[i];
		count++;
		if(count>=GetWidth()-2){
			count=0;
			linecount++;
			if(linecount>chatsize){
				break;
			}
		}
	}
	tempmes[linecount++]+='\n';
	if(mescount+linecount<chatsize){
		for(short i=mescount; i<=mescount+linecount; i++){
			this->mes[i] = tempmes[temppointer++];
			if(i>GetHeight()-3&&i<mescount+linecount){
				ScrollDown();
			}
		}
		mescount+=linecount;
	}else{
		short offset = mescount+linecount-chatsize;
		for(short i=0; i<mescount-offset; i++){
			this->mes[i] = this->mes[i+offset];
		}
		mescount-=offset;
		for(short i=chatsize-linecount; i<chatsize; i++){
			this->mes[i] = tempmes[temppointer++];
		}
		mescount+=linecount;
	}
	SetUpdated(true);
}

void window_chat::ScrollUp(){
	pointer=(pointer-1>-1)?pointer-1:0;
	SetUpdated(true);
}

void window_chat::ScrollDown(){
	short upperlimit = mescount-(GetHeight()-3);
	if(upperlimit<0){
		pointer = 0;
	}else if(pointer+1>upperlimit){
		pointer = upperlimit;
	}else 
		pointer+=1;
	SetUpdated(true);
}

window_playerstats::window_playerstats(short x, short y, short width, short height):
	window_bordered(x, y, width, height)
{
	itemhlght=0;
}

window_playerstats::~window_playerstats(){
	player = NULL;
}

void window_playerstats::PrintStats(){
	werase(subwin);
	if(player==NULL)
		return;
	wprintw(subwin,"hp:%d\n",player->GetHp());
	if(player->GetInHand()!=NULL){
		std::string mes ="";
		mes+=player->GetInHand()->GetImg();
		mes+='\n';
		wprintw(subwin,"hand:%s\n",mes.c_str());
	}else{
		wprintw(subwin,"hand:none\n");
	}
	for(short i=0; i<player->GetInvSize(); i++){
		if(player->GetItem(i)!=NULL){
			if(i==itemhlght){
				waddch(subwin,(player->GetItem(i)->GetImg()|COLOR_PAIR(2)));
			}else{
				waddch(subwin,(player->GetItem(i)->GetImg()|COLOR_PAIR(1)));
			}
		}else{
			waddch(subwin,' ');
		}		
	}
	SetUpdated(true);
}

void window_playerstats::AssignPlayer(creature* player){
	this->player = player;
}

void window_playerstats::FocusLeft(){
	for(short i=itemhlght-1; i>-1; i--){
		if(player->GetItem(i)!=NULL){
			itemhlght=i;
			break;
		}
	}
	SetUpdated(true);
}

void window_playerstats::FocusRight(){
	for(short i=itemhlght+1; i<player->GetInvSize(); i++){
		if(player->GetItem(i)!=NULL){
			itemhlght=i;
			break;
		}
	}
	SetUpdated(true);
}

short window_playerstats::GetHighlight(){	return itemhlght;}

interface::~interface(){
	delete playerstat,
	       chat,
	       mainscr;
	for(short i=0; i<32; i++){
		delete colors[i];
	}
	delete[] colors;
	for(short i=0; i<32; i++){
		delete pairs[i];
	}
	delete[] pairs;
	clear();
	endwin();
}

interface::interface(){
	initscr();
	noecho();
	curs_set(FALSE);
	start_color();
	colornum = 0;
	pairnum = 1;
	colors = new myColor*[32];
	pairs = new myPair*[32];
	colors[colornum] = new myColor(0,0,0,"black",colornum); 	colornum++;	//0
	colors[colornum] = new myColor(1000,1000,1000,"white",colornum);colornum++;	//1
	colors[colornum] = new myColor(500,500,500,"gray",colornum); 	colornum++;	//2
	colors[colornum] = new myColor(1000,1000,200,"yelow",colornum);	colornum++;	//3
	colors[colornum] = new myColor(1000,200,1000,"violet",colornum);colornum++;	//4
	colors[colornum] = new myColor(200,1000,1000,"cyan",colornum); 	colornum++;	//5
	colors[colornum] = new myColor(1000,200,200,"red",colornum);	colornum++;	//6
	colors[colornum] = new myColor(200,1000,200,"green",colornum);	colornum++;	//7
	colors[colornum] = new myColor(200,200,1000,"blue",colornum);	colornum++;	//8
	colors[colornum] = new myColor(800,800,800,"white2",colornum);	colornum++;	//9
	colors[colornum] = new myColor(600,600,600,"white3",colornum);	colornum++;	//10
	colors[colornum] = new myColor(400,400,400,"white4",colornum);	colornum++;	//11
	colors[colornum] = new myColor(200,200,200,"white5",colornum);	colornum++;	//12
	pairs[pairnum] = new myPair(colors[1],colors[0],pairnum);	pairnum++;	//1
	pairs[pairnum] = new myPair(colors[0],colors[1],pairnum);	pairnum++;	//2
	pairs[pairnum] = new myPair(colors[2],colors[0],pairnum);	pairnum++;	//3
	pairs[pairnum] = new myPair(colors[3],colors[0],pairnum);	pairnum++;	//4
	pairs[pairnum] = new myPair(colors[4],colors[0],pairnum);	pairnum++;	//5
	pairs[pairnum] = new myPair(colors[5],colors[0],pairnum);	pairnum++;	//6
	pairs[pairnum] = new myPair(colors[6],colors[0],pairnum);	pairnum++;	//7
	pairs[pairnum] = new myPair(colors[7],colors[0],pairnum);	pairnum++;	//8
	pairs[pairnum] = new myPair(colors[8],colors[0],pairnum);	pairnum++;	//9
	pairs[pairnum] = new myPair(colors[0],colors[9],pairnum);	pairnum++;	//10
	pairs[pairnum] = new myPair(colors[0],colors[10],pairnum);	pairnum++;	//11
	pairs[pairnum] = new myPair(colors[0],colors[11],pairnum);	pairnum++;	//12
	pairs[pairnum] = new myPair(colors[0],colors[12],pairnum);	pairnum++;	//13
	short width,height;
	getmaxyx(stdscr,height,width);
	keypad(stdscr,true);
	short rightoffset = 10;
	short leftoffset = 20;
	short rightstart = width-rightoffset;
	mainscr = new worldwindow(width,height+1);
	playerstat = new window_playerstats(rightstart, 0, rightoffset, 6);
	chat = new window_chat(0,height-12,leftoffset-1,12,50);
	playerstat->SetRUAnch(true);
	chat->SetLDAnch(true);
}

void interface::AssignPlayer(creature* player){
	playerstat->AssignPlayer(player);
}

void interface::Draw(){
	mainscr->Draw();
	playerstat->Draw();
	chat->Draw();
	doupdate();
	move(GetHeight()-1,GetWidth()-1);
}
void interface::CheckResize(){
	short tempwidth, tempheight;
	getmaxyx(stdscr,tempheight,tempwidth);
	if(tempheight!=mainscr->GetHeight() || tempwidth!=mainscr->GetWidth()){
		ResizeInterface(tempwidth, tempheight);
	}
}
void interface::ResizeInterface(short newx, short newy){
	short deltay = newy - mainscr->GetHeight();
	short deltax = newx - mainscr->GetWidth();

	mainscr->Resize(newx,newy);
	ProcessWindowOnResize(playerstat,deltax,deltay);
	ProcessWindowOnResize(chat,deltax, deltay);
	
	Draw();
}
void interface::ProcessWindowOnResize(window_bordered* win, short deltax, short deltay){
	if(win!=NULL){
		if(win->GetLDAnch() || win->GetRDAnch())
			win->MoveAt(0,deltay);
		if(win->GetRUAnch() || win->GetRDAnch())
			win->MoveAt(deltax,0);
	}
}
void interface::ShowPlayerStat(){
	playerstat->PrintStats();
}

void interface::WriteToChat(std::string text){
	chat->AddMessage(text);
}
void interface::ClearChat(){
	chat->Clear();
}
void interface::ChatScrollUp(){
	chat->ScrollUp();
}
void interface::ChatScrollDown(){
	chat->ScrollDown();
}
void interface::SetChatFocus(bool key){	chat->SetFocused(key);}
bool interface::GetChatFocus(){		chat->IsFocused();}

void interface::DrawOnMap(short x, short y,char ch){
	mainscr->Draw(x,y,ch);
}
void interface::DrawOnMap(short x, short y,char ch, short color){
	if(color==1){
		mainscr->Draw(x,y,ch, 2);
	}else{
		mainscr->Draw(x,y,ch, 8+color);
	}
}
void interface::ClearMap(){
	mainscr->Clear();
}

void interface::SetInventoryFocus(bool key){	
	playerstat->SetFocused(key);
}
void interface::InventoryFocusLeft(){
	playerstat->FocusLeft();
}
void interface::InventoryFocusRight(){
	playerstat->FocusRight();
}
short interface::GetWidth(){			return mainscr->GetWidth();}
short interface::GetHeight(){			return mainscr->GetHeight();}
short interface::GetHighlight(){		return playerstat->GetHighlight();}
bool interface::GetInventoryFocus(){		return playerstat->IsFocused();}