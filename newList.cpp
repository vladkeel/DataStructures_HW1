
#include <iostream>
#include <math.h>
#include "newList.h"
#include "macros.h"

using namespace std;
using namespace LinkedList;
class IDequals
{
	int id;
public:
	IDequals(int i) : id(i) {}
	bool operator()(const int& x) const
	{
		return x == id;
	}
};

class doubleEquals
{
	double id;
public:
	doubleEquals(double i) : id(i) {}
	bool operator()(const double& x) const
	{
		return fabs(x-id)<0.001;
	}
};

static bool createTest(){
	bool result = true;
		List<int> intList;
		for(int i = 0; i < 101; i++){
			intList.insert(i);
		}

		for(int i = 0; i < 101; i++){
			TEST_EQUALS(result ,intList.find(IDequals(i)), i );
			//*(intList.find(i))
		}

	return result;
}

static bool removeTest(){
	bool result = true;
	List<int> intList;
	for(int i = 0; i < 101; i++){
		intList.insert(i);
	}
	for(int i = 0; i < 101; i++){
		intList.remove(i);
	}
	int f = 0;
	for(int i = 0 ; i < 50; i++){
		try{
			intList.remove(i);
		} catch(const IsEmpty& ){
			f++;
		}
	}
	TEST_EQUALS(result, f, 50);
	for(int i = 40; i < 240 ; i++){
		intList.insert(i);
	}
	f = 0;
	for(int i = 240; i < 440 ; i++){
			try{
				intList.remove(i);
			} catch (const ElementNotFound&){
				f++;
			}
		}
	TEST_EQUALS(result, f, 200);
	return result;
}

static bool findTest(){
	bool result = true;
	List<double> dblList;
	for(double i = 0.0; i<5.0 ; i+=1){
		dblList.insert(i);
	}
	assert(result);
	List<int> intList;
	for(int i = 0; i<5 ; i++){
		intList.insert(i);
		assert(result);
	}
	for(int i = 0; i<5 ; i++){
		TEST_EQUALS(result, intList.find(IDequals(i)), i);
		assert(result);
	}
	for(int i = 4; i>0 ; i--){
		intList.remove(i);
	}
	intList.remove(0);
	try{
		intList.remove(0);
	} catch(const IsEmpty&){
		cout<<"intList is empty!"<<endl;
	}

	return result;
}

static bool insertTest(){
	bool result = true;
	List<int> intList;
	for(int i = 0; i < 101; i++){
		intList.insert(i);
	}
	for(int i = 0; i < 101; i++){
		TEST_TRUE(result, intList.isIn(i));
	}
	return result;
}

static bool assignmentTest(){
	bool result = true;
	List<int> intList1;
	for(int i = 0; i < 101; i++){
		intList1.insert(i);
	}
	for(int i = 0; i < 101; i++){
		TEST_EQUALS(result, intList1.find(IDequals(i)), i);
	}
	List<int> intList2;
	for(int i = 101; i < 202; i++){
		intList2.insert(i);
	}
	intList1 = intList2;
	for(int i = 101; i < 202; i++){
		TEST_EQUALS(result, intList1.find(IDequals(i)), i);
	}
	int f = 0;
	for(int i = 0; i < 101; i++){
		try{
			intList1.find(IDequals(i));
		} catch(const DoesntExist&){
			f++;
		}
	}
	TEST_EQUALS(result, f, 101);
	intList2 = intList2;
	for(int i = 101; i < 202; i++){
		TEST_EQUALS(result, intList2.find(IDequals(i)), i);
	}
	List<int> intList3;
	List<int> intList4;
	intList3 = intList4;
	intList2 = intList3;
	try{
			intList2.remove(0);
	} catch(const IsEmpty& ){
		cout<<"intList2 is empty!!!"<<endl;
	}

	return result;
}

int main() {
	RUN_TEST(createTest);
	RUN_TEST(insertTest);
	RUN_TEST(assignmentTest);
	RUN_TEST(removeTest);
	RUN_TEST(findTest);
	return 0;
}


