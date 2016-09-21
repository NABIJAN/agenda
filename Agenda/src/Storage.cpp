#include "Storage.hpp"
#include "Path.hpp"
#include <fstream>
#include <string>
#include <iostream>
using namespace std;

Storage::Storage(){
	readFromFile();
	m_dirty = false;
}

Storage::~Storage(){
	writeToFile();//
}
bool Storage::readFromFile(void){
	string t_name = "";
    string t_password = "";
    string t_email = "";
    string t_phone = "";
    string line = "";
    ifstream infile1, infile2;
    infile1.open(Path::userPath,ios::in);
    if(!infile1.is_open()){
    	return false;
    }
    while(!infile1.eof()){
    	line = "";
    	//infile1 >> line;
    	getline(infile1,line);
    	if(line.size() == 0) break;
    	int i;
    	for(i = 1; line[i] != '"'; i++){
    			t_name += line[i];
    	}
    	i += 3;
    	for(; line[i] != '"'; i++){
    			t_password += line[i];
    	}
    	i += 3;
    	for(; line[i] != '"'; i++){
    			t_email += line[i];
    	}
    	i += 3;
    	for(; line[i] != '"'; i++){
    			t_phone += line[i];
    	}
    	m_userList.push_back(User(t_name,t_password,t_email,t_phone));
    	t_name = "";
    	t_password = "";
    	t_email = "";
    	t_phone = "";
    	line = "";
    }
    infile1.close();

    infile2.open(Path::meetingPath,ios::in);
    if(!infile2.is_open()){
    	return false;
    }
    string t_sponsor = "";
    vector<string> t_participators;
    Date t_startDate;
    Date t_endDate;
    string t_title = "";
    string s = "", s1 = "", s2 = "";
    while(!infile2.eof()){
    	line = "";
    	//infile2 >> line;
    	getline(infile2,line);
    	if(line.size() == 0) break;
    	int i;
    	for(i = 1; line[i] != '"'; i++){
    			t_sponsor += line[i];
    	}
    	i += 3;
    	for(; line[i] != '"'; i++){
    		if(line[i] != '&'){
    			s += line[i];
    		} else {
    			t_participators.push_back(s);
    			s = "";
    		}
    	}
    	t_participators.push_back(s);
    	i += 3;
    	for(; line[i] != '"'; i++){
    			s1 += line[i];
    	}
    	i += 3;
    	for(; line[i] != '"'; i++){
    			s2 += line[i];
    	}
    	i += 3;
    	for(; line[i] != '"'; i++){
    			t_title += line[i];
    	}
    	m_meetingList.push_back(Meeting(t_sponsor,t_participators,Date::stringToDate(s1),Date::stringToDate(s2),t_title));
    	t_participators.clear();
    	t_sponsor =  "";
    	t_title = "";
    	s1 = "";
    	s2 = "";
    	s = "";
    }
    infile2.close();
    return true;
}

bool Storage::writeToFile(void){
	fstream outfile1,outfile2;
	outfile1.open(Path::userPath,ios::out);
	if(!outfile1.is_open()) {
		return false;
	}
	for(auto iter = m_userList.begin(); iter != m_userList.end(); iter++){
		outfile1 << "\"" << iter->getName() << "\",\"" << iter->getPassword() << "\",\"" 
				 << iter->getEmail() << "\",\"" << iter->getPhone() << "\"" <<endl;
	}
	outfile1.close();


	outfile2.open(Path::meetingPath,ios::out);
	if(!outfile2.is_open()){
		return false;
	}
	for(auto iter2 = m_meetingList.begin(); iter2 != m_meetingList.end(); iter2++){
		outfile2 << "\"" << iter2->getSponsor() <<  "\",\"";
		vector<string>::iterator it;
		vector<string> temp = iter2->getParticipator();
		for(it = temp.begin(); it != temp.end(); it++){
			if(it != temp.end() - 1){
				outfile2 << *it << "&";
			} else {
				outfile2 << *it;
			}
		}
		outfile2 << "\",\"" << Date::dateToString(iter2->getStartDate()) << "\",\"" 
					<< Date::dateToString(iter2->getEndDate()) << "\",\"" << iter2->getTitle() << "\"" << endl;
	}
	outfile2.close();
	return true;
}

std::shared_ptr<Storage> Storage::m_instance = NULL;
std::shared_ptr<Storage> Storage::getInstance(void){
	if(Storage::m_instance == NULL){
		m_instance.reset(new Storage);
	}
	return m_instance;
}

void Storage::createUser(const User & t_user){
	m_userList.push_back(t_user);
	m_dirty=true;
}

list<User> Storage::queryUser(function<bool(const User &)> filter) const{
	list<User> temp;
	for(auto iter = m_userList.begin(); iter != m_userList.end(); iter++){
		if(filter(*iter)){
			temp.push_back(*iter);
		}
	}
	return temp;
}

int Storage::updateUser(function<bool(const User &)> filter,
	function<void(User &)> Switcher){
	int count = 0;
	for(auto iter = m_userList.begin(); iter != m_userList.end(); iter++){
		if(filter(*iter)){
			Switcher(*iter);
			count += 1;
			m_dirty = true;
		}
	}
	return count;
}

int Storage::deleteUser(function<bool(const User &)> filter){
	int count = 0;
	for(auto iter = m_userList.begin(); iter != m_userList.end();){
		if(filter(*iter)){
			iter = m_userList.erase(iter);
			count += 1;
			m_dirty = true;
		}else{
			iter++;
		}
	}
	return count;
}

void Storage::createMeeting(const Meeting & t_meeting){
	m_meetingList.push_back(t_meeting);
	m_dirty=true;
}

list<Meeting> Storage::queryMeeting(function<bool(const Meeting &)> filter) const{
	list<Meeting> temp;
	for(auto iter = m_meetingList.begin(); iter != m_meetingList.end(); iter++){
		if(filter(*iter)){
			temp.push_back(*iter);
		}
	}
	return temp;
}

int Storage::updateMeeting(function<bool(const Meeting &)> filter,
				function<void(Meeting &)> Switcher) {
	int count = 0;
	for(auto iter = m_meetingList.begin(); iter != m_meetingList.end(); iter++){
		if(filter(*iter)){
			Switcher(*iter);
			count += 1;
			m_dirty = true;
		}
	}
	return count;
}

int Storage::deleteMeeting(function<bool(const Meeting &)> filter){
	int count = 0;
	for(auto iter = m_meetingList.begin(); iter != m_meetingList.end();){
		if(filter(*iter)){
			iter = m_meetingList.erase(iter);
			count += 1;
			m_dirty = true;
		}
		else{
			iter++;
		}
	}
	return count;
}

bool Storage::sync(void){
	writeToFile();
	if(m_dirty){
		m_dirty = false;
		return true;
	} else {
		return false;
	}
}