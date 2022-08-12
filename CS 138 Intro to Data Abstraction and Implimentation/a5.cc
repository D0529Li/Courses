#include<iostream>
#include<vector>
#include<string>
using namespace std;

//class Animal

class Animal{
public:
	virtual ~Animal ();
	virtual void speak () const = 0;
protected:
	Animal (string name);
	string getName () const;
private:
	string name;
};

Animal::~Animal() {}
Animal::Animal(string name) : name{name} {}

string Animal::getName() const {
	if(this == nullptr){
		return "";
	}
	return name;
}

//class Sheep

class Sheep : public Animal{
public:
	virtual ~Sheep ();
	virtual void speak () const override;
	Sheep (string name);
};

Sheep::~Sheep() {}
Sheep::Sheep(string name) : Animal{name} {}

void Sheep::speak() const {
	cout << "    Sheep " << this->getName() << " says \"baaa\"." << endl;
}

//class Dog

class Dog : public Animal{
public:
	virtual ~Dog ();
	virtual void speak () const override;
	Dog (string name);
};

Dog::~Dog() {}
Dog::Dog(string name) : Animal{name} {}

void Dog::speak() const {
	cout << "    Dog " << this->getName() << " says \"woof\"." << endl; 
}

//class Flock

class Flock{
public:
	Flock(string dogName);
	~Flock();
	void addSheep (string name);
	void soundOff () const;
private:
	Dog *dog;
	vector<Sheep*> sheepList;
};

Flock::Flock(string name) {
	dog = new Dog{name};
}

Flock::~Flock() {
	delete dog;
	dog = nullptr;
	for(auto &s : sheepList){
		delete s;
	}
	while(!sheepList.empty()){
		sheepList.pop_back();
	}
}

void Flock::addSheep(string name) {
	Sheep* newSheep = new Sheep{name};
	sheepList.push_back(newSheep);
}

void Flock::soundOff() const {
	cout << "The flock of " << sheepList.size() << " sheep speaks!" << endl;
	dog->speak();
	for(auto &s : sheepList){
		s->speak();
	}
	cout << endl;
}

//int main(int argc, char* argv[]){
//	Flock *myFlock = new Flock ("Spot");
//	myFlock->soundOff();
//	myFlock->addSheep("Daisy");
//	myFlock->addSheep("Clover");
//	myFlock->addSheep("Estelle");
//	myFlock->soundOff();
//	delete myFlock;
//	myFlock = new Flock ("Rover");
//	myFlock->addSheep("Butch");
//	myFlock->addSheep("Jonno");
//	myFlock->soundOff();
//	delete myFlock;
//	return 0;
//}
