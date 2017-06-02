# Polisearch
The container for the storing and searching data

Finally, I was going to put in order the result of one of my experiments on the topic of search structures and expanding arrays.
The goal is to obtain a class that combines the advantages of random array access and the self-ordering of the search tree.
This line of thought is not new. The combination of the principles of low-density arrays with pointers and lists, combined with the idea of converting indexes in a logical array to indices in the primary array (hashing), generated an interesting type such as a hash table or a hash array. It successfully combines the economical memory usage inherent in the lists and the access speed inherent in the arrays. Disadvantages are associated with such things as
- the problem of finding a hash function; // a separate, most powerful theme, which has no general theoretical basis
- rehashing with excessive lengthening of lists of collisions;
- natural incompatibility with orderliness;
 The latter property leads to the fact that, despite a sufficiently high access speed by value, it is not possible to make fast sampling the ordered ranges of elements. But you can quickly work with unordered data.
The PoliSearcher class, which  has been written as the result, is a combination of three containers:
STL: std :: vector and two std :: list
wich parameters contain the data in the form of iterators (!), so as not to invent a bike on the pointers.  Please do not kick roughly  due to of my implementation, because I'm not really an exelent encoder, but more of an inventor. I like to think about the concept. And when it is implemented, the implementation itself interests me much less. It's not laziness. Simply, to each his own. However, I will gladly accept any comments on the implementation.
So we have two lists:

-list- named as "storage"  with a type as list <T> storage;

-list named as "search_list"  with a type as list <list <t> :: const_iterator> search_list;

The storage list is created from the received data, added in the order of arrival, and as a result, it is an unordered sequence of objects of type T.

The list search_list is a list that contains iterators for objects in storage. This list is ordered. It is logical that there are as many elements in it as in storage, but they are smaller in case the type T is larger than the list <T> list iterator (this is, almost always).
It needs to be understood that the iterator of the list search_list is an iterator on an iterator, that is, like a pointer to a pointer.
The third container is std :: vector <NodePointer> bin_search_vec. It contains the NodePointer structure. The NodePointer structure is very simple. It contains an iterator in storage (the same type that is contained in search_list) and length int len. In fact, the element of the vector (NodePointer) contains some elements search_list (list <t> :: const_iterator) located at some distance len from the next element of this list to which the next element of the vector indicates. That is, usually between the elements of the list that are duplicated in the elements of the vector there are chains of elements, which are absent in the elements of the vector. Such chains are similar to lists of collisions in hash arrays  (implemented on chains).
The distance len between a pair of list items that are duplicated in the elements of the vector expressed in the number of elements does not exceed the maximum length of the chain that is a constant chain_size, a parameter of the Polosearcher template that will be discussed later. That is, again: len is the actual distance between the begin  (item in the list that is contained in the NodePointer object of the current element of vector ) of of the search_list chain to the its end (next item in the list that is contained in the NodePointer object of the next element of vector). There also (in the NodePointer) two operators are defined - assignments and casts to type T.
In addition to the specified containers, the Polisearcher class contains a constant - the template parameter: size_t chain_size. It sets the maximum value of len for any element of the NodePointer vector, that is, the maximum size of the chain. There is also at least one a pointer to the comparison predicate for type T in the Polisearcher class. This must be a predicate semantically similar to the comparison operator for strict equality or strict inequality.
This predicate, the bin_search_vec vector and the list of search_list with their methods are packed into the Listed class. And Listed instances are placed into map <Pred, Listed> where the predicate of each Listed is the key. This map allows you to work with a certain number of ordered search structures (consisting essentially of iterators) on one object store and has no special interest. The most interesting is the joint work bin_search_vec and search_list, that is the work of the search structure represented as a class Listed.
Vector bin_search_vec is quite interesting (I hope) container. Its elements contain iterators duplicating the elements search_list but with some step not exceeding chain_size. In the demo the chain_size = 4. This is a small step value, but this has been done for simplicity of demonstration. It is clear, that when all chains contain the maximum len = chain_size = 4, the number of elements in the vector is four times less than in the search_list list. It is also logical, that the sequence of vector elements is ordered, since the sequence of elements of the list of search_list is ordered. When searching for elements, we use binary search for a vector (std :: upper_bound, std :: lower_bound), for a time about log2 (N / chain_size) where N is the total number of list items. After finding the desired element of the vector (indicating the leader of the corresponding chain of the list) you need to go through the chain sequentially up to the targetting object, which at worst will give chain_size-1 steps. And on average, the overall complexity: log2 (N / (2 * chain_size)) + (chain_size-1) / 2, which is much less than N / 2 - the average complexity of sequential find in the storage list.
Naturally, when an object inserting, the item is added to storage, and then runs the search for a targetting chain in the list is made as binary search by vector. After the list chain is foundthe  iterator to an element of storage  is placed in the chain by a sequential pass through it up to the targetting position. So the iterator to an element of storage  is placed to the targetting chain of the list search_list. If there is no place in it (len = chain_size = 4), the algorithm finds chain in which that a free place exists (len <chain_size = 4) and the iterators of the vector are moved along the chains, "transporting" one element along the list, so that one free place appears in the targetting chain, because that a chain has been decreased by the 1. In fact, all the iterators in the vector between the target in the chain for inserting and the chain, where there is at least a one free place are shifted on a 1 position.
And once again. If the chain in which the element is to be inserted is filled to its capacity, then begins a search for a not filled chain starting from its begin (the element of the vector that contains the iterator of the chain beginning), in which there is a free space. If such a chain is found, then all the iterators of the vector from the targetting chain (indicating the beginning of the insertion chain that is occupied to the limit) to the found chain (indicating the beginning of the not filled chain) are shifted one from the found position to the target side, thus transferring the free space to the target chain. After the end of such a permutation - the food chain will be extended by one element and the chain to insert will decreased by the 1. That is, while insertion, the time  of the passage for the described shifting of  values along the vector is added to the time of the targetting chain finding on about the N / chain_size  in the worst case. Average is four times smaller. But you do not need to perform such procedures as balancing, as it will be done BST. In fact the described permutation is an analog of the BST balancing.
In the event that all the chains are filled and there are no places to insert the element, then the elements of the vector are doubled. You can apply different options for search vector reallocation control strategy. This is the simplest and most viewable (naive/uderstandable ). After doubling the size of the vector, the chain size will always be twice as small as chain_size.
This is convenient, since the structure is ready to removing and addition equally, and you can only force it to reallocate memory by adding  or removing  as much as it already contains.
While deleting, everything happens similar, but the limit when the vector size is reduced by half, occurs when all the chainses lengthes are equal to one, that is, the size of the vector and its list are the same. This provides stability  against to reallocation. If you sequentaly add - remove a handful of identical or following strictly sequential elements into the binary search tree, you can easily achieve a mode in which the tree will be constantly make balance, which will lead to a sharp decrease in speed. It does not matter whether ALW or R&B tree is meant. Only the size of the elements group and technical details will differ.
For a more complete understanding of the principle of the Polisearch class, let us take a look at how the storage is being filled from scratch.
In the pictures Pic. 1, Pic. 2, Pic. 3, is shown the filling of Polisearcher containers with the sequence of integers: 9,3,6,1,12,5,2,4,7,10,8,11,15,13,14,16
If all containers are initially empty then adding the one element will result in the appearance of this element in each of the containers. The vector will note that its only chain has one element (L = 1). And this an element is a leader for that a chain, of course. Its iterator in a single NodePointer of the vector is a copy of a single element of the search list that represents an iterator on a single element of int 9 in the storage list. Later, the addition of three more elements 3.6 and 1 leads to the fact that in the storage in the order of receipt are the elements 9,3,6,1. In the search list, they are ordered 1,3,6,9 and the only element of the vector NodPointer structure contains an iterator on 1 in the search list and the length of the chain expressed as len is equal 4. Adding element 12 leads to a doubling of the elements of the vector.

