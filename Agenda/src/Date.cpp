#include "Date.hpp"
#include <string>
#include <iostream>
using namespace std;

Date::Date(){
	m_year = 0;
	m_month = 0;
	m_day = 0;
	m_hour = 0;
	m_minute = 0;
}

Date::Date(int t_year, int t_month, int t_day, int t_hour, int t_minute){
	m_year = t_year;
	m_month = t_month;
	m_day = t_day;
	m_hour = t_hour;
	m_minute = t_minute;
}

Date::Date(std::string dateString){
	if(dateString.length() != 16){
		m_year = 0;
		m_month = 0;
		m_day = 0;
		m_hour = 0;
		m_minute = 0;
	} else {
		if(dateString[4] == '-' && dateString[7] == '-' && dateString[10] == '/' && dateString[13] == ':'){
			int i;
			for(i = 0; i < 16; i++){
				if(i != 4 && i != 7 && i != 10 && i != 13){
					if(dateString[i] >= '0' && dateString[i] <= '9'){
					} else {
						m_year = 0;
						m_month = 0;
						m_day = 0;
						m_hour = 0;
						m_minute = 0;
						break;					
					}
				}
			}
			m_year = (dateString[0] - 48)* 1000 + (dateString[1] - 48 ) * 100 + (dateString[2] -48 ) * 10 + dateString[3] - 48 ;
			m_month = (dateString[5] - 48) * 10 + dateString[6] - 48;
			m_day = (dateString[8] - 48) * 10 + dateString[9] - 48;
			m_hour = (dateString[11] - 48 ) * 10 + dateString[12] - 48;
			m_minute = (dateString[14] -48) * 10 + dateString[15] - 48;
		} else {
			m_year = 0;
			m_month = 0;
			m_day = 0;
			m_hour = 0;
			m_minute = 0;
		}
	}
}

int Date::getYear(void) const{
	return m_year;
}

void Date::setYear(const int t_year){
	m_year = t_year;
}

int Date::getMonth(void) const{
	return m_month;
}

void Date::setMonth(const int t_month){
	m_month = t_month;
}

int Date::getDay(void) const{
	return m_day;
}

void Date::setDay(const int t_day){
	m_day = t_day;
}

int Date::getHour(void) const{
	return m_hour;
}

void Date::setHour(const int t_hour){
	m_hour = t_hour;
}

int Date::getMinute(void) const{
	return m_minute;
}

void Date::setMinute(const int t_minute){
	m_minute = t_minute;
}

bool Date::isValid(const Date t_date){
	if((t_date.getYear() > 9999 || t_date.getYear() < 1000) || (t_date.getMonth() > 12 || t_date.getMonth() <= 0) || 
		(t_date.getHour() >= 24 || t_date.getHour() < 0) || (t_date.getMinute() >= 60 || t_date.getMinute() < 0)){
		return false;
	} else {
		if(t_date.getMonth() == 2){
			if((t_date.getYear() % 400 == 0) || (t_date.getYear() % 100 != 0 && t_date.getYear() % 4 == 0)){
				if(t_date.getDay() > 29 || t_date.getDay() <= 0){
					return false;
				} else {
					return true;
				}
			} else {
				if(t_date.getDay() > 28 || t_date.getDay() <= 0){
					return false;
				} else {
					return true;
				}
			}
		} else {
			if(t_date.getMonth() == 1 || t_date.getMonth() == 3 || t_date.getMonth() == 5 || t_date.getMonth() == 7 || 
				t_date.getMonth() == 8 || t_date.getMonth() == 10 || t_date.getMonth() == 12){
				if(t_date.getDay() > 31 || t_date.getDay() <= 0){
					return false;
				} else {
					return true;
				}
			} else {
				if(t_date.getDay() > 30 || t_date.getDay() <= 0){
					return false;
				} else {
					return true;
				}
			}
		}
	}
}

