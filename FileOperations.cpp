#include "material.h"
#include<fstream>
#include<vector>
#include<ncurses.h>
namespace FileOperations{
	static std::vector<material*> LoadMaterials(){
		std::vector<material*> materials;
		std::ifstream stones("raws/materials/stones.raw");
		short count=0;
		std::string name="";
		std::string type="";
		std::string temptype="";
		std::string val="";
		char img='@';
		short value=-1;
		short melting=-1;
		short density=-1;
		for(std::string line; getline(stones,line);){
			if(line=="["){
				continue;
			}
			if(line=="]"){
				printw("%s ",name.c_str());refresh();
				if(type=="gas"){
					materials.push_back(new gasMAT(img,density,value,name,count));
				}else{
					materials.push_back(new stoneMAT(img,density,value,melting,name,count));
				}
				name="";
				type="";
				img='@';
				value=-1;
				melting=-1;
				density=-1;
				count++;
				continue;
			}
			short pos = line.find(":");
			temptype = line.substr(0,pos);
			val = line.substr(pos+1);
			if(temptype=="name"){
				name=val;
			}else if(temptype=="type"){
				type=val;
			}else if(temptype=="img"){
				img=val[0];
			}else if(temptype=="value"){
				value=std::stoi(val);
			}else if(temptype=="melting"){
				melting=std::stoi(val);
			}else if(temptype=="density"){
				density=std::stoi(val);
			}
		}
		stones.close();
		return materials;
	}
};