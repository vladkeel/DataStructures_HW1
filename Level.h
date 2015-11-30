/*
 * Level.h
 *
 *  Created on: Nov 26, 2015
 *      Author: mac
 */

#ifndef LEVEL_H_
#define LEVEL_H_

#include <iostream>
#include "AVLTree.h"
#include "Exceptions.h"
#include "pokemon.h"
#include <assert.h>

using namespace AVL;

class Level{
	int levelNum;
	Tree<Pokemon> LevelPokemonsByID;
	Node<Pokemon>* minIDPokemon;

public :
	Level(const int& level) : levelNum(level), minIDPokemon(NULL) {};
	Level(const Level& level){
		*this = level;
	}
	Level& operator=(const Level& copyLevel){
		if( this != &copyLevel){
			levelNum = copyLevel.levelNum;
			LevelPokemonsByID = copyLevel.LevelPokemonsByID; // if the tree of copyLevel is empty we have a problem. What if bad_alloc is thrown??
			int minID = (copyLevel.minIDPokemon)->getKey(); // getKey of Node*
			minIDPokemon = LevelPokemonsByID.find(minID); // should it be (tree.find(minID))->getData()??? minIDPokemon gets pokemon* ot node*?
		}
		return *this;
	}
	void addPokemon(const Pokemon& p){
		assert(p.getLevel() == levelNum);
		LevelPokemonsByID.insert(p.getID(), p);// bad_alocc, we're assuming that tree doesn't have p.ID
		if(!minIDPokemon)
			minIDPokemon = LevelPokemonsByID.find(p.getID());
		if( p.getID() < minIDPokemon->getKey())
			minIDPokemon = LevelPokemonsByID.find(p.getID());
	}
	void removePokemon(int ID){
		LevelPokemonsByID.remove(ID);//may throw NotFound exception
		minIDPokemon = LevelPokemonsByID.findMin();
		if(!minIDPokemon)
			throw IsEmptyException();
		return;
	}
	Tree<Pokemon>* getLevelPokemonTree(){
		return &LevelPokemonsByID;
	}

	int getMinID(){
		assert(minIDPokemon);
		return minIDPokemon->getKey();
	}

	int getLevel(){
		return levelNum;
	}
	// need a function for GetTopLevelPokemons
};

#endif /* LEVEL_H_ */
