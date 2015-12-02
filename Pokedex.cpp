#include "Pokedex.h"
using namespace Pokedex;
// Auxiliary function for updateLevels. performs the update for a level pokemon
// tree. the function allocated 4 arrays of the size of the tree, one is a copy
// of the tree. two are seperate arrays for pokemon that need updating and
// those who dont. and another is the merge result of the previous two.
// a new empty tree of the same size is allocated, and the merged array copied
// to it. the old tree is then freed.
// <param name="tree"> The level pokemon tree to update. </param>
// <param name="stoneCode"> The mod key for checking if a pokemon needs
// updating. </param>
// <param name="stoneFactor"> is the multiplication factor for the level. </param>
// <return/>
static void updateLevelTree(AVL::Tree<Key, Pokemon>* tree,
	int stoneCode, int stoneFactor);

void PokedexDS::addTrainer(int trainerID){
	if (trainerID <= 0)
		throw InvalidInput();
	Trainer t(trainerID);
	if (trainerList.isIn(t))
		throw AlreadyExists();
	trainerList.insert(t);
}

void PokedexDS::catchPokemon(int pokemonID, int trainerID, int level){
	if (pokemonID <= 0 || trainerID <= 0 || level <= 0)
		throw InvalidInput();
	Trainer* trainer;
	//Check if trainer exists O(k)
	try {
		const IDequals finder(trainerID);
		trainer = &trainerList.find(finder);
	}
	catch (DoesntExist& e) {
		(void)e;
		throw Failure();
	}
	//check if pokemon already caught O(log(n))
	if (pokemonTreeByID.find(pokemonID) != NULL)
		throw Failure();
	Pokemon pokemon(pokemonID, level, trainerID);
	//insert new pokemon to main pokemon id tree O(log(n))
	pokemonTreeByID.insert(pokemonID, pokemon);
	Key k(level, pokemonID);
	//insert new pokemon to main level pokemon tree O(log(n))
	pokemonLevelsTree.insert(k, pokemon);
	//And the same goes to trainer trees 2O(log(n))
	trainer->getlevelPokemonTree()->insert(k, pokemon);
	trainer->getPokemonTreeByID()->insert(pokemonID, pokemon);
	//find maximal level pokemon with min ID O(log(n))
	updateMaxLevel();
	//And the same for trainer O(log(n))
	trainer->updateMaxLevel();
}

void PokedexDS::freePokemon(int pokemonID){
	if (pokemonID <= 0)
		throw InvalidInput();
	//find the pokemon correlating with pokemonID in
	//pokemon id tree O(log(n))
	AVL::Node<int, Pokemon>* p = pokemonTreeByID.find(pokemonID);
	if (!p)
		throw Failure();
	int levelTemp = (p->getData()).getLevel();
	int trainerTemp = (p->getData()).getTrainer();

	// removing from 2 main trees :

	pokemonTreeByID.remove(pokemonID);

	Key temp(levelTemp, pokemonID);
	pokemonLevelsTree.remove(temp);

	//removing from TrainersList
	//find trainer in list O(k)
	Trainer* trainer = &trainerList.find(IDequals(trainerTemp)); // has to be there
	trainer->getPokemonTreeByID()->remove(pokemonID);
	trainer->getlevelPokemonTree()->remove(temp);
	//find max level min id pokemon 2O(log(n))
	updateMaxLevel(trainerTemp);
	//each remove from tree is done in O(log(n))
}

void PokedexDS::levelUp(int pokemonID, int levelIncrease){
	if (pokemonID <= 0 || levelIncrease <= 0)
		throw InvalidInput();
	//find pokemon correlating to pokemonID O(log(n))
	AVL::Node<int, Pokemon>* pokemonNode = pokemonTreeByID.find(pokemonID);
	if (!pokemonNode)
		throw Failure();
	int trainerID = pokemonNode->getData().getTrainer();
	int levelNum = pokemonNode->getData().getLevel();
	//freePokemon is done in O(log(n)+k)
	freePokemon(pokemonID);
	//catchPokemon is done in O(log(n)+k)
	catchPokemon(pokemonID, trainerID, levelNum + levelIncrease);
}

int PokedexDS::getTopPokemon(int trainerID){
	if (trainerID == 0)
		throw InvalidInput();
	else if (trainerID>0){
		Trainer* trainer;
		try{
			//if trainerID>0 find trainer in linked list. O(k)
			trainer = &trainerList.find(IDequals(trainerID));
		}
		catch (const ElementNotFound&){
			throw Failure();
		}
		//getMaxLevel is O(1)
		if (!trainer->getMaxLevel())
			return -1;
		return ((trainer->getMaxLevel())->getData()).getID();
	}
	else{
		if (!maxLevel)
			return -1;
		return (maxLevel->getData()).getID();
	}
}

int* PokedexDS::GetAllPokemonsByLevel(int trainerID, int* numOfPokemon){
	if (trainerID == 0)
		throw InvalidInput();
	AVL::Tree<Key, Pokemon>* levelsTree;
	if (trainerID < 0)
		levelsTree = &pokemonLevelsTree;
	else {
		Trainer* trainer;
		try {
			//if trainerID>0 find trainer. O(k)
			trainer = &trainerList.find(IDequals(trainerID));
		}
		catch (ElementNotFound& e) {
			(void)e;
			throw Failure();
		}
		levelsTree = trainer->getlevelPokemonTree();
	}
	*numOfPokemon = levelsTree->getSize();
	if (*numOfPokemon == 0){
		return NULL;
	}
	int* pokemonArray = (int*)malloc(*numOfPokemon*sizeof(*pokemonArray));
	if (!pokemonArray)
		throw std::bad_alloc();
	int iterator = 0;
	//in order scan is O(n), tree is size n for all pokemon
	//or n-trainer for trainer pokemon.
	levelsTree->inorderScan(PutPokemonInArray(pokemonArray), &iterator);
	return pokemonArray;
}