Date Date::stringToDate(const std::string t_dateString){
	Date temp;
	if(t_dateString.length() != 16){
		temp = Date(0,0,0,0,0);
		return temp;
	} else {
		if(t_dateString[4] == '-' && t_dateString[7] == '-' && t_dateString[10] == '/' && t_dateString[13] == ':'){
			int i;
			for(i = 0; i < 16; i++){
				if(i != 4 && i != 7 && i != 10 && i != 13){
					if(t_dateString[i] >= '0' && t_dateString[i] <= '9'){
					} else {
						temp = Date(0,0,0,0,0);
						return temp;					
					}
				}
			}
			temp = Date(t_dateString);
			return temp;
		} else {
			temp = Date(0,0,0,0,0);
			return temp;
		}
	}
}

std::string Date::dateToString(Date t_date){
 	string temp = "0000-00-00/00:00";
 	if(isValid(t_date)){
 		temp[0]=t_date.getYear() / 1000 + 48;
 		temp[1]=t_date.getYear() % 1000 / 100 + 48;
 		temp[2]=t_date.getYear() % 100 / 10 + 48;
 		temp[3]=t_date.getYear() % 10 + 48;
 		temp[4]='-';
 		temp[5]=t_date.getMonth()/10 + 48;
 		temp[6]=t_date.getMonth()%10 + 48;
 		temp[7]='-';
 		temp[8]=t_date.getDay()/10 + 48;
 		temp[9]=t_date.getDay()%10 + 48;
 		temp[10]='/';
 		temp[11]=t_date.getHour()/10 + 48;
 		temp[12]=t_date.getHour()%10 + 48;
 		temp[13]=':';
 		temp[14]=t_date.getMinute()/10 + 48;
 		temp[15]=t_date.getMinute()%10 + 48;
 		return temp;
 	}
 	else{
 		return  temp="0000-00-00/00:00";
 	}
 }

  /**
  *  @brief overload the assign operator
  */
  Date & Date::operator=(const Date &t_date){
  	m_year = t_date.m_year;
	m_month = t_date.m_month;
	m_day = t_date.m_day;
	m_hour = t_date.m_hour;
	m_minute = t_date.m_minute;
  }

  /**
  * @brief check whether the CurrentDate is equal to the t_date
  */
  bool Date::operator==(const Date &t_date) const{
  	if(m_year== t_date.m_year && m_month== t_date.m_month && m_day== t_date.m_day && m_hour== t_date.m_hour && m_minute == t_date.m_minute){
  		return true;
  	}
  	return false;
  }


  /**
  * @brief check whether the CurrentDate is  greater than the t_date
  */
  bool Date::operator>(const Date &t_date) const{
  	if(m_year == t_date.m_year && m_month == t_date.m_month && m_day == t_date.m_day && m_hour == t_date.m_hour && m_minute> t_date.m_minute){
  		return true;
  	}
  	else if(m_year == t_date.m_year && m_month == t_date.m_month && m_day == t_date.m_day && m_hour > t_date.m_hour ){
		return true;
  	}
  	else if(m_year == t_date.m_year && m_month == t_date.m_month && m_day > t_date.m_day ){
		return true;
  	}
  	else if(m_year == t_date.m_year && m_month >t_date.m_month  ){
		return true;
  	}
  	else if(m_year > t_date.m_year  ){
		return true;
  	}
  	else{
  		return false;
  	}
  }

  /**
  */
  bool Date::operator<(const Date &t_date) const{
  	if(*this > t_date){
  		return false;
  	}
  	if(*this== t_date){
  		return false;
  	}
  	return true;
  	
  }

  /**
  * @brief check whether the CurrentDate is  greater or equal than the t_date
  */
  bool Date::operator>=(const Date &t_date) const{
  	if(*this> t_date || *this == t_date){
  		return true;
  	}
  	return false;
  }

  /**
  * @brief check whether the CurrentDate is  less than or equal to the t_date
  */
  bool Date::operator<=(const Date &t_date) const{
  	if(*this< t_date || *this == t_date){
  		return true;
  	}
  	return false;
  }



