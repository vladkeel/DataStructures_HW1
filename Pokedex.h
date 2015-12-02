#ifndef POKEDEX_H_
#define POKEDEX_H_
#include "PokedexAux.h"
#include "Exceptions.h"
namespace Pokedex{
	// The main data structure, implementing library1.
	class PokedexDS{
	private:
		// List of trainers
		LinkedList::List<Trainer> trainerList;
		// Tree of pokemon, sorted by ID
		AVL::Tree<int,Pokemon> pokemonTreeByID;
		// Tree of pokemon, sorted by level, then ID
		AVL::Tree<Key, Pokemon> pokemonLevelsTree;
		// lowest value node in pokemonLevelsTree
		AVL::Node<Key, Pokemon>* maxLevel;
		// copy c'tor inaccessable
		PokedexDS(const PokedexDS&);
		// assignment operator inaccessable
		PokedexDS& operator=(const PokedexDS&);
		// get lowest value node for maxLevel
		void updateMaxLevel(){
			maxLevel = pokemonLevelsTree.findMin();
		}
		// get lowest value node for maxLevel, and update
		// the trainer the same way
		// <param name="trainerID"> the trainer to update </param>
		// <return/>
		void updateMaxLevel(int trainerID){
			maxLevel = pokemonLevelsTree.findMin();
			(trainerList.find(IDequals(trainerID))).updateMaxLevel();
		}
	public:
		// Default c'tor
		PokedexDS() :maxLevel(0){
			trainerList = LinkedList::List<Trainer>();
			pokemonTreeByID = AVL::Tree<int, Pokemon>();
			pokemonLevelsTree = AVL::Tree<Key, Pokemon>();
		}
		// Default d'tor
		~PokedexDS(){};
		// Add new trainer
		// <param name="trainerID"> ID of trainer to add. </param>
		// <return/>
		void addTrainer(int trainerID);
		// Catch new pokemon and assign to trainer
		// <param name="pokemonID"> ID of pokemon to add. </param>
		// <param name="trainerID"> ID of trainer to assign. </param>
		// <param name="level"> initial level of pokemon. </param>
		// <return/>
		void catchPokemon(int pokemonID, int trainerID, int level);
		// Remove pokemon from ststem
		// <param name="pokemonID"> ID of pokemon to remove. </param>
		// <return/>
		void freePokemon(int pokemonID);
		// Increase level of a pokemon.
		// <param name="pokemonID"> ID of pokemon to affect. </param>
		// <param name="levelIncrease"> value to increase bt. </param>
		// <return/>
		void levelUp(int pokemonID, int levelIncrease);
		// Get pokemon of highest level. if several - get the one with
		// lowest ID.
		// <param name="trainerID"> ID of trainer whose pokemon to get.
		// if id < 0, get highest of all pokemon in system</param>
		// <return> the pokemonID of said pokemon </return>
		int getTopPokemon(int trainerID);
		// Get a sorted array of all of trainers pokemon, sorted by level
		// then ID.
		// <param name="trainerID"> ID of trainer whose pokemons to get.
		// if id < 0, get all pokemon in system</param>
		// <param name="numOfPokemon"> used as return value - the number
		// of pokemon in the array (size of array)</param>
		// <return> the sorted array of pokemon </return>
		int* GetAllPokemonsByLevel(int trainerID, int* numOfPokemon);
		// Change id of pokemon to a new (evolved) id.
		// <param name="pokemonID"> ID of pokemon to affect. </param>
		// <param name="evolvedID"> new ID. </param>
		// <return/>
		void evolvePokemon(int pokemonID, int evolvedID);
		// Increase level of a all pokemon with suiting ID by a given factor.
		// <param name="stoneCode"> mod key to find suiting pokemon. </param>
		// <param name="stoneFactor"> level multiplication factor. </param>
		// <return/>
		void UpdateLevels(int stoneCode, int stoneFactor);
	};
}
#endif
