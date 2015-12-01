#ifndef POKEDEX_H_
#define POKEDEX_H_
#include "AVLTree.h"
#include "pokemon.h"
#include "Trainer.h"
#include "newList.h"
#include "Exceptions.h"
namespace Pokedex{
	class IDequals{
		int ID;
	public:
		IDequals(int newID) : ID(newID) {};
		bool operator()(const Trainer& trn) const{
			return trn.getID()==ID;
		}
	};
	class PutPokemonInArray{
	private:
		int* pokemonArray;
	public:
		PutPokemonInArray(int* pokemonArray) :pokemonArray(pokemonArray){};
		void operator()(AVL::Node<Key, Pokemon>& node){
			*pokemonArray = node.getData().getID();
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
	class PutNodesInArray{
	private:
		AVL::Node<Key, Pokemon>* nodesArray;
	public:
		PutNodesInArray(AVL::Node<Key, Pokemon>* nodesArray) :nodesArray(nodesArray){};
		void operator()(AVL::Node<Key, Pokemon>& node){
			*nodesArray = node;
		}
	};
	class PutNodesInTree{
	private:
		AVL::Node<Key, Pokemon>* nodesArray;
	public:
		PutNodesInTree(AVL::Node<Key, Pokemon>* nodesArray) :nodesArray(nodesArray){};
		void operator()(AVL::Node<Key, Pokemon>& node){
			node.setKey((*nodesArray).getKey());
			node.setData((*nodesArray).getData());
		}
	};
	static void updateLevelTree(AVL::Tree<Key, Pokemon>* tree, int stoneCode, int stoneFactor){
		const int size = tree->getSize();
		AVL::Node<Key, Pokemon>* pokemonArray = (AVL::Node<Key, Pokemon>*)malloc(size*sizeof(*pokemonArray));
		if (!pokemonArray)
			throw std::bad_alloc();
		tree->inorderScan(PutNodesInArray(pokemonArray++));
		AVL::Node<Key, Pokemon>* changeArray = (AVL::Node<Key, Pokemon>*)malloc(size*sizeof(*pokemonArray));
		if (!changeArray){
			free(pokemonArray);
			throw std::bad_alloc();
		}
		AVL::Node<Key, Pokemon>* stayArray = (AVL::Node<Key, Pokemon>*)malloc(size*sizeof(*pokemonArray));
		if (!stayArray){
			free(pokemonArray);
			free(changeArray);
			throw std::bad_alloc();
		}
		AVL::Node<Key, Pokemon>* newArray = (AVL::Node<Key, Pokemon>*)malloc(size*sizeof(*pokemonArray));
		if (!newArray){
			free(pokemonArray);
			free(changeArray);
			free(stayArray);
			throw std::bad_alloc();
		}
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
		AVL::Tree<Key, Pokemon> newTree(size);
		newTree.inorderScan(PutNodesInTree(pokemonArray++));
		*tree = newTree;
		free(pokemonArray);
		free(changeArray);
		free(stayArray);
		free(newArray);
	}

	class PokedexDS{
	private:
		LinkedList::List<Trainer> trainerList;
		AVL::Tree<int,Pokemon> pokemonTreeByID;
		AVL::Tree<Key, Pokemon> pokemonLevelsTree;
		AVL::Node<Key, Pokemon>* maxLevel;
		PokedexDS(const PokedexDS&);
		PokedexDS& operator=(const PokedexDS&);

		void updateMaxLevel(){
			maxLevel = pokemonLevelsTree.findMin();
		}
		void updateMaxLevel(int trainerID){
			maxLevel = pokemonLevelsTree.findMin();
			(trainerList.find(IDequals(trainerID))).updateMaxLevel();
		}
	public:
		PokedexDS() :maxLevel(0){
			trainerList = LinkedList::List<Trainer>();
			pokemonTreeByID = AVL::Tree<int, Pokemon>();
			pokemonLevelsTree = AVL::Tree<Key, Pokemon>();
		}
		~PokedexDS(){
			if (maxLevel)
				delete maxLevel;
		}
		void addTrainer(int trainerID){
			if(trainerID <= 0)
				throw InvalidInput();
			Trainer t(trainerID);
			if(trainerList.isIn(t))
				throw AlreadyExists();
			trainerList.insert(t);
		}
		void catchPokemon(int pokemonID, int trainerID, int level){
			if (pokemonID <= 0 || trainerID <= 0 || level <= 0)
				throw InvalidInput();
			Trainer* trainer;
			try {
				const IDequals finder(trainerID);
				trainer = &trainerList.find(finder);
			}
			catch (DoesntExist& e) {
				(void)e;
				throw Failure();
			}
			if (pokemonTreeByID.find(pokemonID) != nullptr)
				throw Failure();
			Pokemon pokemon(pokemonID, level, trainerID);
			pokemonTreeByID.insert(pokemonID, pokemon);
			Key k(level, pokemonID);
			pokemonLevelsTree.insert(k, pokemon);
			trainer->getlevelPokemonTree()->insert(k, pokemon);
			trainer->getPokemonTreeByID()->insert(pokemonID, pokemon);
			updateMaxLevel();
			trainer->updateMaxLevel();
		}
	void freePokemon(int pokemonID){
		if(pokemonID <= 0)
			throw InvalidInput();
		AVL::Node<int,Pokemon>* p = pokemonTreeByID.find(pokemonID);
		if(!p)
			throw Failure();
		int levelTemp = (p->getData()).getLevel();
		int trainerTemp = (p->getData()).getTrainer();

		// removing from 2 main trees :

		pokemonTreeByID.remove(pokemonID);

		 Key temp(levelTemp, pokemonID);
		 pokemonLevelsTree.remove(temp);

		//removing from TrainersList

		Trainer* trainer = &trainerList.find(IDequals(trainerTemp)); // has to be there
		trainer->getPokemonTreeByID()->remove(pokemonID);
		trainer->getlevelPokemonTree()->remove(temp);
		updateMaxLevel(trainerTemp);
	}
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
	int getTopPokemon(int trainerID){
		if( trainerID == 0)
			throw InvalidInput();
		else if(trainerID>0){
			Trainer trainer(trainerID);
				try{
					trainer = trainerList.find(IDequals(trainerID));
				} catch(const ElementNotFound&){
					throw Failure();
				}
				if(!trainer.getMaxLevel())
					return -1;
				return ((trainer.getMaxLevel())->getData()).getID();//MaxLevel->getData() - type Pokemon
			} else{
				if(!maxLevel)
					return -1;
				return (maxLevel->getData()).getID();//MaxLevel->getData() - type Pokemon
			}
		}
		int* GetAllPokemonsByLevel(int trainerID, int* numOfPokemon){
			if (trainerID == 0)
				throw InvalidInput();
			AVL::Tree<Key,Pokemon>* levelsTree;
			if (trainerID < 0)
				levelsTree = &pokemonLevelsTree;
			else {
				Trainer trainer = Trainer();
				try {
					trainer = trainerList.find(IDequals(trainerID));
				}
				catch (LinkedList::DoesntExist& e) {
					(void)e;
					throw Failure();
				}
				levelsTree = trainer.getlevelPokemonTree();
			}
			*numOfPokemon = levelsTree->getSize();
			int* pokemonArray = (int*)malloc(*numOfPokemon*sizeof(*pokemonArray));
			if (!pokemonArray)
				throw std::bad_alloc();
			levelsTree->inorderScan(PutPokemonInArray(pokemonArray++));
			return pokemonArray;
		}

	void evolvePokemon(int pokemonID, int evolvedID){
		  if( pokemonID <= 0 || evolvedID <= 0 )
		  		throw InvalidInput();
		  if(!pokemonTreeByID.find(pokemonID) || pokemonTreeByID.find(evolvedID))
		  		throw Failure();
		  	Pokemon old = pokemonTreeByID.find(pokemonID)->getData();
		  	int trainer = old.getTrainer();
		  	int level = old.getLevel();
		  	freePokemon(pokemonID);
		  	catchPokemon(evolvedID, trainer, level);
	}
		void UpdateLevels(int stoneCode, int stoneFactor){
			if (stoneCode < 1 || stoneFactor < 1){
				throw InvalidInput();
			}
			pokemonTreeByID.inorderScan(UpdatePokemonLevels(stoneCode, stoneFactor));
			updateLevelTree(&pokemonLevelsTree, stoneCode, stoneFactor);
			for (LinkedList::Node<Trainer>* it = trainerList.begin(); it != NULL; it = it->getNext()){
				it->getData().getPokemonTreeByID()->inorderScan(UpdatePokemonLevels(stoneCode, stoneFactor));
				AVL::Tree<Key, Pokemon>* tLevelTree = it->getData().getlevelPokemonTree();
				updateLevelTree(tLevelTree, stoneCode, stoneFactor);
			}
		}
	};
}
#endif
