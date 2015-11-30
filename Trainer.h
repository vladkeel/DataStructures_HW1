#ifndef TRAINER_H_
#define TRAINER_H_
#include "AVLTree.h"
#include "pokemon.h"
#include "Level.h"
#include "Exceptions.h"
void removeFromLevel(AVL::Tree<Level>* levelTree, int levelNum, int pokemonID){
	AVL::Node<Level>* levelNode = levelTree->find(levelNum);
	if (!levelNode)
		throw ElementNotFound();
	AVL::Tree<Pokemon>* pokemonTree = levelNode->getData().getPokemonTreeByID(); // Place holder. need a getter for pokemon tree.
	pokemonTree->remove(pokemonID);
	if (pokemonTree->getSize() == 0){
		levelTree->remove(levelNum);
	}
}
class Trainer{
private:
	int trainerID;
	AVL::Tree<Pokemon> pokemonTreeByID;
	AVL::Tree<Level> levelPokemonTree;
	AVL::Node<Level>* maxLevel;
public:
	Trainer() : trainerID(0), maxLevel(0){
		pokemonTreeByID = AVL::Tree<Pokemon>();
		levelPokemonTree = AVL::Tree<Level>();
	}
	Trainer(int id) : trainerID(id), maxLevel(0){
		pokemonTreeByID = AVL::Tree<Pokemon>();
		levelPokemonTree = AVL::Tree<Level>();
	}
	Trainer(const Trainer& trn) : trainerID(trn.trainerID){
		pokemonTreeByID = trn.pokemonTreeByID;
		levelPokemonTree = trn.levelPokemonTree;
		maxLevel = trn.maxLevel ? levelPokemonTree.find(trn.maxLevel->getData().getLevel()) : 0;
	}
	Trainer& operator=(const Trainer& trn){
		if (this != &trn){
			if (maxLevel)
				delete maxLevel;
			trainerID = trn.trainerID;
			pokemonTreeByID = trn.pokemonTreeByID;
			levelPokemonTree = trn.levelPokemonTree;
			maxLevel = trn.maxLevel ? levelPokemonTree.find(trn.maxLevel->getData().getLevel()) : 0;
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
	AVL::Tree<Pokemon>* getPokemonTreeByID(){
		return &pokemonTreeByID;
	}
	AVL::Tree<Level>* getPevelPokemonTree(){
		return &levelPokemonTree;
	}
	const AVL::Node<Level>* getMaxLevel()const{
		return maxLevel;
	}
	void updateMaxLevel(){
		maxLevel = levelPokemonTree.findMax();
	}
};
#endif