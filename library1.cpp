//Here be dragons
#include "library1.h"
#include "Pokedex.h"
using namespace Pokedex;
void* Init(){
	try{
		PokedexDS* DS = new PokedexDS();
		return DS;
	}
	catch (std::bad_alloc& e){
		return 0;
	}
}
StatusType AddTrainer(void *DS, int trainerID){
	if (!DS){
		return (StatusType)INVALID_INPUT;
	}
	try{
		((PokedexDS*)DS)->addTrainer(trainerID);
	}
	catch (BaseException& e){
		return (StatusType)e.getType();
	}
	catch (std::bad_alloc& e){
		return (StatusType)ALLOCATION_ERROR;
	}

}
StatusType CatchPokemon(void *DS, int pokemonID, int trainerID, int level){
	if (!DS){
		return (StatusType)INVALID_INPUT;
	}
	try{
		((PokedexDS*)DS)->catchPokemon(pokemonID, trainerID, level);
	}
	catch (BaseException& e){
		return (StatusType)e.getType();
	}
	catch (std::bad_alloc& e){
		return (StatusType)ALLOCATION_ERROR;
	}
}
StatusType FreePokemon(void *DS, int pokemonID){
	if (!DS){
		return (StatusType)INVALID_INPUT;
	}
	try{
		((PokedexDS*)DS)->freePokemon(pokemonID);
	}
	catch (BaseException& e){
		return (StatusType)e.getType();
	}
	catch (std::bad_alloc& e){
		return (StatusType)ALLOCATION_ERROR;
	}
}
StatusType LevelUp(void *DS, int pokemonID, int levelIncrease){
	if (!DS){
		return (StatusType)INVALID_INPUT;
	}
	try{
		((PokedexDS*)DS)->levelUp(pokemonID, levelIncrease);
	}
	catch (BaseException& e){
		return (StatusType)e.getType();
	}
	catch (std::bad_alloc& e){
		return (StatusType)ALLOCATION_ERROR;
	}
}
StatusType EvolvePokemon(void *DS, int pokemonID, int evolvedID){
	if (!DS){
		return (StatusType)INVALID_INPUT;
	}

	try{
		((PokedexDS*)DS)->evolvePokemon(pokemonID,evolvedID);
	}
	catch (BaseException& e){
		return (StatusType)e.getType();
	}
	catch (std::bad_alloc& e){
		return (StatusType)ALLOCATION_ERROR;
	}
}
StatusType GetTopPokemon(void *DS, int trainerID, int *pokemonID){
	if (!DS){
		return (StatusType)INVALID_INPUT;
	}
	try{
		*pokemonID = ((PokedexDS*)DS)->getTopPokemon(trainerID);
	}
	catch (BaseException& e){
		return (StatusType)e.getType();
	}
	catch (std::bad_alloc& e){
		return (StatusType)ALLOCATION_ERROR;
	}
}
StatusType GetAllPokemonsByLevel(void *DS, int trainerID, int **pokemons, int *numOfPokemon){
	if (!DS){
		return (StatusType)INVALID_INPUT;
	}
	try{
		*pokemons = ((PokedexDS*)DS)->GetAllPokemonsByLevel(trainerID,numOfPokemon);
	}
	catch (BaseException& e){
		return (StatusType)e.getType();
	}
	catch (std::bad_alloc& e){
		return (StatusType)ALLOCATION_ERROR;
	}
}
StatusType UpdateLevels(void *DS, int stoneCode, int stoneFactor){
	if (!DS){
		return (StatusType)INVALID_INPUT;
	}

	try{
		((PokedexDS*)DS)->UpdateLevels(stoneCode, stoneFactor);
	}
	catch (BaseException& e){
		return (StatusType)e.getType();
	}
	catch (std::bad_alloc& e){
		return (StatusType)ALLOCATION_ERROR;
	}
}
void Quit(void** DS){
	if (!DS || !(*DS)){
		return;
	}
	delete *DS;
}