void PokedexDS::evolvePokemon(int pokemonID, int evolvedID){
	if (pokemonID <= 0 || evolvedID <= 0)
		throw InvalidInput();
	// check pokemonID exists and evolvedID does not. 2O(log(n))
	if (!pokemonTreeByID.find(pokemonID) || pokemonTreeByID.find(evolvedID))
		throw Failure();
	//keep pokemon data from tree. O(log(n))
	Pokemon old = pokemonTreeByID.find(pokemonID)->getData();
	int trainer = old.getTrainer();
	int level = old.getLevel();
	//freePokemon is done in O(log(n)+k)
	freePokemon(pokemonID);
	//catchPokemon is done in O(log(n)+k)
	catchPokemon(evolvedID, trainer, level);
}

void PokedexDS::UpdateLevels(int stoneCode, int stoneFactor){
	if (stoneCode < 1 || stoneFactor < 1){
		throw InvalidInput();
	}
	int garbage = 0;
	//inorder scan is done in O(n)
	pokemonTreeByID.inorderScan(
		UpdatePokemonLevels(stoneCode, stoneFactor), &garbage);
	//updateLevelTree is O(n) for each main tree.
	//for trainer trees it is O(n-trainer) for each trainer, 
	// thus O(sum(n-trainer1,n-trainer2,...) the sum of pokemons of each
	// trainer is n, thus O(n).
	updateLevelTree(&pokemonLevelsTree, stoneCode, stoneFactor);
	for (LinkedList::Node<Trainer>* it = trainerList.begin();
		it != NULL; it = it->getNext()){
		it->getData().getPokemonTreeByID()->inorderScan(
			UpdatePokemonLevels(stoneCode, stoneFactor), &garbage);
		AVL::Tree<Key, Pokemon>* tLevelTree = 
			it->getData().getlevelPokemonTree();
		updateLevelTree(tLevelTree, stoneCode, stoneFactor);
	}
}

static void updateLevelTree(AVL::Tree<Key, Pokemon>* tree,
	int stoneCode, int stoneFactor){
	const int size = tree->getSize();
	AVL::Node<Key, Pokemon>** pokemonArray =
		(AVL::Node<Key, Pokemon>**)malloc(size*sizeof(*pokemonArray));
	if (!pokemonArray) //initial sorted level pokemon array
		throw std::bad_alloc();
	AVL::Node<Key, Pokemon>** changeArray =
		(AVL::Node<Key, Pokemon>**)malloc(size*sizeof(*pokemonArray));
	if (!changeArray){ //sorted array for pokemon with pokemonID%stoneCode==0
		free(pokemonArray);
		throw std::bad_alloc();
	}
	AVL::Node<Key, Pokemon>** stayArray =
		(AVL::Node<Key, Pokemon>**)malloc(size*sizeof(*pokemonArray));
	if (!stayArray){ // sorted array for the rest of the pokemon
		free(pokemonArray);
		free(changeArray);
		throw std::bad_alloc();
	}
	AVL::Node<Key, Pokemon>** newArray =
		(AVL::Node<Key, Pokemon>**)malloc(size*sizeof(*pokemonArray));
	if (!newArray){ // merged arrat of changeArray and stayArray
		free(pokemonArray);
		free(changeArray);
		free(stayArray);
		throw std::bad_alloc();
	}
	// initialize all arrays, done in O(n)
	for (int i = 0; i < size; i++){
		pokemonArray[i] = NULL;
		changeArray[i] = NULL;
		stayArray[i] = NULL;
		newArray[i] = NULL;
	}
	int iterator = 0;
	//copy tree to array with inorder scan O(n)
	tree->inorderScan(PutNodesInArray(pokemonArray), &iterator);
	//put each node in pokemonArray in either stayArray or changeArray O(n)
	for (int i = 0, countChange = 0, countStay = 0; i < size; i++){
		int level = (*pokemonArray[i]).getKey().getLevel();
		int id = (*pokemonArray[i]).getKey().getPokemon();
		Pokemon pokemon = (*pokemonArray[i]).getData();
		if (id % stoneCode == 0){
			pokemon.setLevel(level*stoneFactor);
			changeArray[countChange] =
				new AVL::Node<Key, Pokemon>(Key(level*stoneFactor, id), pokemon);
			++countChange;
		}
		else {
			stayArray[countStay] =
				new AVL::Node<Key, Pokemon>(Key(level, id), pokemon);
			++countStay;
		}
	}
	// merge stayArray and changeArray to newArray O(n)
	for (int i = 0, countChange = 0, countStay = 0; i < size; i++){
		if (!stayArray[countStay]){
			newArray[i] = changeArray[countChange];
			++countChange;
		}
		else if (!changeArray[countChange]){
			newArray[i] = stayArray[countStay];
			++countStay;
		}
		else if ((*stayArray[countStay]).getKey() <
			(*changeArray[countChange]).getKey()){
			newArray[i] = stayArray[countStay];
			++countStay;
		}
		else {
			newArray[i] = changeArray[countChange];
			++countChange;
		}
	}
	AVL::Tree<Key, Pokemon> newTree(size);
	iterator = 0;
	// copy newArray to the empty tree of size 'size' with inorder scan O(n)
	newTree.inorderScan(PutNodesInTree(newArray), &iterator);
	*tree = newTree;
	// free auxilliary arrays O(n)
	for (int i = 0; i < size; i++){
		delete pokemonArray[i];
		pokemonArray[i] = NULL;
		delete newArray[i];
		newArray[i] = NULL;
	}
	free(pokemonArray);
	free(changeArray);
	free(stayArray);
	free(newArray);
}
