/***************************************************************************/
/*                                                                         */
/* 234218 Data DSs 1, Winter 2015-2016                                     */
/*                                                                         */
/* Homework : Wet 1                                                        */
/*                                                                         */
/***************************************************************************/

/***************************************************************************/
/*                                                                         */
/* File Name : main1.cpp                                                   */
/*                                                                         */
/* Holds the "int main()" function and the parser of the shell's           */
/* command line.                                                           */
/***************************************************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library1.h"
#include <iostream>
using namespace std;
#ifdef __cplusplus
extern "C" {
#endif

	/* The command's strings */
	typedef enum {
		NONE_CMD = -2,
		COMMENT_CMD = -1,
		INIT_CMD = 0,
		ADDTRAINER_CMD = 1,
		CatchPokemon_CMD = 2,
		REMOVEPOKEMON_CMD = 3,
		INCREASE_CMD = 4,
		EVOLVE_CMD = 5,
		GETTOPPOKEMON_CMD = 6,
		GETALLPOKEMONS_CMD = 7,
		UPDATE_CMD = 8,
		QUIT_CMD = 9
	} commandType;

	static const int numActions = 10;
	static const char *commandStr[] = { "Init", "AddTrainer", "CatchPokemon",
		"FreePokemon", "LevelUp", "EvolvePokemon",
		"GetTopPokemon", "GetAllPokemonsByLevel", "UpdateLevels", "Quit" };

	static const char* ReturnValToStr(int val) {
		switch (val) {
		case SUCCESS:
			return "SUCCESS";
		case ALLOCATION_ERROR:
			return "ALLOCATION_ERROR";
		case FAILURE:
			return "FAILURE";
		case INVALID_INPUT:
			return "INVALID_INPUT";
		default:
			return "";
		}
	}

	/* we assume maximum string size is not longer than 256  */
#define MAX_STRING_INPUT_SIZE (255)
#define MAX_BUFFER_SIZE       (255)

#define StrCmp(Src1,Src2) ( strncmp((Src1),(Src2),strlen(Src1)) == 0 )

	typedef enum {
		error_free, error
	} errorType;
	static errorType parser(const char* const command);

