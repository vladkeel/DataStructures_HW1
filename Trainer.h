#ifndef TRAINER_H_
#define TRAINER_H_
#include "AVLTree.h"
#include "pokemon.h"
#include "Key.h"
#include "Exceptions.h"
class Trainer{
private:
	int trainerID;
	AVL::Tree<int, Pokemon> pokemonTreeByID;
	AVL::Tree<Key, Pokemon> levelPokemonTree;
	AVL::Node<Key, Pokemon>* maxLevel;
public:
	Trainer() : trainerID(0), maxLevel(0){
		pokemonTreeByID = AVL::Tree<int, Pokemon>();
		levelPokemonTree = AVL::Tree<Key, Pokemon>();
	}
	Trainer(int id) : trainerID(id), maxLevel(0){
		pokemonTreeByID = AVL::Tree<int, Pokemon>();
		levelPokemonTree = AVL::Tree<Key, Pokemon>();
	}
	Trainer(const Trainer& trn) : trainerID(trn.trainerID){
		pokemonTreeByID = trn.pokemonTreeByID;
		levelPokemonTree = trn.levelPokemonTree;
		maxLevel = trn.maxLevel ? levelPokemonTree.findMin() : 0;
	}
	Trainer& operator=(const Trainer& trn){
		if (this != &trn){
			if (maxLevel)
				delete maxLevel;
			trainerID = trn.trainerID;
			pokemonTreeByID = trn.pokemonTreeByID;
			levelPokemonTree = trn.levelPokemonTree;
			maxLevel = trn.maxLevel ? levelPokemonTree.findMax() : 0;
		}
		return *this;
	}
	bool operator==(const Trainer& other)const{
		return (trainerID == other.trainerID);
	}
	~Trainer(){
		if (maxLevel){
			delete maxLevel;
		}
	}
	int getID()const{
		return trainerID;
	}
	AVL::Tree<int, Pokemon>* getPokemonTreeByID(){
		return &pokemonTreeByID;
	}
	AVL::Tree<Key, Pokemon>* getlevelPokemonTree(){
		return &levelPokemonTree;
	}
	AVL::Node<Key, Pokemon>* getMaxLevel()const{
		return maxLevel;
	}
	void updateMaxLevel(){
		maxLevel = levelPokemonTree.findMin();
	}
};
#endif