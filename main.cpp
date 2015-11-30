#include "Pokedex.h"
#include "macros.h"

#include "iostream"
#include "ostream"

using namespace std;


void printAction(int k){
	std::cout << k << std::endl;
}

/*
static void test1(){
	AVL::Tree<int> tree = AVL::Tree<int>(1, 1);
	tree.insert(3, 3);
	tree.insert(4, 4);
	tree.insert(5, 5);
	tree.insert(6, 6);
	tree.insert(2, 2);
	tree.remove(5);
	std::cout << "print inorder scan" << std::endl;
	tree.inorderScan(printAction);
	std::cout << "==================" << std::endl;
	tree.reverseInorderScan(printAction);
}
static void test2(){
	AVL::tree<int> tree = AVL::tree<int>();
	tree.root = new AVL::node<int>(20, 20);
	tree.root->right = new AVL::node<int>(10, 10);
	tree.root->right->right = new AVL::node<int>(5, 5);
	tree.root->right->right->right = new AVL::node<int>(4, 4);
	tree.root->right->right->left = new AVL::node<int>(6, 6);
	tree.root->right->left = new AVL::node<int>(15, 15);
	tree.root->right->left->right = new AVL::node<int>(13, 13);
	tree.root->right->left->left = new AVL::node<int>(17, 17);
	tree.root->right->left->right->right = new AVL::node<int>(12, 12);
	tree.root->right->left->right->left = new AVL::node<int>(14, 14);
	tree.root->right->left->left->right = new AVL::node<int>(16, 16);
	tree.root->right->left->left->left = new AVL::node<int>(18, 18);
	tree.root->left = new AVL::node<int>(30, 30);
	tree.root->left->right = new AVL::node<int>(25, 25);
	tree.root->left->left = new AVL::node<int>(35, 35);
	tree.root->left->right->right = new AVL::node<int>(23, 23);
	tree.root->left->right->left = new AVL::node<int>(27, 27);
	tree.root->left->left = new AVL::node<int>(35, 35);
	tree.root->left->left = new AVL::node<int>(35, 35);
}
void test3(){
	AVL::Tree<int> tree = AVL::Tree<int>(5,5);
	tree.insert(2, 2);
	tree.insert(6, 6);
	tree.insert(1, 1);
 	tree.insert(4, 4);
	//Magic happen here:
	tree.insert(3, 3);
	
	tree.remove(6);
	//Now magic!
	tree.remove(5);
	int i;
	std::cin >> i;
}

static bool levelTest(){
	bool result = true;
}
*/
using namespace Pokedex;
int main(){

	bool result = true;
	PokedexDS system;
	int i = 1;
	for(; i<20; i++){
		system.addTrainer(i);
	}
	int ex=0;
	for(int i = 1; i<20; i++){
		try{
		system.addTrainer(i);
		} catch(const AlreadyExists&){
			ex++;
		}
	}
	TEST_EQUALS(result, ex, 19);
	assert(result);
cout<<ex<<endl;

}
