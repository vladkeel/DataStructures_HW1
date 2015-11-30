#ifndef POKEDEX_H_
#define POKEDEX_H_
#include "AVLTree.h"
#include "pokemon.h"
#include "Trainer.h"
#include "Level.h"
#include "newList.h"
#include "Exceptions.h"
namespace Pokedex{
	class findTrainer{
	private:
		int trainerID;
	public:
		findTrainer(int trainerID) :trainerID(trainerID){};
		bool operator()(Trainer& trainer){
			return trainer.getID() == trainerID;
		}
	};
	class PutPokemonInArray{
	private:
		int* pokemonArray;
	public:
		PutPokemonInArray(int* pokemonArray) :pokemonArray(pokemonArray){};
		void operator()(Pokemon& pokemon){
			int static i = 0;
			pokemonArray[i] = pokemon.getID();
		}
	};
	class scanLevels{
	private:
		int* pokemonArray;
	public:
		scanLevels(int* pokemonArray) :pokemonArray(pokemonArray){};
		void operator()(Level& level){
			level.getPokemonTreeByID()->inorderScan(PutPokemonInArray(pokemonArray));
		}
	};

	class invalidInputException : public std::exception{};
	class failureException : public std::exception{};

	class Pokedex{
	private:
		LinkedList::List<Trainer> trainerList;
		AVL::Tree<Pokemon> pokemonTreeByID;
		AVL::Tree<Level> levelPokemonTree;
		AVL::Node<Level>* maxLevel;
		Pokedex(const Pokedex&);
		Pokedex& operator=(const Pokedex&);

		void updateMaxLevel(){
			maxLevel = levelPokemonTree.findMax();
		}
	public:
		Pokedex() :maxLevel(0){
			trainerList = LinkedList::List<Trainer>();
			pokemonTreeByID = AVL::Tree<Pokemon>();
			levelPokemonTree = AVL::Tree<Level>();
		}
		~Pokedex(){
			if (maxLevel)
				delete maxLevel;
		}
		void catchPokemon(int pokemonID, int trainerID, int level){
			if (pokemonID <= 0 || trainerID <= 0 || level <= 0)
				throw InvalidInput();
			Trainer trainer = Trainer();
			try {
				trainer = trainerList.find(findTrainer(trainerID));
			}
			catch(LinkedList::DoesntExist e) {
				throw Failure();
			}
			if (pokemonTreeByID.find(pokemonID) != nullptr)
				throw Failure();
			Pokemon pokemon(pokemonID, level, trainerID);
			if (trainer.getPevelPokemonTree()->find(level) == nullptr)
				trainer.getPevelPokemonTree()->insert(level, Level(level));
			AVL::Node<Level>* levelNode = trainer.getPevelPokemonTree()->find(level);
			levelNode->getData().addPokemon(pokemon);
			trainer.updateMaxLevel();
			trainer.getPokemonTreeByID()->insert(pokemonID, pokemon);
			pokemonTreeByID.insert(pokemonID, pokemon);
			if (levelPokemonTree.find(level) == nullptr)
				levelPokemonTree.insert(level, Level(level));
			levelNode = levelPokemonTree.find(level);
			levelNode->getData().addPokemon(pokemon);
			updateMaxLevel();
		}
		void freePokemon(int pokemonID){}
		void levelUp(int pokemonID, int levelIncrease){
			if (pokemonID <= 0 || levelIncrease <= 0)
				throw InvalidInput();
			AVL::Node<Pokemon>* pokemonNode = pokemonTreeByID.find(pokemonID);
			if (!pokemonNode)
				throw Failure();
			int trainerID = pokemonNode->getData().getTrainer();
			int levelNum = pokemonNode->getData().getLevel();
			freePokemon(pokemonID);
			catchPokemon(pokemonID, trainerID, levelNum + levelIncrease);
		}
		int GetTopPokemon(int trainerID);
		int* GetAllPokemonsByLevel(int trainerID, int* numOfPokemon){
			if (trainerID == 0)
				throw InvalidInput();
			AVL::Tree<Level>* levelsTree;
			if (trainerID < 0)
				levelsTree = &levelPokemonTree;
			else {
				Trainer trainer = Trainer();
				try {
					trainer = trainerList.find(findTrainer(trainerID));
				}
				catch (LinkedList::DoesntExist e) {
					throw Failure();
				}
				levelsTree = trainer.getPevelPokemonTree();
			}
			*numOfPokemon = levelsTree->getSize();
			int* pokemonArray = (int*)malloc(*numOfPokemon*sizeof(*pokemonArray));
			if (!pokemonArray)
				throw std::bad_alloc();
			levelsTree->reverseInorderScan(scanLevels(pokemonArray));
			return pokemonArray;
		}
		void EvolvePokemon(int pokemonID, int evolvedID);
		void UpdateLevels(int stoneCode, int stoneFactor){
			int size;
			int* allPokemon = GetAllPokemonsByLevel(-1, &size);
			int leavePokemon[size] = { 0 };
			int updatePokemon[size] = { 0 };
			int countLeave=0, countUpdate = 0;
			for (int i = 0; i < size; i++){
				if ()
			}
		}
	};
}
#endif