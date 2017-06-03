//
#ifndef PoliSearcher
#include "PoliSearcher.h"
#endif

#ifndef cin
#include<iostream>
#endif
#include<string>
using namespace PoliSearchering;

bool less_int(const int& a, const int& b){
return a<b;
    }
bool more_int(const int& a, const int& b){
return a>b;
    }
	int main()
{
const size_t len=4;
std::cout<<"\n----arr_to_add[]-----\n";
int arr_to_add[17]={9,3,6,1,12,5,2,4,7,10,8,11,15,13,14,16}; 

int arsize=sizeof(arr_to_add)/sizeof(arr_to_add[0]);

PoliSearcher<int, len> listed_lst (&arr_to_add[0], &arr_to_add[17], "less int", less_int);


listed_lst.show(less_int ,"after first Listed creation with pred=less_int ");
std::cout<<std::endl;
listed_lst.add_new_listed(more_int, "field int more");
listed_lst.show(more_int,"after adding Listed with new pred= more_int ");
std::cout<<std::endl;

std::cout<<std::endl;

std::cout<<"\n\n----deletion all one by one-----\n\n";
for(int i=0; i<arsize; ++i)
	{
int val_to_del=arr_to_add[i];
char bf[40];
itoa(val_to_del, bf, 10);
std::string val_to_del_str=bf;

listed_lst.erase_it(val_to_del, less_int);
listed_lst.show(less_int,"less after listed_lst.listed_list.erase(val= "+val_to_del_str+")");
if(!(i%3))system("pause>0");
}

std::cout<<std::endl;
system("pause");
return 0;
}
