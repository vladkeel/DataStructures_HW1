/*
 * Exceptions.h
 *
 *  Created on: Nov 26, 2015
 *      Author: mac
 */

#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_
typedef enum {
	SUCCESS = 0,
	FAILURE = -1,
	ALLOCATION_ERROR = -2,
	INVALID_INPUT = -3
} ExceptionStatusType;
class BaseException : public std::exception{
private:
	ExceptionStatusType type;
public:
	BaseException() :exception(){};
	BaseException(ExceptionStatusType type) :exception(), type(type){};
	ExceptionStatusType getType()const{
		return type;
	}
};
class IsEmptyException : public BaseException{
public:
	IsEmptyException() :BaseException(FAILURE){};
};
class InvalidInput : public BaseException{
public:
	InvalidInput() :BaseException(INVALID_INPUT){};
};
class ElementNotFound : public BaseException {
public:
	ElementNotFound() :BaseException(FAILURE){};
};
class AlreadyExists : public BaseException {
public:
	AlreadyExists() :BaseException(FAILURE){};
};
class DoesntExist : public BaseException {
public:
	DoesntExist() : BaseException(FAILURE){};
};

class Failure : public BaseException {
public:
	Failure() : BaseException(FAILURE){};
};

#endif /* EXCEPTIONS_H_ */
