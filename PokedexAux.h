#ifndef _POKEDEX_AUX_H_
#define _POKEDEX_AUX_H_
#include "Pokedex.h"
#include "Trainer.h"
#include "pokemon.h"
#include "AVLTree.h"
#include "newList.h"
#include "Key.h"

// IDequals is a predicate for searching a trainer with ID == newID
// in the Linked list.
// trn is the trainer from list to compare with newID.
class IDequals{
	int ID;
public:
	IDequals(int newID) : ID(newID) {};
	bool operator()(const Trainer& trn) const{
		return trn.getID() == ID;
	}
};
// PutPokemonInArray is the action for in-order scan
// of a level pokemon tree. pokemonArray is an empty
// int array, node is the pokemon to insert and
// iterator is the index in pokemonArray where node will be.
class PutPokemonInArray{
private:
	int* pokemonArray;
public:
	PutPokemonInArray(int* pokemonArray) :pokemonArray(pokemonArray){};
	void operator()(AVL::Node<Key, Pokemon>& node, int* iterator){
		pokemonArray[*iterator] = node.getData().getID();
	}
};
// UpdatePokemonLevels is the action for in-order scan
// of a pokemon ID tree, used for updateLevels. stoneCode is the
// mod key to decide if a pokemon is updated, stoneFactor is the
// multiplication factor of the level. node is the pokemon checked
// and updated if needed.
class UpdatePokemonLevels{
private:
	int stoneCode;
	int stoneFactor;
public:
	UpdatePokemonLevels(int stoneCode, int stoneFactor) :
		stoneCode(stoneCode), stoneFactor(stoneFactor){};
	void operator()(AVL::Node<int, Pokemon>& node, int*){
		if (node.getKey() % stoneCode == 0){
			node.getData().setLevel(node.getData().getLevel() * stoneFactor);
		}
	}
};
// PutNodesInArray is the action for in-order scan
// of a level pokemon tree, used for updateLevels.
// nodesArray is an array of Node* to copy the tree over to.
// node is the pokemon node to be copied, and iterator is
// the index in nodesArray to put it in.
class PutNodesInArray{
private:
	AVL::Node<Key, Pokemon>** nodesArray;
public:
	PutNodesInArray(AVL::Node<Key, Pokemon>** nodesArray) :
		nodesArray(nodesArray){};
	void operator()(AVL::Node<Key, Pokemon>& node, int* iterator){
		nodesArray[*iterator] = new AVL::Node<Key, Pokemon>(node);
	}
};
// PutNodesInArray is the action for in-order scan
// of an empty level pokemon tree, used for updateLevels.
// nodesArray is an array of Node* which will be copied to the tree.
// node is the empty node to assign the 'iterator' indexed node to.
class PutNodesInTree{
private:
	AVL::Node<Key, Pokemon>** nodesArray;
public:
	PutNodesInTree(AVL::Node<Key, Pokemon>** nodesArray) :
		nodesArray(nodesArray){};
	void operator()(AVL::Node<Key, Pokemon>& node, int* iterator){
		node.setKey((*nodesArray[*iterator]).getKey());
		node.setData((*nodesArray[*iterator]).getData());
	}
};
#endif