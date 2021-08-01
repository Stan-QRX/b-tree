#include<stdio.h>
#include<iostream>
#include<malloc.h> 
#include<stdlib.h>
using namespace std;

#define m 5        //  几阶树     
#define datatype int 
#include"标头.h" 
int main()
{
	btree* root=(btree *)malloc(sizeof(btree));
	for(int i=0;i<=m;i++)
	root->kid[i] = 0;
	root->sumkey = 0;
	root->parent = 0;
	cha(&root, 39);
	cha(&root,22 );
	cha(&root, 97);
	cha(&root, 41);
	cha(&root, 53);
	cha(&root, 13);
	cha(&root, 21);
	cha(&root, 40);
	cha(&root, 30);
	cha(&root, 27);
	cha(&root, 33);
	cha(&root, 36);
	cha(&root, 35);
	cha(&root, 34);
	cha(&root, 24);
	cha(&root, 29);
	cha(&root, 26);
	cha(&root, 17);
	cha(&root, 28);
	cha(&root, 23);
	cha(&root, 31);
	cha(&root, 32);
	bianli(root);
	cout << endl;
	shan(&root, 21);
	bianli(root);
	cout << endl;
	shan(&root, 27);
	bianli(root);
	cout << endl;
	shan(&root, 32);
	bianli(root);
	cout << endl;
	shan(&root, 40);
	bianli(root);
	cout << endl;
	return 1;
}