////////////////////////////////////////////////////////

To understand how the vector grows, you need to understand what is the field "norma" of the PoliSearcher class.
Norma is a positive integer indicating the number of times (from division to the whole) the starting size of the normalized chain ch_sz = chain_size / norma is less than the maximum size chain_size.
Since the norma field in the search list is set by default and equal to 2, this means that the starting size of the new chains will be twice less than the maximum size (chain_size = 4), that is, the starting len = 2 for all new chains. Their quantity will therefore be twice as much as before, that is, two. Further, the addition of new elements leads to an increase in the total number of elements (storage size) to 7. The size of the first and second chains at this point becomes 4 and 3, respectively. Here there is a situation when the size of the search list has not yet reached a maximum for two chains of four, that is eight, but exactly the chain where the element 4 should be placed is already filled with four elements: 1,2,3,5. Next, a search is performed on the all (both) free chains to find a chain with len less of 4. Such a chain is present - it is only one and has 3 elements. Next, the leader of this chain - the iterator in the vector is shifted to one position in the direction of the chain requesting free space in exchange for an extra element. Now the leader of the chain giving up free space is changed from 6 to 5, and in the chain where the 4 will be placed there are three elements: 1,2,3 and after adding 4 becomes 1,2,3,4. This is shown in Fig. Pic. 2. Here the addition of a new element with value 7 causes a new doubling of the vector, since two chains of four elements are completely filled. A new vector of four elements is ready to accept another eight (or give four and have contraction itself in half), after which a new doubling will happen. The appearance after doubling from 4 to 8 is shown in Fig. Pic. 3.
When you delete elements, they are extracted from the chains and the chain is rebuilt without changing the size of the vector until there is  one element left in each chain. At such a moment, the number of elements of the branch and the list are equal. In this case, further removal without decreasing the size of the vector is impossible. Decrease of the vector at norma = 2 occurs also exactly twice. While the new size of each chain is 2 and it is ready for both growth and further removal without changing the size of the vector until the boundary size of the list is reached relative to the size of the vector-the condition for changing the size of the vector.

Brief class description:

namespace PoliSearchering

Class "PoliSearcher" - the structure of storage and search of elements of a given type, according to predicates comparison strictly more  or  less.

Constructors:

template<typename T, size_t chain_size> class PoliSearcher {

PoliSearcher
	(
		list<T> & storage_, 
		const string &pred_description=less_default_pred_description(),
		Pred predicate_ =less_default, 
		int norma_=2
	);


template<typename FwdIt>
PoliSearcher
	(
		const FwdIt bg_,
		const FwdIt en_, 
		const string &pred_description=less_default_pred_description(),
		Pred predicate_=less_default,
		int norma_=2
	);

}

were:

T - the type of element that is stored in PoliSearcher as an object for searching;

chain_size - the maximum value of the length of the chain of elements between its initial and final elements, - objects to which two adjacent iterators in elements of the search vector indicate;

Storage_ - list of objects for constructing the search structure PoliSearcher;

Pred_description is a string describing the comparison predicate. It is desirable that it contains information: about the field / fields by which the comparison is carried out and how the objects of PoliSearcher are compared.The format of this line is the developer's business. Defaults to the predicate_ predicate by default:
predicate_ the pointer to the default comparison predicate. By default it corresponds to the call of the operator <on type T. When there is no a such an operator overloded, the parametr predicate_ should be passed explisetily 
Parametr predicate_ is the pointer to the default comparison predicate. By default it corresponds to the call of the operator <on type T.
When such an operator does not overloaded, the parametr predicate_ must be explicitly passed.
The type Pred is 
typedef const bool (*Pred)(const &T, const &T);

norma_ is an integer, a positive integer indicating how many times (from division to the whole): the starting size of the normalized chain ch_sz = chain_size / norma is less than the maximum size chain_size;

Bg_, en_, - forward (successive) iterators of the container or array specifying the sequence of items to store.

members:

//////////////////////////// Iterator
Nested class "Iterator":

template<Pred predicate> 
class Iterator; // is a template class whose instances are used to access to the ordered structures of the PoliSearcher class

Constructors:
The default constructor creates an uninitialized iterator:
Iterator(){};

Copy constructor:
Iterator(const Iterator& rhs);

members:

bool is_valid(); - verification of the validity of the received iterator

int size (); - the size of the ordered structure by the given predicate (the sizes of all structures coincide with the size of the storage)
operators:
Note: All access to type T is provided as read only
Iterator operator[](int ind); - access by index (not recommended)
Iterator & operator++();
Iterator operator++(int);
Iterator & operator--();
Iterator operator--(int);
Iterator & operator=(const Iterator rhs);
bool operator == (const Iterator rhs);
bool operator != (const Iterator rhs);
////////////////////////////////Iterator

template
<Pred pred> 
Iterator<pred> begin();- the beginning of the search structure, sorted by the predicate Pred pred

template
<Pred pred> 
Iterator<pred> end(); -the iterator pointing to the address located behind the last valid element of the search structure, which is sorted by the predicate Pred pred

template<Pred predicate> 
Iterator<predicate> find_search_list_iter_start(const T & val ); -a method that returns an iterator to the first element that is equal to an element val in accordance with the predicate predicate.
Equality is determined in accordance with:
(!predicate(elem, val) && !predicate(val, elem)) == (val == elem)

template<Pred predicate> 
Iterator<predicate> find_search_list_iter_last(const T & val ); - a method that returns an iterator to the first element that is behind the last element that is equal to the element val in accordance with the predicate "predicate"

Note: The pair of listed methods that are called with the values val1 and val2 return the iterators of range val1 and val2 (inclusive). The values val1 and val2 must follow one another in accordance with the predicate "predicate". It is logical that if both methods are called with the same value val, then iterators will be obtained for a range containing all elements equal to each other  (val) by the predicate "predicate" 

template<Pred predicate> 
Iterator<predicate> find_search_list_iter_first_by_all_pred_equal(const T & val ); - a method that returns an iterator to the first element that is equal to the element val in accordance with all the PoliSearcher predicates

