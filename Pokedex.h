#ifndef POKEDEX_H_
#define POKEDEX_H_
#include "AVLTree.h"
#include "pokemon.h"
#include "Trainer.h"
#include "newList.h"
#include "Exceptions.h"
namespace Pokedex{
	class findTrainer{
	private:
		int trainerID;
	public:
		findTrainer(int trainerID) :trainerID(trainerID){};
		const bool operator()(const Trainer& trainer) const{
			return trainer.getID() == trainerID;
		}
	};
	class PutPokemonInArray{
	private:
		int* pokemonArray;
	public:
		PutPokemonInArray(int* pokemonArray) :pokemonArray(pokemonArray){};
		void operator()(AVL::Node<Key, Pokemon>& node){
			static int i = 0;
			pokemonArray[i++] = node.getData().getID();
		}
	};
	class UpdatePokemonLevels{
	private:
		int stoneCode;
		int stoneFactor;
	public:
		UpdatePokemonLevels(int stoneCode, int stoneFactor) : stoneCode(stoneCode), stoneFactor(stoneFactor){};
		void operator()(AVL::Node<int, Pokemon>& node){
			if (node.getKey() % stoneCode == 0){
				node.getData().setLevel(node.getData().getLevel() * stoneFactor);
			}
		}
	};
	template <class S, class T> class PutNodesInArray{
	private:
		AVL::Node<S, T>* nodesArray;
	public:
		PutNodesInArray(AVL::Node<S, T>* nodesArray) :nodesArray(nodesArray){};
		void operator()(AVL::Node<S, T>& node){
			static int c = 0;
			nodesArray[c++] = node;
		}
	};
	static void updateLevelTree(AVL::Tree<Key, Pokemon>* tree, int stoneCode, int stoneFactor){
		const int size = tree->getSize();
		AVL::Node<Key, Pokemon>* pokemonArray = (AVL::Node<Key, Pokemon>*)malloc(size*sizeof(*pokemonArray));
		if (!pokemonArray)
			throw std::bad_alloc();
		tree->inorderScan(PutNodesInArray<Key, Pokemon>(pokemonArray));
		AVL::Node<Key, Pokemon>* changeArray = (AVL::Node<Key, Pokemon>*)malloc(size*sizeof(*pokemonArray));
		if (!changeArray)
			throw std::bad_alloc();
		AVL::Node<Key, Pokemon>* stayArray = (AVL::Node<Key, Pokemon>*)malloc(size*sizeof(*pokemonArray));
		if (!stayArray)
			throw std::bad_alloc();
		AVL::Node<Key, Pokemon>* newArray = (AVL::Node<Key, Pokemon>*)malloc(size*sizeof(*pokemonArray));
		if (!newArray)
			throw std::bad_alloc();
		for (int i = 0, countChange = 0, countStay = 0; i < size; i++){
			int id = pokemonArray[i].getKey().getPokemon();
			int level = pokemonArray[i].getKey().getLevel();
			Pokemon pokemon = pokemonArray[i].getData();
			if (id % stoneCode == 0){
				pokemon.setLevel(level*stoneFactor);
				changeArray[countChange] = AVL::Node<Key, Pokemon>(Key(level*stoneFactor, id), pokemon);
				++countChange;
			}
			else {
				stayArray[countStay] = AVL::Node<Key, Pokemon>(Key(level, id), pokemon);
				++countStay;
			}
		}
		for (int i = 0, countChange = 0, countStay = 0; i < size; i++){
			if (stayArray[countStay].getKey() < changeArray[countChange].getKey()){
				newArray[i] = stayArray[countStay];
				++countStay;
			}
			else {
				newArray[i] = changeArray[countChange];
				++countChange;
			}
		}
	}

	class Pokedex{
	private:
		LinkedList::List<Trainer> trainerList;
		AVL::Tree<int,Pokemon> pokemonTreeByID;
		AVL::Tree<Key, Pokemon> levelPokemonTree;
		AVL::Node<Key, Pokemon>* maxLevel;
		Pokedex(const Pokedex&);
		Pokedex& operator=(const Pokedex&);

		void updateMaxLevel(){
			maxLevel = levelPokemonTree.findMin();
		}
	public:
		Pokedex() :maxLevel(0){
			trainerList = LinkedList::List<Trainer>();
			pokemonTreeByID = AVL::Tree<int, Pokemon>();
			levelPokemonTree = AVL::Tree<Key, Pokemon>();
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
				const findTrainer finder(trainerID);
				trainer = trainerList.find(finder);
			}
			catch (DoesntExist e) {
				throw Failure();
			}
			if (pokemonTreeByID.find(pokemonID) != nullptr)
				throw Failure();
			Pokemon pokemon(pokemonID, level, trainerID);
			pokemonTreeByID.insert(pokemonID, pokemon);
			levelPokemonTree.insert(Key(level, pokemonID), pokemon);
			trainer.getlevelPokemonTree()->insert(Key(level, pokemonID), pokemon);
			trainer.getPokemonTreeByID()->insert(pokemonID, pokemon);
			updateMaxLevel();
			trainer.updateMaxLevel();
		}
		void freePokemon(int pokemonID){}
		void levelUp(int pokemonID, int levelIncrease){
			if (pokemonID <= 0 || levelIncrease <= 0)
				throw InvalidInput();
			AVL::Node<int,Pokemon>* pokemonNode = pokemonTreeByID.find(pokemonID);
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
			AVL::Tree<Key,Pokemon>* levelsTree;
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
				levelsTree = trainer.getlevelPokemonTree();
			}
			*numOfPokemon = levelsTree->getSize();
			int* pokemonArray = (int*)malloc(*numOfPokemon*sizeof(*pokemonArray));
			if (!pokemonArray)
				throw std::bad_alloc();
			levelsTree->inorderScan(PutPokemonInArray(pokemonArray));
			return pokemonArray;
		}

		void EvolvePokemon(int pokemonID, int evolvedID);

		void UpdateLevels(int stoneCode, int stoneFactor){
			if (stoneCode < 1 || stoneFactor < 1){
				throw InvalidInput();
			}
			pokemonTreeByID.inorderScan(UpdatePokemonLevels(stoneCode, stoneFactor));
			updateLevelTree(&levelPokemonTree, stoneCode, stoneFactor);
		}
	};
}
#endif