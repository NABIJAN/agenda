#include "AgendaUI.hpp"
#include <cmath>
#include <iomanip>
#include <iostream>
using namespace std;

AgendaUI::AgendaUI(){
	m_userName = "";
	m_userPassword = "";
    m_agendaService.startAgenda();
}
void AgendaUI::OperationLoop(void){
	string operation;
	do{
		startAgenda();
		operation = getOperation();
		if(executeOperation(operation)){
			if(operation == "l") userLogIn();
			if(operation == "r") userRegister();
		} else {
                cout << "[error] error!" << endl;
        }
	} while(operation != "q");
    quitAgenda();
}
/**
     * constructor
     */
    void AgendaUI::startAgenda(void){
    	cout << "-----------------------------Agenda--------------------------------" << endl
    		 << "Action : " << endl
    		 << "l   - log in Agenda by user name and password" << endl
    		 << "r   - register an Agenda account" << endl
    		 << "q   - quit Agenda" << endl
    		 << "-------------------------------------------------------------------" << endl << endl
    		 << "Agenda : ~$ ";
    }

    /**
     * catch user's operation
     * @return the operation
     */
    std::string AgendaUI::getOperation(){
    	string s;
    	cin >> s;
    	return s;
    }

    /**
     * execute the operation
     * @return if the operationloop continue
     */
    bool AgendaUI::executeOperation(std::string t_operation){
    	if(t_operation == "q" || t_operation == "l" || t_operation == "r"){
    		return true;
    	} else {
    		return false;
    	}
    }

    /**
     * user Login
     */
    void AgendaUI::userLogIn(void){
    	cout << "[log in] [username] [password]" << endl
    		 << "[log in] ";
    		 cin >> m_userName >> m_userPassword;
    		 if(m_agendaService.userLogIn(m_userName,m_userPassword)){
    		 	cout << "[log in] succeed!" << endl;
    		 	string op;
    		 	do{
    		 		cout <<"-----------------------------Agenda--------------------------------" << endl
    		 			 << "o   - log out Agenda" << endl
    		 			 << "dc  - delete Agenda account" << endl
    		 			 << "lu  - list all Agenda user" << endl
    		 			 << "cm  - create a meeting" << endl
    		 			 << "la  - list all meetings" << endl
    		 			 << "las - list all sponsor meeting" << endl
    		 			 << "lap - list all participate meetings" << endl
    		 			 << "qm  - query meeting by title" << endl
    		 			 << "qt  - query meeting by time interval" << endl
    		 			 << "dm  - delete meeting by title" << endl
    		 			 << "da  - delete all meetings" << endl
    		 			 << "------------------------------------------------------------------" << endl << endl
    		 			 << "Agenda@" << m_userName << " : # ";
    		 		op = getOperation();

    		 		if(op == "o") { 
                        userLogOut();
                        break;
                    }
    		 		else if(op == "dc") {
                        deleteUser();
                        break;
                    }
    		 		else if(op == "lu") listAllUsers();
    		 		else if(op == "cm") createMeeting();
    		 		else if(op == "la") listAllMeetings();
    		 		else if(op == "las") listAllSponsorMeetings();
    		 		else if(op == "lap") listAllParticipateMeetings();
    		 		else if(op == "qm") queryMeetingByTitle();
    		 		else if(op == "qt") queryMeetingByTimeInterval();
    		 		else if(op == "dm") deleteMeetingByTitle();
    		 		else if(op == "da") deleteAllMeetings();
    		 		else{
    		 			cout << "[error] error!" << endl;
    		 		}
    		 	} while(op != "o" || op != "dc");

    		 } else {
    		 	cout << "[error] log in fail!" << endl;
    		 }
    }

    /**
     * user regist
     */
    void AgendaUI::userRegister(void){
    	cout << "[register] [username] [password] [email] [phone]" << endl
    		 << "[register] ";
    		 string email, phone;
    		 cin >> m_userName >> m_userPassword >> email >> phone;
    		 if(m_agendaService.userRegister(m_userName,m_userPassword,email,phone)){
    		 	cout << "[register] succeed!" << endl;
    		 } else {
    		 	cout << "[error] register fail!" << endl;
    		 }
    }

    /**
     * quit the Agenda
     */
    void AgendaUI::quitAgenda(void){
        m_agendaService.quitAgenda();
    }

    /**
     * user logout
     */
    void AgendaUI::userLogOut(void){
    	m_userName = "";
    	m_userPassword = "";
    }

    /**
     * delete a user from storage
     */
    void AgendaUI::deleteUser(void){
    	if(m_agendaService.deleteUser(m_userName,m_userPassword)){
    		cout << "[delete Agenda account] succeed!" << endl;
    	} else {
    		cout << "[error] delete Agenda account fail!" << endl;
    	}
    }

    /**
     * list all users from storage
     */
    void AgendaUI::listAllUsers(void){
    	cout << "[list all users]" << endl << endl
    	 	 << "name    " << "email           " << "phone      " << endl;
    	list<User> temp = m_agendaService.listAllUsers();
    	for(auto iter = temp.begin(); iter != temp.end(); iter++){
    		cout << std::left << setw(8) << iter->getName() << setw(16) << iter->getEmail() 
                 << setw(11) << iter->getPhone() << endl;
    	}
    }

    /**
     * user create a meeting with someone else
     */
    void AgendaUI::createMeeting(void){
    	string num;
        bool isv = true;
    	cout << "[create meeting] [the number of participators]" << endl
    		 << "[create meeting] ";
    	cin >> num;
        for(int i = 0; i < num.length(); i++){
            if(num[i] < '0' || num[i] > '9') {
                isv = false;
                break;
            }
        }
        if(isv){
            int num1 = 0;
            for(int i = 0; i < num.length(); i++){
                num1 += (num[i] - 48) * pow(10,num.length() - 1 - i);
            }
            vector<string> par;
            string t_title,st_date, end_date;
            for(int i = 0; i < num1; i++){
                string t_name;
                cout << "[create meeting] [please enter the participator " << i + 1 << "]" << endl
                     << "[create meeting] ";
                cin >> t_name;
                par.push_back(t_name);
            }

            cout << "[create meeting] [title] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << endl
                 << "[create meeting] ";
            cin >> t_title >> st_date >> end_date;
            if(!Date::isValid(Date::stringToDate(st_date)) || !Date::isValid(Date::stringToDate(end_date)) ||
                (Date::stringToDate(st_date) >= Date::stringToDate(end_date))){
                    cout << "[create meeting] error!" << endl;
            } else {
                if(m_agendaService.createMeeting(m_userName,t_title,st_date,end_date,par)){
                    cout << "[create meeting] succeed!" << endl;
                } else {
                    cout << "[create meeting] error!" << endl;
                }
            }/////////////////////////////////
        } else {
            cout << "[create meeting] error!" << endl;
        }
    }

    /**
     * list all meetings from storage
     */
    void AgendaUI::listAllMeetings(void){
    	cout << "[list all meetings]" << endl << endl;
    	list<Meeting> temp = m_agendaService.listAllMeetings(m_userName);
    	printMeetings(temp);
    }

    /**
     * list all meetings that this user sponsored
     */
    void AgendaUI::listAllSponsorMeetings(void){
    	cout << "[list all meetings]" << endl << endl;
    	list<Meeting> temp = m_agendaService.listAllSponsorMeetings(m_userName);
    	printMeetings(temp);
    }

    /**
     * list all meetings that this user take part in
     */
    void AgendaUI::listAllParticipateMeetings(void){
    	cout << "[list all meetings]" << endl << endl;
    	list<Meeting> temp = m_agendaService.listAllParticipateMeetings(m_userName);
    	printMeetings(temp);
    }

    /**
     * search meetings by title from storage
     */
    void AgendaUI::queryMeetingByTitle(void){
    	string title;
    	cout << "[query meeting] [title]" << endl
    		 << "[query meeting] ";
    	cin >> title;
    	list<Meeting> temp = m_agendaService.meetingQuery(m_userName,title);
    	printMeetings(temp);		

    }

    /**
     * search meetings by timeinterval from storage
     */
    void AgendaUI::queryMeetingByTimeInterval(void){
    	string st_date, end_date;
    	cout << "[query meetings] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << endl
    		 << "[query meetings] ";
    	cin >> st_date >> end_date;
    	if(!Date::isValid(Date::stringToDate(st_date)) || !Date::isValid(Date::stringToDate(end_date)) ||
            (Date::stringToDate(st_date) > Date::stringToDate(end_date))){
    		cout << "[query meetings] error!" << endl;
    	} else {
    		cout << "[query meetings] " << endl << endl;
    		list<Meeting> temp = m_agendaService.meetingQuery(m_userName,st_date,end_date);
    		printMeetings(temp);   			 
    	}
    }

    /**
     * delete meetings by title from storage
     */
    void AgendaUI::deleteMeetingByTitle(void){
    	string title;
    	cout << "[delete meeting] [title]" << endl
    		 << "[delete meeting] ";
    	cin >> title;
    	if( m_agendaService.deleteMeeting(m_userName,title)){
    		cout << "[delete meeting by title] succeed!" << endl;
    	} else {
    		cout << "[error] delete meeting fail!" << endl;
    	}
    }

    /**
     * delete all meetings that this user sponsored
     */
    void AgendaUI::deleteAllMeetings(void){
    	if(m_agendaService.deleteAllMeetings(m_userName)){
            
    		cout << "[delete all meeting] succeed!" << endl;
    	} else {
    		cout << "[error] delete meeting fail!" << endl;
    	}
    }

    /**
     * show the meetings in the screen
     */
    void AgendaUI::printMeetings(std::list<Meeting> t_meetings){
		cout << "title       sponsor       start time        end time          participators" << endl;
    	list<Meeting>::iterator	iter;
    	for(iter = t_meetings.begin(); iter != t_meetings.end(); iter++){
    		cout << std::left << setw (12) << iter->getTitle() << setw(14) << iter->getSponsor() 
    			 << setw(18) << Date::dateToString(iter->getStartDate()) 
    			 << setw(18) << Date::dateToString(iter->getEndDate());
    		vector<string>::iterator iter2;
            std::vector<string> temp3 = iter->getParticipator();
    		for(iter2 = temp3.begin(); iter2 != temp3.end(); iter2++){
    			if(iter2 == temp3.end() - 1){
                    cout << *iter2;
                } else {
                    cout << *iter2 << ",";
                }
    		}
    		cout << endl;
    	}
    }