template<Pred predicate> 
Iterator<predicate> find_search_list_iter_first_by_copl_pred_equal(const std::vector<Pred> &vpred, const T & val );- a method that returns an iterator to the first element that is equal to the element val in accordance with all the predicates sent to
Vpred. It is assumed that all of the transferred ones are present in PoliSearcher. If some predicates are missing in PoliSearcher, they will be ignored. If there is no one in PoliSearcher, will be returned the iterator for the adress located after the end of the Polisearch structure sorted by the predicate "predicate".

Note: If you need to search for ranges for all or part of the predicate, you must create complex predicates and create the appropriate structure , and then use the first 2 search methods.

void add_new_listed(Pred predicate_, std::string pred_description_); - a method that adds a new ordered structure that points to the PoliSearcher item store. Predicate_ and pred_description_ - respectively, a new comparison predicate and its description.
If a predicate is already provided in PoliSearcher, then nothing will be done. Predicates are pointers and means two methods that do, logically, the same work, but defined as independent functions, are considered different. When passing such methods, identical ordered chains will be created, which makes no sense.

void remove_listed(Pred predicate); - a method that removes an ordered structure built on a predicate. If it is not found, nothing will be removed.

void add(const T & val);- a method that adds an element to PoliSearcher (that is, in all its ordered structures)

void erase_it(const T & val,  Pred predicate_ ); - a method that removes an element from PoliSearcher (that is, in all its ordered structures)

void show(Pred predicate_=less_default, std::string str=""); - demonstration and debugging method. Displays structure of the so called Class "Listed" created by the predicate "predicate". The output is search_list, bin_search_vec, and together. String str - header, is outputting at the request of the developer.

void show_search_list(Pred predicate_=less_default, std::string str="" ); - a method of outputting the search structure by its predicate.


SOURCE CODE

//PoliSearcher.h

#include<iostream>
#include <iterator>
#include<list>
#include<map>
#include<vector>
#include<string>
#include<algorithm>
#include <functional>
#define nullptr 0

//using namespace std;
namespace PoliSearchering{
template<typename T, size_t chain_size>
class PoliSearcher
	{
    public: typedef bool ( *Pred )(const T &rhs,  const T& lhs);
    
