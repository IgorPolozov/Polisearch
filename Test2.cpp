#ifndef PoliSearcher
#include "PoliSearcher.h"
#endif
#include<iostream>
#include<string>

using namespace PoliSearchering;

struct DataToStore
    {
std::string name;
int height;
int weight;
int mark_math;
int mark_physics;
int mark_chemistry;


DataToStore(){};

DataToStore(
std::string name_,
int height_, 
int weight_,
int mark_math_,
int mark_physics_,
int mark_chemistry_
            )
:
name(name_),
height(height_), 
weight(weight_),
mark_math(mark_math_),
mark_physics(mark_physics_),
mark_chemistry(mark_chemistry_)
    {};
bool operator<(const DataToStore &rhs){
return name.compare(rhs.name)<0;
    }
friend
bool operator<(const DataToStore &lhs, const DataToStore &rhs){
return lhs.name.compare(rhs.name)<0 ;
    }
void init(
std::string name_,
int height_, 
int weight_,
int mark_math_,
int mark_physics_,
int mark_chemistry_
){
name=name_;
height=height_;
weight=weight_;
mark_math=mark_math_;
mark_physics=mark_physics_;
mark_chemistry=mark_chemistry_;
    }

friend
std::ostream& operator<<(std::ostream& os, const DataToStore &rhs){
std::cout<<"\n 1 name "<<rhs.name<<std::endl;
std::cout<<" 2 height "<<rhs.height<<std::endl;
std::cout<<" 3 weight "<<rhs.weight<<std::endl;
std::cout<<" 4 mark_math "<<rhs.mark_math<<std::endl;
std::cout<<" 5 mark_physics "<<rhs.mark_physics<<std::endl;
std::cout<<" 6 mark_chemistry "<<rhs.mark_chemistry<<std::endl;
return os;
    }
};

bool less_by_name(const DataToStore &lhs, const DataToStore &rhs){
return lhs.name.compare(rhs.name)<0;
    }
bool less_by_height(const DataToStore &lhs, const DataToStore &rhs){
return lhs.height<rhs.height;
    }
bool less_by_weight(const DataToStore &lhs, const DataToStore &rhs){
return lhs.weight<rhs.weight;
    }
bool less_by_mark_math(const DataToStore &lhs, const DataToStore &rhs){
return lhs.mark_math<rhs.mark_math;
    }
bool less_by_mark_physics(const DataToStore &lhs, const DataToStore &rhs){
return lhs.mark_physics<rhs.mark_physics;
    }
bool less_by_mark_chemistry(const DataToStore &lhs, const DataToStore &rhs){
return lhs.mark_chemistry<rhs.mark_chemistry;
    }

bool more_by_name(const DataToStore &lhs, const DataToStore &rhs){
return lhs.name.compare(rhs.name)>0;
    }
bool more_by_height(const DataToStore &lhs, const DataToStore &rhs){
return lhs.height>rhs.height;
    }
bool more_by_weight(const DataToStore &lhs, const DataToStore &rhs){
return lhs.weight>rhs.weight;
    }
bool more_by_mark_math(const DataToStore &lhs, const DataToStore &rhs){
return lhs.mark_math>rhs.mark_math;
    }
bool more_by_mark_physics(const DataToStore &lhs, const DataToStore &rhs){
return lhs.mark_physics>rhs.mark_physics;
    }
bool more_by_mark_chemistry(const DataToStore &lhs, const DataToStore &rhs){
return lhs.mark_chemistry>rhs.mark_chemistry;
    }

//predicate stage definition for
//the comparison of lhs и rhs by the all predicates in array array_pred

typedef bool(*Pred) (const DataToStore &, const DataToStore &);

Pred array_preds[]=
    {less_by_name,      less_by_height,         less_by_weight, 
    less_by_mark_math,  less_by_mark_physics,   less_by_mark_chemistry};
const size_t array_preds_size=sizeof(array_preds)/sizeof(array_preds[0]);

template<typename T, size_t N, Pred (&array_pred)[N]>
bool pred_stage_all(const T& lhs, const T& rhs){
int i=0;
bool is_pred;
    for(; i<N; ++i){
is_pred = array_pred[i](lhs, rhs);
        if( !is_pred && !array_pred[i](rhs, lhs) )continue;//равенство
            else return is_pred; //неравенство          
        }
return false;//все равны
    }

