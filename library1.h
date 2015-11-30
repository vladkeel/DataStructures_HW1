/****************************************************************************/
/*                                                                          */
/* This file contains the interface functions                               */
/* you should use for the wet ex 1                                          */
/*                                                                          */
/****************************************************************************/

/****************************************************************************/
/*                                                                          */
/* File Name : library1.h                                                   */
/*                                                                          */
/****************************************************************************/

#ifndef _234218_WET1_
#define _234218_WET1_

#ifdef __cplusplus
extern "C" {
#endif

	/* Return Values
	* ----------------------------------- */
	typedef enum {
		SUCCESS = 0,
		FAILURE = -1,
		ALLOCATION_ERROR = -2,
		INVALID_INPUT = -3
	} StatusType;

	/* Required Interface for the Data Structure
	* -----------------------------------------*/

	/* Description:   Initiates the data structure.
	* Input:         DS - A pointer to the data structure.
	* Output:        None.
	* Return Values: A pointer to a new instance of the data structure - as a void* pointer.
	*/
	void* Init();

	/* Description:   Adds a new trainer.
	* Input:         DS - A pointer to the data structure.
	*                trainerID - The ID of the trainer to add.
	* Output:        None.
	* Return Values: ALLOCATION_ERROR - In case of an allocation error.
	*                INVALID_INPUT - If DS==NULL or if trainerID <= 0.
	*                FAILURE - If trainerID is already in the DS.
	*                SUCCESS - Otherwise.
	*/
	StatusType AddTrainer(void *DS, int trainerID);

	/* Description:   Adds a new pokemon to the system.
	* Input:         DS - A pointer to the data structure.
	*                pokemonID - ID of the pokemon to add.
	*                trainerID - The ID of the pokemon's trainer
	*                level - The pokemon's level
	* Output:        None.
	* Return Values: ALLOCATION_ERROR - In case of an allocation error.
	*                INVALID_INPUT - If DS==NULL, or if pokemonID <=0, or if trainerID <=0, or if level <= 0
	*                FAILURE - If pokemonID is already in the DS, or trainerID isn't in the DS.
	*                SUCCESS - Otherwise.
	*/
	StatusType CatchPokemon(void *DS, int pokemonID, int trainerID, int level);

	/* Description:   Removes an existing pokemon.
	* Input:         DS - A pointer to the data structure.
	*                pokemonID - The ID of the pokemon to remove.
	* Output:        None.
	* Return Values: ALLOCATION_ERROR - In case of an allocation error.
	*                INVALID_INPUT - If DS==NULL or if pokemonID <= 0.
	*                FAILURE - If pokemonID isn't in the DS.
	*                SUCCESS - Otherwise.
	*/
	StatusType FreePokemon(void *DS, int pokemonID);

	/* Description:   Increases the level of a pokemon.
	* Input:         DS - A pointer to the data structure.
	*                pokemonID - The ID of the pokemon.
	*		            levelIncrease - The increase in level.
	* Output:        None.
	* Return Values: ALLOCATION_ERROR - In case of an allocation error.
	*                INVALID_INPUT - If DS==NULL, or if pokemonID<=0, or if levelIncrease<=0
	*                FAILURE - If pokemonID isn't in the DS.
	*                SUCCESS - Otherwise.
	*/
	StatusType LevelUp(void *DS, int pokemonID, int levelIncrease);

	/* Description:   Evolves a pokemon, updating his ID, while maintaining his level.
	* Input:         DS - A pointer to the data structure.
	*                pokemonID - The original ID of the pokemon.
	*                evolvedID - The new ID of the pokemon.
	* Output:        None.
	* Return Values: ALLOCATION_ERROR - In case of an allocation error.
	*                INVALID_INPUT - If DS==NULL, or if pokemonID<=0, or if evolvedID<=0.
	*                FAILURE - If pokemonID isn't in the DS, or evolvedID is in the DS.
	*                SUCCESS - Otherwise.
	*/
	StatusType EvolvePokemon(void *DS, int pokemonID, int evolvedID);

	/* Description:   Returns the pokemon with the highest level from trainerID
	* 			If trainerID < 0, returns the top pokemon in the entire DS.
	* Input:         DS - A pointer to the data structure.
	*                trainerID - The trainer that we'd like to get the data for.
	* Output:        pokemonID - A pointer to a variable that should be updated to the ID of the top pokemon.
	* Return Values: ALLOCATION_ERROR - In case of an allocation error.
	*                INVALID_INPUT - If DS==NULL, or if pokemonID == NULL, or if trainerID == 0.
	*                SUCCESS - Otherwise.
	*/
	StatusType GetTopPokemon(void *DS, int trainerID, int *pokemonID);

	/* Description:   Returns all the pokemons from trainerID sorted by their level.
	*           			If trainerID < 0, returns all the pokemons in the entire DS sorted by their level.
	* Input:         DS - A pointer to the data structure.
	*                trainerID - The trainer that we'd like to get the data for.
	* Output:        pokemons - A pointer to a to an array that you should update with the pokemons' IDs sorted by their level,
	*			          in case two pokemons have same level they should be sorted by their ID.
	*                numOfPokemon - A pointer to a variable that should be updated to the number of pokemons.
	* Return Values: ALLOCATION_ERROR - In case of an allocation error.
	*                INVALID_INPUT - If any of the arguments is NULL or if trainerID == 0.
	*                SUCCESS - Otherwise.
	*/
	StatusType GetAllPokemonsByLevel(void *DS, int trainerID, int **pokemons, int *numOfPokemon);

	/* Description:   Updates the level of the pokemons where pokemonID % stoneCode == 0.
	* 			          For each matching pokemon, multiplies its level by stoneFactor.
	* Input:         DS - A pointer to the data structure.
	*                stoneCode - The basis that the stone works on
	*          		  stoneFactor - The multiply factor of the level
	* Output:        None.
	* Return Values: ALLOCATION_ERROR - In case of an allocation error.
	*                INVALID_INPUT - If DS==NULL or if stoneCode < 1 or if stoneFactor <1
	*                SUCCESS - Otherwise.
	*/
	StatusType UpdateLevels(void *DS, int stoneCode, int stoneFactor);


	/* Description:   Quits and deletes the database.
	*                DS should be set to NULL.
	* Input:         DS - A pointer to the data structure.
	* Output:        None.
	* Return Values: None.
	*/
	void Quit(void** DS);

#ifdef __cplusplus
}
#endif

#endif    /*_234218_WET1_ */