#define ValidateRead(read_parameters,required_parameters,ErrorString) \
if ( (read_parameters)!=(required_parameters) ) { printf(ErrorString); return error; }

	static bool isInit = false;

	/***************************************************************************/
	/* main                                                                    */
	/***************************************************************************/

	int main(int argc, const char**argv) {
		char buffer[MAX_STRING_INPUT_SIZE];
		FILE* input = stdin;
		if (argc == 2){
			const char* fileName = argv[1];
			input = fopen(fileName,"r");
			if (!input){
				std::cout << "BAD FILE! not cool..." << std::endl;
				exit(1);
			}
		}
		// Reading commands
		while (fgets(buffer, MAX_STRING_INPUT_SIZE, input) != NULL) {
			fflush(stdout);
			if (parser(buffer) == error)
				break;
		};
		return 0;
	}

	/***************************************************************************/
	/* Command Checker                                                         */
	/***************************************************************************/

	static commandType CheckCommand(const char* const command,
		const char** const command_arg) {
		if (command == NULL || strlen(command) == 0 || StrCmp("\n", command))
			return (NONE_CMD);
		if (StrCmp("#", command)) {
			if (strlen(command) > 1)
				printf("%s", command);
			return (COMMENT_CMD);
		};
		for (int index = 0; index < numActions; index++) {
			if (StrCmp(commandStr[index], command)) {
				*command_arg = command + strlen(commandStr[index]) + 1;
				return ((commandType)index);
			};
		};
		return (NONE_CMD);
	}

	/***************************************************************************/
	/* Commands Functions                                                      */
	/***************************************************************************/

	static errorType OnInit(void** DS, const char* const command);
	static errorType OnAddTrainer(void* DS, const char* const command);
	static errorType OnCatchPokemon(void* DS, const char* const command);
	static errorType OnFreePokemon(void* DS, const char* const command);
	static errorType OnLevelUp(void* DS, const char* const command);
	static errorType OnEvolvePokemon(void* DS, const char* const command);
	static errorType OnGetTopPokemon(void* DS, const char* const command);
	static errorType OnGetAllPokemonsByLevel(void* DS, const char* const command);
	static errorType OnUpdateLevels(void* DS, const char* const command);
	static errorType OnQuit(void** DS, const char* const command);

	/***************************************************************************/
	/* Parser                                                                  */
	/***************************************************************************/

	static errorType parser(const char* const command) {
		static void *DS = NULL; /* The general data structure */
		const char* command_args = NULL;
		errorType rtn_val = error;

		commandType command_val = CheckCommand(command, &command_args);

		switch (command_val) {

		case (INIT_CMD) :
			rtn_val = OnInit(&DS, command_args);
			break;
		case (ADDTRAINER_CMD) :
			rtn_val = OnAddTrainer(DS, command_args);
			break;
		case (CatchPokemon_CMD) :
			rtn_val = OnCatchPokemon(DS, command_args);
			break;
		case (REMOVEPOKEMON_CMD) :
			rtn_val = OnFreePokemon(DS, command_args);
			break;
		case (INCREASE_CMD) :
			rtn_val = OnLevelUp(DS, command_args);
			break;
		case (EVOLVE_CMD) :
			rtn_val = OnEvolvePokemon(DS, command_args);
			break;
		case (GETTOPPOKEMON_CMD) :
			rtn_val = OnGetTopPokemon(DS, command_args);
			break;
		case (GETALLPOKEMONS_CMD) :
			rtn_val = OnGetAllPokemonsByLevel(DS, command_args);
			break;
		case (UPDATE_CMD) :
			rtn_val = OnUpdateLevels(DS, command_args);
			break;
		case (QUIT_CMD) :
			rtn_val = OnQuit(&DS, command_args);
			break;

		case (COMMENT_CMD) :
			rtn_val = error_free;
			break;
		case (NONE_CMD) :
			rtn_val = error;
			break;
		default:
			assert(false);
			break;
		};
		return (rtn_val);
	}

	/***************************************************************************/
	/* OnInit                                                                  */
	/***************************************************************************/
	static errorType OnInit(void** DS, const char* const command) {
		if (isInit) {
			printf("Init was already called.\n");
			return (error_free);
		};
		isInit = true;

		*DS = Init();
		if (*DS == NULL) {
			printf("Init failed.\n");
			return error;
		};
		printf("Init done.\n");

		return error_free;
	}

	/***************************************************************************/
	/* OnAddTrainer                                                             */
	/***************************************************************************/
	static errorType OnAddTrainer(void* DS, const char* const command) {
		int trainerID;
		ValidateRead(sscanf(command, "%d", &trainerID), 1, "AddTrainer failed.\n");
		StatusType res = AddTrainer(DS, trainerID);

		if (res != SUCCESS) {
			printf("AddTrainer: %s\n", ReturnValToStr(res));
			return error_free;
		}
		else {
			printf("AddTrainer: %s\n", ReturnValToStr(res));
		}

		return error_free;
	}

	/***************************************************************************/
	/* OnCatchPokemon                                                          */
	/***************************************************************************/
	static errorType OnCatchPokemon(void* DS, const char* const command) {
		int pokemonID;
		int trainerID;
		int level;
		ValidateRead(
			sscanf(command, "%d %d %d", &pokemonID, &trainerID, &level),
			3, "CatchPokemon failed.\n");
		StatusType res = CatchPokemon(DS, pokemonID, trainerID, level);

		if (res != SUCCESS) {
			printf("CatchPokemon: %s\n", ReturnValToStr(res));
			return error_free;
		}

		printf("CatchPokemon: %s\n", ReturnValToStr(res));
		return error_free;
	}

	/***************************************************************************/
	/* OnFreePokemon                                                            */
	/***************************************************************************/
	static errorType OnFreePokemon(void* DS, const char* const command) {
		int pokemonID;
		ValidateRead(sscanf(command, "%d", &pokemonID), 1,
			"FreePokemon failed.\n");
		StatusType res = FreePokemon(DS, pokemonID);

		if (res != SUCCESS) {
			printf("FreePokemon: %s\n", ReturnValToStr(res));
			return error_free;
		}

		printf("FreePokemon: %s\n", ReturnValToStr(res));
		return error_free;
	}

	/***************************************************************************/
	/* OnLevelUp                                                         */
	/***************************************************************************/
	static errorType OnLevelUp(void* DS, const char* const command) {
		int pokemonID;
		int levelIncrease;
		ValidateRead(sscanf(command, "%d %d", &pokemonID, &levelIncrease), 2,
			"LevelUp failed.\n");
		StatusType res = LevelUp(DS, pokemonID, levelIncrease);

		if (res != SUCCESS) {
			printf("LevelUp: %s\n", ReturnValToStr(res));
			return error_free;
		}

		printf("LevelUp: %s\n", ReturnValToStr(res));
		return error_free;
	}

	/***************************************************************************/
	/* OnEvolvePokemon                                                            */
	/***************************************************************************/
	static errorType OnEvolvePokemon(void* DS, const char* const command) {
		int pokemonID;
		int evolvedID;
		ValidateRead(sscanf(command, "%d %d", &pokemonID, &evolvedID), 2,
			"EvolvePokemon failed.\n");
		StatusType res = EvolvePokemon(DS, pokemonID, evolvedID);

		if (res != SUCCESS) {
			printf("EvolvePokemon: %s\n", ReturnValToStr(res));
			return error_free;
		}

		printf("EvolvePokemon: %s\n", ReturnValToStr(res));
		return error_free;
	}

	/***************************************************************************/
	/* OnGetTopPokemon                                                         */
	/***************************************************************************/
	static errorType OnGetTopPokemon(void* DS, const char* const command) {
		int trainerID;
		ValidateRead(sscanf(command, "%d", &trainerID), 1, "GetTopPokemon failed.\n");
		int pokemonID;
		StatusType res = GetTopPokemon(DS, trainerID, &pokemonID);

		if (res != SUCCESS) {
			printf("GetTopPokemon: %s\n", ReturnValToStr(res));
			return error_free;
		}

		cout << "Pokemon with highest level is: " << pokemonID << endl;
		return error_free;
	}

	/***************************************************************************/
	/* OnGetAllGames                                                        */
	/***************************************************************************/

	void PrintAll(int *pokemons, int numOfPokemons) {
		if (numOfPokemons > 0) {
			cout << "Level	||	Pokemon" << endl;
		}

		for (int i = 0; i < numOfPokemons; i++) {
			cout << i + 1 << "\t||\t" << pokemons[i] << endl;
		}
		cout << "and there are no more pokemons!" << endl;

		free(pokemons);
	}

	static errorType OnGetAllPokemonsByLevel(void* DS, const char* const command) {
		int trainerID;
		ValidateRead(sscanf(command, "%d", &trainerID), 1,
			"GetAllPokemonsByLevel failed.\n");
		int* pokemons;
		int numOfPokemons;
		StatusType res = GetAllPokemonsByLevel(DS, trainerID, &pokemons, &numOfPokemons);

		if (res != SUCCESS) {
			printf("GetAllPokemonsByLevel: %s\n", ReturnValToStr(res));
			return error_free;
		}

		PrintAll(pokemons, numOfPokemons);
		return error_free;
	}

	/***************************************************************************/
	/* OnUpdateLevels                                                           */
	/***************************************************************************/
	static errorType OnUpdateLevels(void* DS, const char* const command) {
		int stoneCode;
		int stoneFactor;
		ValidateRead(sscanf(command, "%d %d", &stoneCode, &stoneFactor), 2,
			"UpdateLevels failed.\n");
		StatusType res = UpdateLevels(DS, stoneCode, stoneFactor);

		if (res != SUCCESS) {
			printf("UpdateLevels: %s\n", ReturnValToStr(res));
			return error_free;
		}

		printf("UpdateLevels: %s\n", ReturnValToStr(res));
		return error_free;
	}

	/***************************************************************************/
	/* OnQuit                                                                  */
	/***************************************************************************/
	static errorType OnQuit(void** DS, const char* const command) {
		Quit(DS);
		if (*DS != NULL) {
			printf("Quit failed.\n");
			return error;
		};

		isInit = false;
		printf("Quit done.\n");

		return error_free;
	}

#ifdef __cplusplus
}
#endif