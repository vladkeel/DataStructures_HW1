/*
 * Key.h
 *
 *  Created on: Nov 30, 2015
 *      Author: mac
 */

#ifndef KEY_H_
#define KEY_H_

class Key{
	int levelNum;
	int pokemonID;
public:
	Key():levelNum(0), pokemonID(0){};
	Key(int level, int pokemon) : levelNum(level), pokemonID(pokemon){};
	bool operator==(const Key& k){
		return (levelNum==k.levelNum)&&(pokemonID==k.pokemonID);
	}
	bool operator<(const Key& k) const{
		if(levelNum>k.levelNum)
			return true;
		else if(levelNum==k.levelNum)
			return pokemonID < k.pokemonID ? true : false;
		return false;
	}
	int getLevel() const{
		return levelNum;
	}
	int getPokemon() const{
		return pokemonID;
	}

};


#endif /* KEY_H_ */