    private:

/////////////////////////////////////////////////////////////
//forward declaration for typedefs
struct NodePointer;

//the managing vector iterator 
typedef typename std::vector<NodePointer>::iterator IterNodePointer;

//iterator of a list of elements T (Storage) - dereferece gives the r-value of T
typedef typename std::list<T>::const_iterator Iter;

class Listed;

typedef typename std::list<Iter>::const_iterator IterToIter ;

typedef typename std::map<Pred, Listed >::iterator ListListedsIter;

struct NodePointer //the managing vector element
	{
		IterToIter node_it;
			int len;
operator T ()
	{
		return **node_it;
	}

NodePointer & operator = (const NodePointer & rhs)
		{
			if(this != &rhs){
				node_it=rhs.node_it;
					len=rhs.len;
		}
			return *this;
	}
    };
///////////////////////////////////////////////////////////////////////
class Listed// Object that builds the ordered structures/////////
//sorted vector + sorted list wich contain iterators to common storage of PoliSearcher 
//Every such a structure is build using the unical predicate of comparison of elements 
//with type T on strictly more or strictly less 
{
friend class PoliSearcher;
public: 
	Iter storage_begin, storage_end;   

Listed(){}
//////////////////////
////////////////////
bool operator()(const T &rhs,  const T &lhs)
	{
		return (*predicate)(rhs, lhs);
	}
////////////////////////////////
bool operator()(const Iter &rhs,  const Iter &lhs)
	{
		return (*predicate)(*rhs, *lhs);
	}
bool operator()(const IterToIter &rhs,  const IterToIter &lhs)
	{
		return (*predicate)(**rhs, **lhs);
	}

bool operator()(const Iter &rhs,  const T &lhs)
	{
		return (*predicate)(*rhs, lhs);
	}

bool operator()(const T &rhs,  const Iter  &lhs)//на cppreference ошибка или у майкрософт?
	{
		return (*predicate)(rhs, *lhs);
	}	

//method finds the first iterator in sorted list (search_list) pointing to val
IterToIter find_search_list_iter_start(const T & val){

IterToIter found=search_list.end(), last=search_list.end();

if(search_list.empty())
				{
					return found;
				}
                    IterNodePointer chain_leader_it = find_chain_leader(val);
						if(chain_leader_it == bin_search_vec.end())
							{
								return found;
							}
IterNodePointer chain_next_leader_it = chain_leader_it+1;
if(chain_next_leader_it==bin_search_vec.end())
		{
			last=search_list.end();
				last--;
		}
			else
				{
					last=chain_next_leader_it->node_it;
				}
					found=lower_bound(chain_leader_it->node_it, last, val, *this);
						if(found != search_list.end() && 
                            compire_pred_eq(**found , val)
                            )
							{
								return found;
							}
								return search_list.end();
		}
//method finds the first iterator in not sorted list (common storage) pointing to val
IterToIter find_search_list_iter_last(const T & val){
    IterToIter it_start=find_search_list_iter_start(val);
        if(it_start == search_list.end())return it_start;
            IterNodePointer chain_leader_it = find_chain_leader(val);

IterNodePointer chain_leader_last = 
    upper_bound(chain_leader_it, bin_search_vec.end(), val, *this);

IterToIter it_last=search_list.end();
    if(chain_leader_last != bin_search_vec.end()){
           it_last=chain_leader_last->node_it;
    }

IterToIter it=upper_bound(it_start, it_last, val, *this);
    return it;
    }


Iter find_storage_iter(const T & val)
	{
		Iter found=storage_end;

			if(search_list.empty())
				{
					return found;
				}
					IterNodePointer chain_leader_it = find_chain_leader(val);
						if(chain_leader_it == bin_search_vec.end())
							{
								return found;
							}
								IterNodePointer chain_next_leader_it = chain_leader_it+1;
IterToIter deleted, last;
	if(chain_next_leader_it==bin_search_vec.end())
		{
			last=search_list.end();
				last--;
		}
			else
				{
					last=chain_next_leader_it->node_it;
				}
					deleted=lower_bound(chain_leader_it->node_it, last, val, *this);
						if(
                            deleted != search_list.end() && //**deleted == val
                            compire_pred_eq(**deleted , val)
                            )
							{
								found=*deleted;
							}
								return found;
	}


	IterToIter search_list_end(){
return search_list.end();
		}


bool contains(const T & val){
Iter storage_it=find_storage_iter(val);
if(storage_it == storage_end) return false;
return true;
}

std::string Pred_description(){
return pred_description;
    }

private: 

Pred predicate;
    std::string pred_description;
	    std::list<Iter> search_list;
		    std::vector<NodePointer> bin_search_vec;
                
            
//norma is positive integer shows how many times the starting size of the normalized chain
//is less then the maximal size of chain (chain_size)
//so it is the result of division by the modulus (%)
            int norma;

void init(const std::list<T> &storage_, Pred predicate_, int norma_=2){
storage_begin=storage_.begin();
storage_end=storage_.end();
predicate=predicate_;
norma=norma_;
if(storage_begin != storage_end)
			{
				Iter it=storage_begin;
					for(; it != storage_end; it++)
						{
                       		add(it);	
						}
			}

    }
void init(const std::list<T> &storage_, const std::string& pred_description_, Pred predicate_, int norma_=2){
pred_description=pred_description_;
init(storage_, predicate_, norma_);    
    }

//the element deletion from the search_list and bin_search_vec
//the deletion from the storage is not performed but the iterator to the element
//in the storage is returned.
//using it Polisercher deletes this an alement by its own
//from other Listeds and from the storage after all
void erase(const T & val)
	{
		Iter storage_it_to_delete=storage_end;
			storage_it_to_delete= find_storage_iter(val);
            if(storage_it_to_delete !=storage_end){
					erase(storage_it_to_delete);
                }else{
cout<< "storage_it_to_delete ==storage_end at void erase(const T & val)" << endl;
                }
						//return storage_it_to_delete;
	}
    
    Listed & operator = (const Listed & rhs){
        if(this!=&rhs){
storage_begin=rhs.storage_begin;
 storage_end = rhs.storage_end; 
  predicate = rhs.predicate;
    norma = rhs.norma;
      search_list=rhs.search_list;
        bin_search_vec=rhs.bin_search_vec;
            }
        return *this;
        }

bool compire_pred_eq(const T &rhs,  const T& lhs)// это == по < или >
	{
		return !predicate(rhs, lhs) && !predicate(lhs, rhs);
	}

void decrease_halfing_search_vec()
	{
		size_t bin_search_vec_size=bin_search_vec.size();
			int is_odd_old_size=bin_search_vec_size%2;
				size_t new_vec_size=bin_search_vec_size/2;
					new_vec_size+=is_odd_old_size;
						std::vector<NodePointer> new_vec(new_vec_size);
							size_t j=0;//new_vec индекс
for(size_t i=0; i < bin_search_vec_size ; i++)
	{
		if(!(i%2))
			{
				new_vec[j].node_it=bin_search_vec[i].node_it;
					new_vec[j].len=2;
						j++;
			}
else
	if(is_odd_old_size && i==bin_search_vec_size-1 )
		{
			new_vec[j].node_it=bin_search_vec[i].node_it;
				new_vec[j].len=1;
					j++;
		}
	}
bin_search_vec=new_vec;
}


void increase_doubling_search_vec(int norma=2)
{
if(having_a_free_cell())//функция корректно работает только если все цепи заполнены
//то есть нет свободного места, поэтому тут от греха - проверка и выход если есть свободная
//ячейка
	{
		std::cout<<"increase_doubling_search_vec() may not run with having_a_free_cell()==true"<<std::endl;
			std::cin.get();
				return;
	}
		size_t start_size=chain_size/norma;
			size_t new_vec_size=bin_search_vec.size() * norma;
				std::vector<NodePointer> new_vec(new_vec_size);
					size_t j;//new_vec индекс
for(size_t i=0; i<bin_search_vec.size(); ++i)
	{
		j=2*i;
			new_vec[j].node_it=bin_search_vec[i].node_it;
				new_vec[j+1].node_it=bin_search_vec[i].node_it;
					advance(new_vec[j+1].node_it, start_size);
						new_vec[j].len=start_size;
							new_vec[j+1].len=start_size;
	}
		bin_search_vec=new_vec;
}

IterNodePointer find_chain_leader(const T & val)
	{
		IterNodePointer found = 
			lower_bound(bin_search_vec.begin(), bin_search_vec.end(), val, *this);
				if(found > bin_search_vec.begin())
					{
						found-=1;
					}
						return found;
	}		


bool is_prev_bin_search_vec(const IterNodePointer &chain_leader_it, const T & val) 
	{
		return 
			(
				chain_leader_it==bin_search_vec.begin()
				&& 
				predicate(val, **(bin_search_vec.begin()->node_it))
				|| 
				compire_pred_eq(val, **(bin_search_vec.begin()->node_it))
			);
}

IterNodePointer find_donor_chain_right(IterNodePointer chain_leader)
	{
		if(chain_leader != bin_search_vec.end())
			{

				while(chain_leader!=bin_search_vec.end())
					{
						chain_leader++;
							if(
								chain_leader == bin_search_vec.end() 
								|| 
								chain_leader->len < chain_size
								)
									{
										break;
									}
					}
			}
		return chain_leader;
	}

IterNodePointer find_acceptor_chain_right(IterNodePointer chain_leader)
	{
		if(chain_leader != bin_search_vec.end())
			{
				while(chain_leader!=bin_search_vec.end())
					{
						chain_leader++;
							if(
								chain_leader == bin_search_vec.end() 
								|| 
								chain_leader->len > 1 
								)
									{
										break;
									}
					}
			}
		return chain_leader;
	}
	
IterNodePointer find_donor_chain_left(IterNodePointer chain_leader)
	{
		if(chain_leader != bin_search_vec.begin())
			{
				while(true)
					{
						chain_leader--;
							if(chain_leader->len < chain_size)
								{
									break;
								}
									if(chain_leader==bin_search_vec.begin())
										{
											chain_leader = bin_search_vec.end();
												break;
										}
					}
			}
				else 
					{
						chain_leader = bin_search_vec.end();
					}
						return chain_leader;
	}

IterNodePointer find_acceptor_chain_left(IterNodePointer chain_leader)
	{
		if(chain_leader != bin_search_vec.begin())
			{
				while(true)
					{
						chain_leader--;
							if(chain_leader->len > 1)break; 
								if(chain_leader==bin_search_vec.begin())
									{
										chain_leader = bin_search_vec.end();
											break;
									}
					}
			}
				else chain_leader = bin_search_vec.end();
					return chain_leader;
	}

IterNodePointer 
give_donors_cell_from_right(IterNodePointer chain_leader, IterNodePointer donor_chain_leader)
	{
		donor_chain_leader->len++;
			while(donor_chain_leader>chain_leader)
				{
					donor_chain_leader->node_it--;
						donor_chain_leader--;
				}
					chain_leader->len--;
						return chain_leader;
	}	

IterNodePointer 
give_donors_cell_from_left(IterNodePointer chain_leader, IterNodePointer donor_chain_leader)
	{
		donor_chain_leader->len++;
			donor_chain_leader++;
				while(donor_chain_leader<chain_leader+1)
					{
						donor_chain_leader->node_it++;
							donor_chain_leader++;
					}
						chain_leader->len--;
							return chain_leader;
	}


IterToIter 
find_ins_pos_to_search_list(
				   const T & val, 
				   IterToIter left_it,
				   IterToIter right_it
                   ){
return upper_bound(left_it, right_it, val, *this);
    }

IterToIter 
add_To_search_list(
				   const T & val, 
				   IterToIter left_it,
				   IterToIter right_it
				   )
	{
IterToIter ins_pos=find_ins_pos_to_search_list(val, left_it, right_it);
 
	Iter ins_iter=storage.insert(storage.end(), val);	
 
		IterToIter ins_iter_to_iter=search_list.insert(ins_pos, ins_iter);	
			return ins_pos;
	}

IterToIter 
add_To_search_list(
				   Iter  ins_iter,//iterator in storage, 
				   IterToIter left_it,
				   IterToIter right_it
				   )
	{   
		
			const T & val=*ins_iter;
                IterToIter ins_pos=find_ins_pos_to_search_list(val, left_it, right_it);

								search_list.insert(ins_pos, ins_iter);	
									return ins_pos;
	}

bool having_a_free_cell()
	{
		size_t common_len=0;
			size_t max_len=chain_size * bin_search_vec.size();
				IterNodePointer it=bin_search_vec.begin();
					for(; it !=bin_search_vec.end(); ++it)
						{
							common_len+=it->len;
						}
		if(common_len < max_len) return true;
			if(common_len == max_len) return false;

std::cout
<<"common_len >  max_len in bool having_a_free_cell() and common_len= " 
<< common_len << "with max_len= " << max_len << std::endl;

	std::system("pause");
		throw("having_a_free_cell common_len > chain_size * bin_search_vec.size()");
}

void add(const Iter & it_storage)
	{   
		if(bin_search_vec.empty() || search_list.empty())
			{
				if(!bin_search_vec.empty())
					{
						bin_search_vec.clear();
					}
						if(!search_list.empty())
							{
								search_list.clear();
							}
								search_list.push_back(it_storage);
									NodePointer np;
										np.node_it=search_list.begin();
											np.len=1;
												bin_search_vec.push_back(np);
													return;
			}
				if(having_a_free_cell() == false)
					{
						increase_doubling_search_vec();//удвоили размер вектора уменьшив вдвое размер цепочки
							add(it_storage);//и вошли снова
								return;
					}
IterToIter ins_iter_to_iter, ins_pos, end_pos;
	T val = *it_storage;
		IterNodePointer chain_leader_it = find_chain_leader(val);
			if(is_prev_bin_search_vec(chain_leader_it, val))
				{
					ins_pos=chain_leader_it->node_it;
						ins_iter_to_iter=search_list.insert(ins_pos, it_storage);
							bin_search_vec.begin()->node_it=ins_iter_to_iter;
								bin_search_vec.begin()->len++;
				} 
					else
						{
							if(chain_leader_it+1==bin_search_vec.end())
								{
									end_pos=search_list.end();
								}
									else
										{
											if(chain_leader_it != bin_search_vec.end())
												{
													end_pos=(chain_leader_it+1)->node_it;
												}
													else 
														{
															end_pos=search_list.end();
														}
										}

			ins_iter_to_iter=add_To_search_list(it_storage, chain_leader_it->node_it, end_pos);
				chain_leader_it->len++;
						}
							if(ins_iter_to_iter==search_list.begin())
								{
									chain_leader_it->node_it=ins_iter_to_iter;
								}
									if(chain_leader_it->len>chain_size)
										{
											IterNodePointer donor_it=find_donor_chain_right(chain_leader_it);
												if(donor_it!=bin_search_vec.end())
													{
														give_donors_cell_from_right(chain_leader_it, donor_it);
													}
					else 
						{
							donor_it=find_donor_chain_left(chain_leader_it);
								give_donors_cell_from_left(chain_leader_it, donor_it);
						}
		}
	}

class SearchListedStructure
    {
friend class Listed;
IterNodePointer chain_leader_it;
IterToIter search_list_it;

SearchListedStructure(const  IterNodePointer & chain_leader_it_, const  IterToIter & search_list_it_)
:chain_leader_it(chain_leader_it_), search_list_it(search_list_it_)
{}

SearchListedStructure(const SearchListedStructure & rhs)
:chain_leader_it(rhs.chain_leader_it), search_list_it(rhs.search_list_it)
{}
SearchListedStructure & operator = (const SearchListedStructure & rhs)
{
if(this != rhs)
	{
		chain_leader_it = rhs.chain_leader_it;
		search_list_it = rhs.search_list_it;
	}
return *this;
}
bool operator == (const SearchListedStructure & rhs)
	{
		return chain_leader_it == rhs.chain_leader_it && search_list_it == rhs.search_list_it ;	
	}
};//end SearchListedStructure

Iter erase(Iter storage_it_to_delete)
	{
		if(bin_search_vec.empty() || search_list.empty())
			{
				if(!bin_search_vec.empty())bin_search_vec.clear();
					if(!search_list.empty())search_list.clear();
						return storage_it_to_delete;
			}
SearchListedStructure searchListedStructure=find_SearchListedStructure(storage_it_to_delete);
	IterNodePointer chain_leader_it = searchListedStructure.chain_leader_it;
		IterToIter deleted=searchListedStructure.search_list_it;

if(chain_leader_it->len==1)
	{
		if(search_list.size()==1 )
			{
				if(*deleted == storage_it_to_delete){
					search_list.clear();
						bin_search_vec.clear();
			}

			return storage_it_to_delete;
	}

		if(chain_leader_it != bin_search_vec.end()-1)
			{
				IterNodePointer acceptor_it=find_acceptor_chain_right(chain_leader_it);
					if(acceptor_it != bin_search_vec.end())
						{
							give_donors_cell_from_left(acceptor_it, chain_leader_it);
						}
			}
	if(chain_leader_it->len==1 && chain_leader_it != bin_search_vec.begin())//можно поискать акцептора слева
		{
			IterNodePointer acceptor_it=find_acceptor_chain_left(chain_leader_it);
				if(acceptor_it != bin_search_vec.end())
					{
						give_donors_cell_from_right(acceptor_it, chain_leader_it);
					}
		}
	if(chain_leader_it->len==1)
			{
			decrease_halfing_search_vec();
				storage_it_to_delete=erase(storage_it_to_delete);
					return storage_it_to_delete;
			}
	}
	if(chain_leader_it->node_it == deleted)
		{
			deleted=chain_leader_it->node_it;
				storage_it_to_delete=*deleted;
					chain_leader_it->node_it++;
						chain_leader_it->len--;
							search_list.erase(deleted); 
		}
			else
				{
					search_list.erase(deleted); 
						chain_leader_it->len--;
				}
					return storage_it_to_delete;
}

SearchListedStructure find_SearchListedStructure(Iter todel_it)
	{
		IterToIter erased=search_list.end();
			if(search_list.empty())
				{
		SearchListedStructure searchListedStructure(bin_search_vec.end(), search_list.end());
			return searchListedStructure;
				}
					const T & val = *todel_it;
IterNodePointer chain_leader_begin =lower_bound(bin_search_vec.begin(),bin_search_vec.end(), val, *this);
	if( chain_leader_begin  != bin_search_vec.begin() )
		{
			chain_leader_begin=chain_leader_begin-1;
		}
			IterNodePointer chain_leader_end=
				upper_bound(bin_search_vec.begin(),bin_search_vec.end(), val, *this);

					IterNodePointer chain_leader_current=chain_leader_begin;
						IterToIter iterToIter;

for	(
	;
chain_leader_current != bin_search_vec.end() && 
(
	(
		chain_leader_end!=bin_search_vec.end() &&
		chain_leader_current!=chain_leader_end+1
	) || 
	chain_leader_current!=chain_leader_end
)	; 
chain_leader_current++
	)//for )))
		{
			iterToIter=chain_leader_current->node_it;
				if(chain_leader_current == bin_search_vec.end())
					{
						goto find_and_go;
					}
						for(int i=0; i<chain_leader_current->len; ++i)
							{
								if(*iterToIter == todel_it)
									{
										goto find_and_go;
									}
										iterToIter++;
							}
		}

			find_and_go :;

				SearchListedStructure searchListedStructure(chain_leader_current, iterToIter);
					return searchListedStructure;
	}

void show_search_list(const std::string name=0)
	{
		if(name.length()!=0)
			{
				std::cout<<name<<std::endl;
			}
				for(IterToIter it=search_list.begin(); it != search_list.end(); it++)
					{
						std::cout<<(*(*it))<<' ';
					}
						std::cout<<std::endl;
	}

void show_bin_search_vec(const std::string name=0)
	{
	if(name.length()!=0)
		{
			std::cout<<name<<std::endl;
		}
   std::cout<<"bin_search_vec.size() = "<< bin_search_vec.size()<<std::endl;
    std::cout<<std::endl;
			for(IterNodePointer it=bin_search_vec.begin(); it != bin_search_vec.end(); it++)
				{
                std::cout<<(*(*it->node_it))<<' '<<"the len of the chain is: "<<it->len<<std::endl;
				}
					std::cout<<std::endl;
	}

void show_bin_search_vec(IterNodePointer b, IterNodePointer e, const std::string name=0)
	{
		if(name.length()!=0)
			{
				std::cout<<name<<std::endl;
			}
				for(IterNodePointer it=b; it != e; it++)
					{                    
						std::cout<<(*(*it->node_it))<<' '<<it->len<<std::endl;
					}
						std::cout<<std::endl;
	}


void show_chain(size_t ind, const std::string str="" )
	{
		std::cout<<str<<std::endl;
			if(ind>bin_search_vec.size()-1)
				{
					std::cout<<"show_chain returns with out of index= "
						<<ind<<"when last is "
						<<bin_search_vec.size()-1
						<<std::endl;
	}
		IterToIter lid=bin_search_vec[ind].node_it;
			while(lid != search_list.end())
				{
					if(ind<bin_search_vec.size()-1 && bin_search_vec[ind+1].node_it==lid )
						{
							break;
						}
							std::cout<<**lid++<<' ';
				}
					std::cout<<"\n__________________________\n";//отладочное))
	}

void show_chains(const std::string str="")
	{
		std::cout<<std::endl<<str<<std::endl;
			std::cout<<"\n__________________________\n";
				for(size_t i=0; i<bin_search_vec.size(); ++i)
					{
						std::cout<<"# "<<i<<" #";show_chain(i);
					}
	}	

void show(const std::string str="")
	{
		show_search_list("show_search_list "+str);       
			show_bin_search_vec("show_bin_search_vec "+str);           
				show_chains("show_chains "+str);
	}
	

};//end class Listed


//find predicate by the its description (not used yet) 
Pred select_pred_by_description(std::string description){
    if(!list_Listeds.empty()){
ListListedsIter listed_list_it=list_Listeds.begin(), listed_list_end=list_Listeds.end();
for(; listed_list_it!=listed_list_end; ++listed_list_it){ 
if(listed_list_it->pred_description == description) return listed_list_it->predicate;    
    }
        }
return nullptr;
    }

void erase_it(const T & val,  ListListedsIter listed_list_it)
	{
    if(listed_list_it == list_Listeds.end())return;
		Iter storage_it_to_delete=storage.end();
        			storage_it_to_delete= listed_list_it->second.find_storage_iter(val);
                if(storage_it_to_delete !=storage.end()){
                    listed_list_it= list_Listeds.begin();
                        for(;listed_list_it != list_Listeds.end(); listed_list_it++)
					        listed_list_it->second.erase(storage_it_to_delete);
                }
            storage.erase(storage_it_to_delete);//удаление из хранилища           
	}

IterToIter find_search_list_iter_start(Pred predicate_, const T & val ){
    ListListedsIter find_listed=list_Listeds.find(predicate_);
        IterToIter fnd_it = find_listed->second.find_search_list_iter_start(val);
            return fnd_it;
    }

IterToIter find_search_list_iter_last(Pred predicate_, const T & val ){
    ListListedsIter find_listed=list_Listeds.find(predicate_);
        IterToIter fnd_it = find_listed->second.find_search_list_iter_last(val);
            return fnd_it;
    }

//finds IterToIter in Listed(predicate_)  with condition of equality to val by all predicates
IterToIter find_search_list_iter_first_by_all_pred_equal(Pred predicate_, const T & val){
    ListListedsIter find_listed= list_Listeds.find(predicate_);
        IterToIter fnd_it = find_listed->second.search_list.end();
            IterToIter fnd_it_start = find_listed->second.find_search_list_iter_start(val);
                IterToIter fnd_it_last = find_listed->second.find_search_list_iter_last(val);
bool is_eq_by_curent_pred=false;
if(fnd_it_start != find_listed->second.search_list.end() ){//did not find within borders
    for( ; fnd_it_start!=fnd_it_last; ++fnd_it_start ){

//the cycle of checking the value from the interval using the all known (presents) predicates:
ListListedsIter it= list_Listeds.begin();
    is_eq_by_curent_pred=true;
    for( ; it!=list_Listeds.end(); ++it ){
if(find_listed==it) continue;
            if(it->second.compire_pred_eq(val, **fnd_it_start) == false){
                is_eq_by_curent_pred=false;
                     break;
                }
        }

if(is_eq_by_curent_pred) break;
        }   
    }
else is_eq_by_curent_pred=false;
    if(!is_eq_by_curent_pred)fnd_it=find_listed->second.search_list.end();
        else fnd_it=fnd_it_start;
return fnd_it;
    }

//finds IterToIter in Listed(predicate_)  with condition of equality to val by some of presents in Polisercher predicates
//that predicates are passed by the vpred
IterToIter find_search_list_iter_first_by_copl_pred_equal(Pred predicate_, const std::vector<Pred>& vpred, const T & val){

    ListListedsIter find_listed= list_Listeds.find(predicate_);
        IterToIter fnd_it = find_listed->second.search_list.end();
            IterToIter fnd_it_start = find_listed->second.find_search_list_iter_start(val);
                IterToIter fnd_it_last = find_listed->second.find_search_list_iter_last(val);

bool is_eq_by_curent_pred=false;
if(fnd_it_start != find_listed->second.search_list.end() /*|| fnd_it_last == fnd_it*/){//не нашла в границах
    for( ; fnd_it_start!=fnd_it_last; ++fnd_it_start ){

//the cycle of checking the value from the interval using the some known (presents in vpred) predicates:

ListListedsIter it= list_Listeds.begin();
    is_eq_by_curent_pred=true;
    for( ; it!=list_Listeds.end(); ++it ){
std::vector<Pred>::const_iterator &vit=find(vpred.begin(), vpred.end(), it->second.predicate);
if(vit == vpred.end() || find_listed==it ) continue;
            if(it->second.compire_pred_eq(val, **fnd_it_start) == false){
                is_eq_by_curent_pred=false;
                     break;
                }
        }

if(is_eq_by_curent_pred) break;
        }   
    }
else is_eq_by_curent_pred=false;
if(!is_eq_by_curent_pred)fnd_it=find_listed->second.search_list.end();
    else fnd_it=fnd_it_start;
        return fnd_it;
    }

bool is_valid_IterToIter(const IterToIter &it, Pred predicate){
    ListListedsIter find_listed=list_Listeds.find(predicate);
        return it != find_listed->second.search_list.end();
    }

//the method initialzating predicate by the default
//it requires the operator < to be overloded for the type T
static bool less_default(const T &rhs,  const T& lhs){return rhs<lhs;}

static const std::string less_default_pred_description(){
    return " less default";
    }

void show(ListListedsIter listed_list_it, std::string str=""){
        listed_list_it->second.show(str;)
        }

public:	

PoliSearcher(std::list<T> & storage_, const std::string &pred_description=less_default_pred_description(),  Pred predicate_ =less_default, int norma_=2)
:storage(storage_)	
{
list_Listeds.insert(std::make_pair<Pred, Listed>(predicate_, Listed()));
list_Listeds.begin()->second.init(storage, pred_description, predicate_, norma_);
	}

template<typename FwdIt>
PoliSearcher(const FwdIt bg_, const FwdIt en_, const std::string &pred_description=less_default_pred_description(), Pred predicate_=less_default, int norma_=2)
:storage(bg_, en_)
{
list_Listeds.insert(std::make_pair<Pred, Listed>(predicate_, Listed()));
list_Listeds.begin()->second.init(storage, pred_description, predicate_, norma_);
	}


template<Pred predicate> 
class Iterator{
friend class PoliSearcher;
public:
Iterator(){}

Iterator(const Iterator& rhs)
:iter_to_iter(rhs.iter_to_iter), poliSearcher(rhs.poliSearcher)
{}

IterToIter operator->(){return iter_to_iter;}

Iter operator*(){return *iter_to_iter;}

int size(){
ListListedsIter find_listed= poliSearcher->list_Listeds.find(predicate);
int counter=0;
if(!find_listed->second.bin_search_vec.empty()){
IterNodePointer bsv_it = find_listed->second.bin_search_vec.begin(); 
while(bsv_it != find_listed->second.bin_search_vec.end()){
counter+=bsv_it->len;
++bsv_it;
    }
--bsv_it;
counter-=bsv_it->len;
IterToIter sl_it = bsv_it->node_it;
while(sl_it != find_listed->second.search_list.end()){
++counter;
++sl_it;
    }
    }
return counter;
    }

Iterator operator[](int ind){
ListListedsIter find_listed= poliSearcher->list_Listeds.find(predicate);
    Iterator it_by_ind=poliSearcher->end<predicate>();
        if(ind>-1 && ind<size() && find_listed != poliSearcher->list_Listeds.end()){
            if(!find_listed->second.bin_search_vec.empty()){
                int after_ind(0), ind_lider(0);
                    IterNodePointer bsv_it = find_listed->second.bin_search_vec.begin(); 
while(bsv_it != find_listed->second.bin_search_vec.end() && after_ind<ind){
    ind_lider=after_ind;
        after_ind+=bsv_it->len;
            ++bsv_it;
    }
if(bsv_it != find_listed->second.bin_search_vec.begin()){
--bsv_it;
 after_ind-=bsv_it->len;
    }
    IterToIter sl_it = bsv_it->node_it;   

while(sl_it != find_listed->second.search_list.end() && ind_lider!= ind){
    ++ind_lider;
        ++sl_it;
    }
it_by_ind=Iterator(sl_it, poliSearcher) ;
                }
            }
return it_by_ind;
    }

Iterator& operator++(){
    ++iter_to_iter;
        return *this;
    }

Iterator operator++(int){
Iterator tmp = Iterator(*this);
    ++iter_to_iter;
        return tmp;
    }

Iterator& operator--(){
    --iter_to_iter;
        return *this;
    }

Iterator operator--(int){
    Iterator tmp = Iterator(*this);
        --iter_to_iter;
            return tmp;
    }

Iterator &operator=(const Iterator rhs){
    if(this != &rhs){
        iter_to_iter = rhs.iter_to_iter;   
            poliSearcher = rhs.poliSearcher;       
        }
return *this;
    }


bool operator == (const Iterator rhs){
return (iter_to_iter == rhs.iter_to_iter) && (poliSearcher == rhs.poliSearcher);
    }


bool operator != (const Iterator rhs){
return !(*this == rhs) ;
    }

bool is_valid(){
return poliSearcher->is_valid_IterToIter(iter_to_iter, predicate);
    }

private:

bool operator == (const IterToIter rhs){
return iter_to_iter == rhs;
    }

bool operator != (const IterToIter rhs){
return !(*this == rhs) ;
    }


Iterator(const IterToIter& rhs, PoliSearcher *poliSearcher_)
:iter_to_iter(rhs), poliSearcher(poliSearcher_)
{}
IterToIter iter_to_iter;
PoliSearcher *poliSearcher;
    };

template
<Pred pred> 
Iterator<pred>
begin(){
    ListListedsIter find_listed= list_Listeds.find(pred);
        IterToIter bgn=find_listed->second.search_list.begin();
            Iterator<pred> bgh_ret(bgn, this);
return bgh_ret;
    }

template
<Pred pred> 
Iterator<pred>
end(){
    ListListedsIter find_listed= list_Listeds.find(pred);
        IterToIter en=find_listed->second.search_list.end();
            Iterator<pred> en_ret(en, this);
return en_ret;
    }

template<Pred predicate> 
Iterator<predicate> find_search_list_iter_start(const T & val ){
    IterToIter it = find_search_list_iter_start(predicate, val );
        return Iterator<predicate>( it , this);
    }


template<Pred predicate> 
Iterator<predicate> find_search_list_iter_last(const T & val ){
    IterToIter it = find_search_list_iter_last(predicate, val );
        return Iterator<predicate>( it , this);
    }

template<Pred predicate> 
Iterator<predicate> find_search_list_iter_first_by_all_pred_equal(const T & val ){
    IterToIter it = find_search_list_iter_first_by_all_pred_equal(predicate, val );
        return Iterator<predicate>( it , this);
    }

template<Pred predicate> 
Iterator<predicate> find_search_list_iter_first_by_copl_pred_equal(const std::vector<Pred> &vpred, const T & val ){
    IterToIter it = find_search_list_iter_first_by_copl_pred_equal(predicate, vpred, val );
        return Iterator<predicate>( it , this);
    }

//method creating the new Listed and adding it to the container Listeds
void add_new_listed(Pred predicate_, std::string pred_description_){
    ListListedsIter find_listed=list_Listeds.find(predicate_);
        if(find_listed == list_Listeds.end()){
            int norma=2;
if(!list_Listeds.empty())norma = list_Listeds.begin()->second.norma;
    Listed to_add;
        list_Listeds[predicate_]=to_add;
            list_Listeds[predicate_].init(storage, pred_description_, predicate_, norma);
     }
}
//doing opposite to add_new_listed
void remove_listed(Pred predicate){
    ListListedsIter find_listed=list_Listeds.find(predicate_);
        if(find_listed != list_Listeds.end()){
            list_Listeds.erase(find_listed);
    }
    }

//method adding an element to storage and its iterator to
//to all Listeds present in list_Listeds
void add(const T & val){//ListListedsIter listed_list_it

    Iter ins_iter=storage.insert(storage.end(), val);
        
ListListedsIter listed_list_it=list_Listeds.begin(), listed_list_end=list_Listeds.end();
for(; listed_list_it!=listed_list_end; ++listed_list_it){ 
listed_list_it->second.add(ins_iter);    
    }
}

void erase_it(const T & val,  Pred predicate_ ){
        ListListedsIter listed_list_it =  list_Listeds.find(predicate_); 
            if(listed_list_it != list_Listeds.end())
                erase_it(val, listed_list_it);
    }


void show( Pred predicate_=less_default, std::string str="" ){
    ListListedsIter listed_list_it =  list_Listeds.find(predicate_); 
    if(listed_list_it != list_Listeds.end()){
        std::cout<<"pred_description= "<<listed_list_it->second.pred_description<<std::endl;
        if(predicate_==less_default)str=PoliSearcher::less_default_pred_description();
            listed_list_it->second.show(str);
        }
else{std::cout<<"listed_list not found in void show(ListListedsIter listed_list_it, string str=\"\")";}
    }

void show_search_list(Pred predicate_=less_default, std::string str="" ){
ListListedsIter listed_list_it =  list_Listeds.find(predicate_); 
    if(listed_list_it != list_Listeds.end()){
        std::cout<<"pred_description= "<<listed_list_it->second.pred_description<<std::endl;
        if(predicate_==less_default)str=PoliSearcher::less_default_pred_description();
            listed_list_it->second.show_search_list("The search_list is sorted by "+str); 
        }
else{std::cout<<"listed_list not found in void show(ListListedsIter listed_list_it, string str=\"\")";}
    }

private:
std::list<T> storage;    
     std::map<Pred, Listed> list_Listeds;

};	//end class Polysearcher

bool more(const int & rhs, const int & lhs)
	{
		return rhs>lhs;
	}
bool les(const int & rhs, const int & lhs)
	{
		return rhs<lhs;
	}
template<typename T>
void show_list(std::list<T> & search_list, const std::string name=0, int siz=0)
{
	if(name.length()!=0)
		{
			std::cout<<name<<std::endl;
		}
			int cnt(0);
				for
					(
						typename std::list<T>::iterator it=search_list.begin();
						it != search_list.end(); it++
					)
						{
							cnt++;
								std::cout<<*it<<' ';
						}
							std::cout<<std::endl;
								if(siz)std::cout<<"Size of interval = "<<cnt<<std::endl;
}

template<typename T>
void show_list(
typename std::list<T>::iterator b,
typename std::list<T>::iterator e,
const std::string name=0,
int siz=0
)
	{
		if(name.length()!=0)
			{
				std::cout<<name<<std::endl;
			}
				int cnt(0);
					for(typename std::list<T>::iterator it=b; it != e; it++)
						{
							cnt++;
								std::cout<<*it<<' ';
						}
							std::cout<<std::endl;
							if(siz)
								{
									std::cout<<"Size of interval = "<<cnt<<endl;
								}
	}
}// end of namespace PoliSearchering

