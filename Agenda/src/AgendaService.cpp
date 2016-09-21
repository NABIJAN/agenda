#include "AgendaService.hpp"
using namespace std;

/**
     * constructor
     */
    AgendaService::AgendaService(){
        m_storage = Storage::getInstance();
    }

    /**
     * destructor
     */
    AgendaService::~AgendaService(){
        m_storage->sync();
    }
    

    /**
     * check if the username match password
     * @param userName the username want to login
     * @param password the password user enter
     * @return if success, true will be returned
     */
    bool AgendaService::userLogIn(const std::string userName, const std::string password){
        list<User> temp = m_storage->queryUser([userName,password](const User &user1)->bool{
            if((user1.getName() == userName) && (user1.getPassword() == password)){
                return true;
            } else {
                return false;
            }
        });
        if(temp.size() == 0){
            return false;
        } else {
            return true;
        }

    }

    /**
     * regist a user
     * @param userName new user's username
     * @param password new user's password
     * @param email new user's email
     * @param phone new user's phone
     * @return if success, true will be returned
     */
    bool AgendaService::userRegister(const std::string userName, const std::string password,
                      const std::string email, const std::string phone){
        list<User> temp = m_storage->queryUser([userName](const User &user1)->bool {
            if(user1.getName() == userName){
                return true;
            } else {
                return false;
            }
        });
        if(temp.size() != 0){
            return false;
        } else {
            m_storage->createUser(User(userName,password,email,phone));
            return true;
        }
    }

    /**
     * delete a user
     * @param userName user's username
     * @param password user's password
     * @return if success, true will be returned
     */
    bool AgendaService::deleteUser(const std::string userName, const std::string password){
        int num = m_storage->deleteUser([userName,password](const User &user1)->bool{
            if((user1.getName() == userName) && (user1.getPassword() == password)){
                return true;
            } else {
                return false;
            }
        });
        if(num == 0){
            return false;
        } else {
            list<Meeting> temp = listAllMeetings(userName);
            for(auto iter = temp.begin(); iter != temp.end(); iter++){
                string temp1 = iter->getTitle();
                string temp2 = iter->getSponsor();
                deleteMeeting(temp2,temp1);
            }
            return true;
        }
    }

    /**
     * list all users from storage
     * @return a user list result
     */
    std::list<User> AgendaService::listAllUsers(void) const{
        list<User> temp = m_storage->queryUser([](const User &user1)->bool{
            return true;
        });
        return temp;
    }

    /**
     * create a meeting
     * @param userName the sponsor's userName
     * @param title the meeting's title
     * @param participator the meeting's participator
     * @param startData the meeting's start date
     * @param endData the meeting's end date
     * @return if success, true will be returned
     */
    bool AgendaService::createMeeting(const std::string userName, const std::string title,
                       const std::string startDate, const std::string endDate,
                       const std::vector<std::string> participator){
        Date sd = Date::stringToDate(startDate);
        Date ed = Date::stringToDate(endDate);
        
        if(participator.size() == 0) return false;

        std::list<User> ul =m_storage ->queryUser([=] (const User& u){
            if(u.getName()==userName) return true;
            for(auto &it : participator){
                if(u.getName()==it) return true;
            }
            return false;
        });
        if((participator.size()+1) != ul.size() ){
            return false;
        }

        
        if(!(Date::isValid(sd) && Date::isValid(ed) && (sd < ed))){
            return false;
        }
        for(auto iter = participator.begin(); iter != participator.end(); iter++){
            if(*iter == userName) return false;
        }

       for(int i = 0; i < participator.size() - 1; i++){
            for(int j = i + 1; j < participator.size(); j++){
                if(participator[i] == participator[j]) return false;
            }
       }

        list<Meeting> temp4 = m_storage->queryMeeting([title](const Meeting &m)->bool{
            if(m.getTitle() == title){
                return true;
            } else {
                return false;
            }
        });
        if(temp4.size() != 0) return false;

        for(auto iter1 = participator.begin();iter1 != participator.end(); iter1++){
            string name = *iter1;
            list<Meeting> tempmm = listAllMeetings(name);

            for(auto iter2 = tempmm.begin(); iter2 != tempmm.end(); iter2++){
                if(!(iter2->getStartDate() >= ed || iter2->getEndDate() <= sd)){
                    return false;
                }
            }
        }

        list<Meeting> temp1 = listAllMeetings(userName);

        for(auto iter = temp1.begin(); iter != temp1.end(); iter++){
            if(!(iter->getStartDate() >= ed || iter->getEndDate() <= sd)){
                return false;
            }
        }
            m_storage->createMeeting(Meeting(userName,participator,Date::stringToDate(startDate),
                Date::stringToDate(endDate),title));
            return true;
    }

    /**
     * search meetings by username and title (user as sponsor or participator)
     * @param uesrName the user's userName
     * @param title the meeting's title
     * @return a meeting list result
     */
    std::list<Meeting> AgendaService::meetingQuery(const std::string userName,
                                    const std::string title) const{
        list<Meeting> temp = m_storage->queryMeeting([title](const Meeting &m)->bool{
            if(m.getTitle() == title) {
                return true;
            } else {
                return false;
            }
        });
        list<Meeting>::iterator iter;
        list<Meeting> temp1;
        for(iter = temp.begin(); iter != temp.end(); iter++){
            if(iter->getSponsor() == userName){
                temp1.push_back(*iter);
            }
            for(int i=0;i<iter->getParticipator().size();i++){
                if(iter->getParticipator()[i]==userName){
                    temp1.push_back(*iter);
                }
            }
        }

        return temp1;
    }
    /**
     * search a meeting by username, time interval (user as sponsor or participator)
     * @param uesrName the user's userName
     * @param startDate time interval's start date
     * @param endDate time interval's end date
     * @return a meeting list result
     */
    std::list<Meeting> AgendaService::meetingQuery(const std::string userName,
                                    const std::string startDate,
                                    const std::string endDate) const{
        Date sd = Date::stringToDate(startDate);
        Date ed = Date::stringToDate(endDate);
        list<Meeting> temp1;
        if(!(Date::isValid(sd) && Date::isValid(ed) && (sd <= ed))){
            return temp1;
        }

        list<Meeting> temp = m_storage->queryMeeting([userName,sd,ed](const Meeting &m)->bool{
            if(m.getSponsor() == userName || m.isParticipator(userName)){
                if(!(m.getStartDate() > ed || m.getEndDate() < sd)) {
                    return true;
                } else {
                    return false;
                }
            }
            else return false;
        });
        return temp;
    }

    /**
     * list all meetings the user take part in
     * @param userName user's username
     * @return a meeting list result
     */
    std::list<Meeting> AgendaService::listAllMeetings(const std::string userName) const{
        list<Meeting> temp = m_storage->queryMeeting([userName](const Meeting &m)->bool{
           if(userName == m.getSponsor() || m.isParticipator(userName)){
            return true;
           }
           else {
            return false;
           }
        });
        return temp;
    }

    /**
     * list all meetings the user sponsor
     * @param userName user's username
     * @return a meeting list result
     */
    std::list<Meeting> AgendaService::listAllSponsorMeetings(const std::string userName) const{
        list<Meeting> temp = m_storage->queryMeeting([userName](const Meeting &m)->bool{
            if(m.getSponsor() == userName) {
                return true;
            } else {
                return false;
            }
        });
        return temp;
    }

    /**
     * list all meetings the user take part in and sponsor by other
     * @param userName user's username
     * @return a meeting list result
     */
    std::list<Meeting> AgendaService::listAllParticipateMeetings(
        const std::string userName) const{
        list<Meeting> temp = m_storage->queryMeeting([](const Meeting &m)->bool{
            return true;
        });
        list<Meeting>::iterator iter;
        list<Meeting> temp1;
        for(iter = temp.begin(); iter != temp.end(); iter++){
            
            for(int i=0;i<iter->getParticipator().size();i++){
                if(iter->getParticipator()[i]==userName){
                    temp1.push_back(*iter);
                }
            }
        }
        return temp1;
    }

    /**
     * delete a meeting by title and its sponsor
     * @param userName sponsor's username
     * @param title meeting's title
     * @return if success, true will be returned
     */
    bool AgendaService::deleteMeeting(const std::string userName, const std::string title){
        int num = m_storage->deleteMeeting([userName,title](const Meeting &m)->bool{
            if(m.getSponsor() == userName && m.getTitle() == title){
                return true;
            } else {
                return false;
            }
        });
        if(num == 0){
            return false;
        } else {
            return true;
        }
    }

    /**
     * delete all meetings by sponsor
     * @param userName sponsor's username
     * @return if success, true will be returned
     */
    bool AgendaService::deleteAllMeetings(const std::string userName){
        int num = m_storage->deleteMeeting([userName](const Meeting &m)->bool{
            if(m.getSponsor() == userName ){
                return true;
            } else {
                return false;
            }
        });
        if(num == 0){
            return false;
        } else {
            return true;
        }
    }

    /**
     * start Agenda service and connect to storage
     */
    void AgendaService::startAgenda(void){
        m_storage = Storage::getInstance();
    }

    /**
     * quit Agenda service
     */
    void AgendaService::quitAgenda(void){
        m_storage->sync();
    }
