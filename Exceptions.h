/*
 * Exceptions.h
 *
 *  Created on: Nov 26, 2015
 *      Author: mac
 */

#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

class IsEmptyException : public std::exception{};
class InvalidInput : public std::exception{};
class ElementNotFound : public std::exception {} ;
class AlreadyExists : public std::exception {} ;
class DoesntExist : public std::exception {} ;

class Failure : public std::exception {} ;


#endif /* EXCEPTIONS_H_ */