///////////////////////
//Test1.h
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

///////////////////////////
//Test2.cpp
#ifndef PoliSearcher
#include "PoliSearcher.h"
#endif

#ifndef cin
#include<iostream>
#endif

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

schoolers.show_search_list(more_by_name,"the field number 1 ");
std::cout<<std::endl;

//adding Listed sorted by growth (predicate <) of the names value
schoolers.add_new_listed(less_by_name,"less_by_name");
std::cout<<"here!"<<std::endl;
schoolers.show( less_by_name,"show - less_by_name " );


schoolers.show_search_list(less_by_name,"less_by_name");
std::cout<<std::endl;

//adding Listed sorted by growth (predicate <) of the heighs value
schoolers.add_new_listed(less_by_height,"less_by_height");
schoolers.show_search_list(less_by_height,"less_by_height");
std::cout<<std::endl;

//adding an element (it should be added into storage first and then it wil be
//added to all the Listeds present
DataToStore bonifatiy("Bonifatiy", 2017, 2017, 2, 5, 3);
schoolers.add(bonifatiy);
schoolers.show_search_list(less_by_name,"less_by_name");
schoolers.show_search_list(less_by_height,"less_by_height");

//removing an element 
schoolers.erase_it(bonifatiy,less_by_name);
schoolers.show_search_list(less_by_name,"less_by_name");
schoolers.show_search_list(less_by_height,"less_by_height");

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
psfctor.show_search_list (less_by_name);
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

