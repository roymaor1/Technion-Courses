#include <iostream>
#include "colosseum.h"
#include <assert.h>


int main(){
	//Tree S
	/*
	Splay<int,int> s1(1,0,NULL,NULL);
	Splay<int,int> s3(3,0,NULL,NULL);
	Splay<int,int> s5(5,0,NULL,NULL);
	Splay<int,int> s2(2,0,&s1,&s3);
	Splay<int,int> s4(4,0,&s2,&s5);
	s4.RR();
	s2.printTree();
	cout << s2.BSTFind(3)->key << endl;
	cout << s2.BSTFind(-2)->key << endl;
	cout << s2.BSTFind(7)->key << endl;
	 */
	/*
	Splay<int,int>::Node n2(2);
	Splay<int,int>::Node n1(1);
	Splay<int,int>::Node n3(3);
	Splay<int,int>::Node n4(4);
	Splay<int,int>::Node n7(7);
	 */
	//Splay<int,int>::Node n10(10);

/*
	Splay::Node<int*,int*> n1(pp1);
	Splay::Node<int*,int*> n2(pp2);
	Splay::Node<int*,int*> n3(pp3);
	Splay::Node<int*,int*> n4(pp4);
	Splay::Node<int*,int*> n5(pp5);
	Splay::Node<int*,int*> n6(pp6);

	Splay<int*> T;
	T.Insert(n6);
	T.Insert(4);
	T.Insert(2);
	T.Insert(7);
	T.Insert(1);
*/

	/*
	T.setRoot(&n2);
	n2.addLeft(&n1);
	n2.addRight(&n4);
	n4.addLeft(&n3);
	n4.addRight(&n7);
	 */
	//T.LL();
	//T.SplayIt(7);
	//n10.addLeft(&n7);
	//T.setRoot(&n10);
	//T.SplayIt(2);
	//T.Insert(6);
	//T.Find(7);
	//T.SplayIt(3);
	//T.Delete(7);
	//cout << "----------" << endl;
	//T.Splay::printTree();
	//cout << "kill the tree" << endl;
	//T.DeleteTree();
	//T.Splay::printTree();
	//T.Insert(x);
	/*
	Colosseum::Trainer::Gladiator g1(NULL,12,96);
	Colosseum::Trainer::Gladiator g2(NULL,10,100);
	if(g1>g2)
		cout << "good" << endl;
	 */
/*
	Splay<Colosseum::Trainer::Gladiator*> T;
	Colosseum::Trainer::Gladiator g1(NULL,3653, 7705);
	Colosseum::Trainer::Gladiator g2(NULL,767, 969);
	Colosseum::Trainer::Gladiator g3(NULL,876, 800);
	Colosseum::Trainer::Gladiator g4(NULL,231, 230);
	Colosseum::Trainer::Gladiator g5(NULL,432, 154);
	Colosseum::Trainer::Gladiator g6(NULL,113, 746);
	Colosseum::Trainer::Gladiator g7(NULL,516, 677);
	T.Insert(&g1);
	T.Insert(&g2);
	T.Insert(&g3);
	T.Insert(&g4);
	T.Insert(&g5);
	T.Insert(&g6);
	T.Insert(&g7);
	T.DeleteTree();
	T.printTree();
*/

/*
	Splay<int*> T;
	int x=2;
	int y=3;
	//Splay<int*>::Node n2(&x);
	T.Insert(&x);
	T.Insert(&y);
	T.printTree();
	//T.Delete(&x);
	T.DeleteTree();
	cout << "------" << endl;
	if(!T.root->key)
		cout << "yes!!" << endl;
	T.printTree();
*/

	//Build Coloseum (World)
	Colosseum* DS = (Colosseum*)Init();

	///Trees for trainers - should be done auto
	//Create trainers


	AddTrainer(DS,84); //yossi
	AddTrainer(DS,66); //rami
	AddTrainer(DS,44); //itzhak
	AddTrainer(DS,60);
	AddTrainer(DS,44);

	//Create Gladiators

	BuyGladiator(DS,999,84,50);



	BuyGladiator(DS,333,84,45);

	BuyGladiator(DS,7,66,99);
	BuyGladiator(DS,58,44,10);
	BuyGladiator(DS,655,8,25);
	BuyGladiator(DS,560,84,45);
	BuyGladiator(DS,999,66,30);
	BuyGladiator(DS,567,60,80);



	FreeGladiator(DS,111111);
	FreeGladiator(DS,-234);

	//FreeGladiator(DS,999);
	//FreeGladiator(DS,333);
	//FreeGladiator(DS,58);

	FreeGladiator(DS,111111);
	//FreeGladiator(DS,7);
	//FreeGladiator(DS,567);
	//FreeGladiator(DS,560);

	FreeGladiator(DS,-4);


	LevelUp(DS,560,60);
	LevelUp(DS,560,-32);
	LevelUp(DS,777,60);

	int t=84;
	int* tmp = new int(0);
	AddTrainer(DS,100);
	FreeGladiator(DS,7);
	GetTopGladiator(DS,t,tmp);
	//cout << "Top Gladiator of " << t << " is: " << *tmp << endl;
	//cout << "----------" << endl;
	BuyGladiator(DS,7,66,99);

	int x=0;
	int tid=84;
	int** gs = (int**)malloc(sizeof(int*));
	int* num = (int*)malloc(sizeof(int));
	GetAllGladiatorsByLevel(DS,tid,gs,num);

	for(int i=0; i<*num; i++){
		//cout << "Number" << i << " is: " << (*gs)[i] << endl;
	}
	//cout << "Num of Gladiators with trainer: " << tid << ": " << *num << endl;
	//cout << "----------" << endl;

	UpgradeGladiator(DS,98,999);
	//GetTopGladiator(DS,t,tmp);
	//cout << "Top Gladiator of " << t << " is: " << *tmp << endl;

	BuyGladiator(DS,11,66,99);


	cout << "---------BEFORE-----------" << endl;
	DS->printColosseum();
	cout << "------------------------" << endl;
	UpdateLevels(DS,3,2);
	cout << "---------AFTER-----------" << endl;
	//DS->printColosseum();
	//cout << "-------------------------" << endl;

	Quit((void**)&DS);



/*
	DoubleLinkedList<Colosseum::Trainer::Gladiator*> dll;
	Colosseum::Trainer::Gladiator g1(NULL,3653, 7705);
	Colosseum::Trainer::Gladiator g2(NULL,767, 969);
	Colosseum::Trainer::Gladiator g3(NULL,876, 800);
	dll.listInsert(&g1);
	dll.listInsert(&g2);
	dll.listInsert(&g3);
	dll.printList();
	cout << dll.getSize() << endl;
	*/

/*
	DoubleLinkedList<int*> d1;
	DoubleLinkedList<int*> d2;
	DoubleLinkedList<int*> merge;
	int x1=13;
	int x2=21;
	int x3=43;
	int x4=65;
	int x5=79;
	int x6=100;
	d1.listInsert(&x1);
	d1.listInsert(&x4);
	d1.listInsert(&x6);
	d2.listInsert(&x2);
	d2.listInsert(&x3);
	d2.listInsert(&x5);

	d1.printList();
	cout << "----------" << endl;
	d2.printList();
	cout << "----------" << endl;
	DoubleLinkedList<int*>::mergeLists(&d1,&d2,&merge);
	merge.printList();
	//cout << dll.getSize() << endl;
*/


/*
	int** arr;
	arr = new int*(0);
	*arr = (int*)malloc(sizeof(int)*4);
	*arr[0]=1;
	*arr[1]=2;
	*arr[2]=3;
	*arr[3]=4;
	cout << arr[0] << endl;
	cout << arr[1] << endl;
	cout << arr[2] << endl;
	cout << arr[3] << endl;
*/


/*
	DoubleLinkedList<int*> dll;
	int x=5;
	int y=7;
	int z=9;
	dll.listInsert(&x);
	dll.listInsert(&y);
	dll.listInsert(&z);
	int k = 6;
	dll.printList();
	DoubleLinkedList<int*>::Node* found = dll.findElement(&k);
	if(!found)
		cout << k << " was not found" << endl;
	else
		cout << *(found->key) << " was found!" << endl;
	//DS->printColosseum();
	//Create List
	//Combine
	//cout << *(DS->TrainersList.head->key) << endl;
	//DS->TrainersList.head->key->trainerGladiators->Splay::printTree();
*/

	//DS->printColosseum();
	return 0;
}