std::string  pred_stagede_scription(std::vector<std::string> &vds){
std::string sds;
for(size_t i=0; i<vds.size(); ++i)
sds+=vds[i]+"\n";
return sds;
    }

#include<ctime>
int main(int argc, char *argv[]){
   
srand((unsigned)time(0));
    const int dataToStoresSize=10;
        DataToStore dataToStores[dataToStoresSize];
            std::string names[dataToStoresSize]={"Tolik", "Vasya", "John", "Bob", "Harry",
                "Mary", "Masha", "Cathrine", "Dasha", "Julia"};

for(int i=0; i<dataToStoresSize; ++i){
    dataToStores[i]=
        DataToStore(names[i], 100+rand()%60, 50+rand()%20, 2+rand()%4, 2+rand()%4, 2+rand()%4);
    }

const size_t len=4;

//creating with the beginning Listed sorted by more_by_name of the names comparison
PoliSearcher<DataToStore, len> 
schoolers(
          &dataToStores[0], &dataToStores[0]+dataToStoresSize,
         "more by name",
          more_by_name
          );

schoolers.show_search_list(std::cout, more_by_name,"the field number 1 ");
std::cout<<std::endl;

//adding Listed sorted by growth (predicate <) of the names value
schoolers.add_new_listed(less_by_name,"less_by_name");
std::cout<<"here!"<<std::endl;
schoolers.show(std::cout,  less_by_name,"show - less_by_name " );


schoolers.show_search_list(std::cout, less_by_name,"less_by_name");
std::cout<<std::endl;

//adding Listed sorted by growth (predicate <) of the heighs value
schoolers.add_new_listed(less_by_height,"less_by_height");
schoolers.show_search_list(std::cout, less_by_height,"less_by_height");
std::cout<<std::endl;

//adding an element (it should be added into storage first and then it wil be
//added to all the Listeds present
DataToStore bonifatiy("Bonifatiy", 2017, 2017, 2, 5, 3);
schoolers.add(bonifatiy);
schoolers.show_search_list(std::cout, less_by_name,"less_by_name");
schoolers.show_search_list(std::cout, less_by_height,"less_by_height");

//removing an element 
schoolers.erase_it(bonifatiy,less_by_name);
schoolers.show_search_list(std::cout, less_by_name,"less_by_name");
schoolers.show_search_list(std::cout, less_by_height,"less_by_height");

std::list<DataToStore> lds;

//adding the 5 elements with the name is repeated and random other fields value
for(int i=0; i<5; ++i){
    DataToStore to_add("Masha",100+rand()%60, 50+rand()%20, 2+rand()%4, 2+rand()%4, 2+rand()%4); 
 schoolers.add(to_add); 
 lds.push_front(to_add);
    }
std::cout<<"first ctor"<<std::endl;
//the first constructor demo:
PoliSearcher<DataToStore, len> psfctor(lds, "psfctor with pred = less_by_name", less_by_name, 4);
psfctor.show_search_list (std::cout, less_by_name);
//demo of iteration over the List<predicate> by the Iterator<predicate> objects
PoliSearcher<DataToStore, len>::Iterator<less_by_name> begin_it_less_by_name=schoolers.begin<less_by_name>();
PoliSearcher<DataToStore, len>::Iterator<less_by_name> end_it_less_by_name=schoolers.end<less_by_name>();
PoliSearcher<DataToStore, len>::Iterator<less_by_name> beg_it(begin_it_less_by_name);
std::cout<<"Printing by the Iterators:\n";
if(beg_it.is_valid())
while(beg_it!=end_it_less_by_name){
std::cout<<**beg_it;
beg_it++;
    }

std::cout<<beg_it.size()<<std::endl;

for(int i=0; i<beg_it.size(); ++i){
PoliSearcher<DataToStore, len>::Iterator<less_by_name> it_by_ind=beg_it[i];
if(it_by_ind.is_valid())std::cout<<**beg_it[i]<<std::endl;
else std::cout<<"Problem with Iterator["<<i<<"]"<<std::endl;
    }

schoolers.show_search_list(std::cout, less_by_name,"less_by_name");
schoolers.show_search_list(std::cout, less_by_height,"less_by_height");

PoliSearcher<DataToStore, len>::Iterator<less_by_name> find_first_it =
schoolers.find_search_list_iter_start<less_by_name>(DataToStore ("Masha", 0, 0, 0,0,0));
if(find_first_it.is_valid()){
std::cout<<"\nfind_first_it " << **find_first_it << std::endl;
    }else{
std::cout<<"\nnot found_first_it " << std::endl;
    }

PoliSearcher<DataToStore, len>::Iterator<less_by_name> find_last_it =
schoolers.find_search_list_iter_last<less_by_name>(DataToStore ("Masha", 0, 0,0,0,0));

if( find_first_it.is_valid() && find_last_it.is_valid() ){

std::cout<<"\nfind_first_it and find_last_it " << **find_last_it << std::endl;
    }else{
std::cout<<"\nfind_first_it and/or not found_last_it " << std::endl;
    }


PoliSearcher<DataToStore, len>::Iterator<less_by_name> diap_it =find_first_it;
std::cout<<"\nAll Mashas found:\n";
// pair of these numbers using the case of iteration over all the mages in one of the Listed (less_by_name more precisely)
// allow you to set the iterator find_first_masha to the number given by masha_tobe_found_by_all_predicates
// this value is used further on when searching for the first element with full match of fields
// all available predicates
int masha_num=0, masha_tobe_found_by_all_predicates=2;
PoliSearcher<DataToStore, len>::Iterator<less_by_name> for_by_all_pred_test_it ;
find_last_it++;//point to next of last object found 
for( ;diap_it!=find_last_it; ++diap_it){
std::cout<<**diap_it<<std::endl;
if(++masha_num == masha_tobe_found_by_all_predicates)  for_by_all_pred_test_it= diap_it;
    }
// look for a match of all the predicate in Listed (less_by_name)
PoliSearcher<DataToStore, len>::Iterator<less_by_name>  by_all_pred_test_it =
schoolers.find_search_list_iter_first_by_all_pred_equal<less_by_name>(**for_by_all_pred_test_it);
if(by_all_pred_test_it.is_valid()){
std::cout<<"\nby_all_pred_test_it " << **by_all_pred_test_it << std::endl;
    }else{
std::cout<<"\nnot found by_all_pred_test_it " << std::endl;
    }

// add a Listed ordered in ascending order by comparing less_by_mark_math
schoolers.add_new_listed(less_by_mark_math,"less_by_mark_math");
//schoolers.show_search_list(less_by_mark_math,"less_by_mark_math");
std::cout<<std::endl;

// add a Listed ordered in ascending order by comparing less_by_mark_physics
schoolers.add_new_listed(less_by_mark_physics,"less_by_mark_physics");
//schoolers.show_search_list(less_by_mark_physics,"less_by_mark_physics");
std::cout<<std::endl;

// add a Listed ordered in ascending order by comparing less_by_mark_chemistry
schoolers.add_new_listed(less_by_mark_chemistry,"less_by_mark_chemistry");
//schoolers.show_search_list(less_by_mark_chemistry,"less_by_mark_chemistry");
std::cout<<std::endl;

// search in Listed (less_by_name) for the match of the three predicates specified in the vector

std::vector<PoliSearcher<DataToStore, len>::Pred> vpred(3);

vpred[0]=less_by_mark_math;
vpred[1]=less_by_mark_physics;
vpred[2]=less_by_mark_chemistry;

PoliSearcher<DataToStore, len>::Iterator<less_by_name>  by_tree_pred_test_it=
schoolers.find_search_list_iter_first_by_copl_pred_equal<less_by_name>(vpred,**for_by_all_pred_test_it); 
if(by_tree_pred_test_it.is_valid()){
std::cout<<"\nby_tree_pred_test_it " << **by_tree_pred_test_it << std::endl;
    }else{
std::cout<<"\nnot found by_tree_pred_test_it " << std::endl;
    }
std::cout<<std::endl;

schoolers.add_new_listed(&pred_stage_all<DataToStore, array_preds_size, array_preds>,"less_by_all");
schoolers.show_search_list(std::cout, &pred_stage_all<DataToStore, array_preds_size, array_preds>,"less_by_all");
std::cout<<std::endl;

std::system("pause");
return 0;
    }