schoolers.show_search_list(less_by_name,"less_by_name");
schoolers.show_search_list(less_by_height,"less_by_height");

PoliSearcher<DataToStore, len>::Iterator<less_by_name> find_first_it =
schoolers.find_search_list_iter_start<less_by_name>(DataToStore ("Masha", 0, 0, 0,0,0));
if(find_first_it.is_valid()){
std::cout<<"\nfind_first_it " << **find_first_it << std::endl;
    }else{
std::cout<<"\nnot found_first_it " << std::endl;
    }

PoliSearcher<DataToStore, len>::Iterator<less_by_name> find_last_it =
schoolers.find_search_list_iter_last<less_by_name>(DataToStore ("Masha", 0, 0,0,0,0));
if(find_first_it.is_valid()){
std::cout<<"\nfind_last_it " << **find_last_it << std::endl;
    }else{
std::cout<<"\nnot found_last_it " << std::endl;
    }
PoliSearcher<DataToStore, len>::Iterator<less_by_name> diap_it =find_first_it;
std::cout<<"\nAll Mashas found:\n";
// pair of these numbers using the case of iteration over all the mages in one of the Listed (less_by_name more precisely)
// allow you to set the iterator find_first_masha to the number given by masha_tobe_found_by_all_predicates
// this value is used further on when searching for the first element with full match of fields
// all available predicates
int masha_num=0, masha_tobe_found_by_all_predicates=2;
PoliSearcher<DataToStore, len>::Iterator<less_by_name> for_by_all_pred_test_it ;
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
schoolers.show_search_list(&pred_stage_all<DataToStore, array_preds_size, array_preds>,"less_by_all");
std::cout<<std::endl;

std::system("pause");
return 0;

    }



