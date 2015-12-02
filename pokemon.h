#ifndef POKEMON_H_
#define POKEMON_H_
// pokemon class
class Pokemon{
private:
	// pokemon ID
	int pokemonID;
	// pokemon level
	int level;
	// belonging to trainer of trainerID
	int trainerID;
public:
	// Default c'tor
	Pokemon():pokemonID(0),level(0),trainerID(0){};
	// data c'tor
	Pokemon(int id, int lvl, int trn) : pokemonID(id), level(lvl), trainerID(trn){};
	// copy c'tor
	Pokemon(const Pokemon& pkmn) : pokemonID(pkmn.pokemonID), level(pkmn.level), trainerID(pkmn.trainerID){};
	// assignment operator
	Pokemon& operator=(const Pokemon& pkmn){
		if (this != &pkmn){
			pokemonID = pkmn.pokemonID;
			level = pkmn.level;
			trainerID = pkmn.trainerID;
		}
		return *this;
	}
	// default d'tor
	~Pokemon(){};
	// get pokemon id
	inline int getID()const{
		return pokemonID;
	}
	// comparison (equals) operator - according to id
	inline bool operator==(const Pokemon& other)const{
		return (pokemonID == other.pokemonID);
	}

	// get pokemon level
	inline int getLevel()const{
		return level;
	}
	
	// get trainer id
	inline int getTrainer()const{
		return trainerID;
	}

	// set new level
	inline void setLevel(int lvl){
		level = lvl;
	}
};

#endif