#ifndef TRAINER_H_
#define TRAINER_H_
#include "AVLTree.h"
#include "pokemon.h"
#include "Key.h"
#include "Exceptions.h"

// trainer class
class Trainer{
private:
	//trainer id
	int trainerID;
	// AVL tree sorted by id
	AVL::Tree<int, Pokemon> pokemonTreeByID;
	// AVL tree sorted by level then id
	AVL::Tree<Key, Pokemon> levelPokemonTree;
	// lowest (leftest) node of levelPokemonTree
	AVL::Node<Key, Pokemon>* maxLevel;
public:
	// default c'tor
	Trainer() : trainerID(0), maxLevel(0){
		pokemonTreeByID = AVL::Tree<int, Pokemon>();
		levelPokemonTree = AVL::Tree<Key, Pokemon>();
	}
	// data c'tor
	Trainer(int id) : trainerID(id), maxLevel(0){
		pokemonTreeByID = AVL::Tree<int, Pokemon>();
		levelPokemonTree = AVL::Tree<Key, Pokemon>();
	}
	// copy c'tor
	Trainer(const Trainer& trn) : trainerID(trn.trainerID){
		pokemonTreeByID = trn.pokemonTreeByID;
		levelPokemonTree = trn.levelPokemonTree;
		maxLevel = trn.maxLevel ? levelPokemonTree.findMin() : 0;
	}
	// assignment operator
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
	// comparison (equals) operator
	inline bool operator==(const Trainer& other)const{
		return (trainerID == other.trainerID);
	}
	// d'tor
	~Trainer(){};

	//get trainer ID
	inline int getID()const{
		return trainerID;
	}

	// get tree of pokemon
	inline AVL::Tree<int, Pokemon>* getPokemonTreeByID(){
		return &pokemonTreeByID;
	}

	// get tree of pokemon by level
	inline AVL::Tree<Key, Pokemon>* getlevelPokemonTree(){
		return &levelPokemonTree;
	}

	// get lowest (leftest) node of level tree
	inline AVL::Node<Key, Pokemon>* getMaxLevel()const{
		return maxLevel;
	}

	// find lowest (leftest) node of level tree
	void updateMaxLevel(){
		maxLevel = levelPokemonTree.findMin();
	}
};
#endif