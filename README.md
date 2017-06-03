# Polisearch
The container for the storing and searching data

Finally, I was going to put in order the result of one of my experiments on the topic of search structures and expanding arrays.
The goal is to obtain a class that combines the advantages of random array access and the self-ordering of the search tree.
This line of thought is not new. The combination of the principles of low-density arrays with pointers and lists, combined with the idea of converting indexes in a logical array to indices in the primary array (hashing), generated an interesting type such as a hash table or a hash array. It successfully combines the economical memory usage inherent in the lists and the access speed inherent in the arrays. Disadvantages are associated with such things as
- the problem of finding a hash function; // a separate, most powerful theme, which has no general theoretical basis
- rehashing with excessive lengthening of lists of collisions;
- the logarithmic search time on the size of the primary array, which can be much smaller than the total amount of data;
- natural incompatibility with orderliness;
 The latter property leads to the fact that, despite a sufficiently high access speed by value, it is not possible to make fast sampling the ordered ranges of elements. But you can quickly work with unordered data.
 
 The proposed solution does not require hashing, as such. It is resistant to the order of data arrival. Allows you to quickly and simultaneously maintain many ordered structures on the same data storage.
 
The PoliSearcher class, which  has been written as the result, is a combination of three containers:
STL: std :: vector and two std :: list
wich parameters contain the data in the form of iterators (!), so as not to invent a bike on the pointers.  Please do not kick roughly  due to of my implementation, because I'm not really an exelent coder, but more of an inventor. I like to think about the concept. And when it is implemented, the implementation itself interests me much less. It's not laziness. Simply, to each his own. However, I will gladly accept any comments on the implementation.
So we have two lists:

-list- named as "storage"  with a type as list< T > storage;

-list named as "search_list"  with a type as list < list < T > :: const_iterator > search_list;

	The storage list is created from the received data, added in the order of arrival, and as a result, it is an unordered sequence of objects of type T.

	The list search_list is a list that contains iterators for objects in storage. This list is ordered. It is logical that there are as many elements in it as in storage, but they are smaller in case the type T is larger than the list <T> list iterator (this is, almost always).
It needs to be understood that the iterator of the list search_list is an iterator on an iterator, that is, like a pointer to a pointer.
	The third container is std :: vector < NodePointer > bin_search_vec. It contains the NodePointer structure. The NodePointer structure is very simple. It contains an iterator in storage (the same type that is contained in search_list) and length int len. In fact, the element of the vector (NodePointer) contains some elements search_list (list <t> :: const_iterator) located at some distance len from the next element of this list to which the next element of the vector indicates. That is, usually between the elements of the list that are duplicated in the elements of the vector there are chains of elements, which are absent in the elements of the vector. Such chains are similar to lists of collisions in hash arrays  (implemented on chains).
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

Reading next text, look the Pic files attached there, please.

	In the pictures Pic. 1, Pic. 2, Pic. 3, is shown the filling of Polisearcher containers with the sequence of integers: 9,3,6,1,12,5,2,4,7,10,8,11,15,13,14,16
If all containers are initially empty then adding the one element will result in the appearance of this element in each of the containers. The vector will note that its only chain has one element (L = 1). And this an element is a leader for that a chain, of course. Its iterator in a single NodePointer of the vector is a copy of a single element of the search list that represents an iterator on a single element of int 9 in the storage list. Later, the addition of three more elements 3.6 and 1 leads to the fact that in the storage in the order of receipt are the elements 9,3,6,1. In the search list, they are ordered 1,3,6,9 and the only element of the vector NodPointer structure contains an iterator on 1 in the search list and the length of the chain expressed as len is equal 4. Adding element 12 leads to a doubling of the elements of the vector.

	To understand how the vector grows, you need to understand what is the field "norma" of the PoliSearcher class.
Norma is a positive integer indicating the number of times (from division to the whole) the starting size of the normalized chain ch_sz = chain_size / norma is less than the maximum size chain_size.
	Since the norma field in the search list is set by default and equal to 2, this means that the starting size of the new chains will be twice less than the maximum size (chain_size = 4), that is, the starting len = 2 for all new chains. Their quantity will therefore be twice as much as before, that is, two. Further, the addition of new elements leads to an increase in the total number of elements (storage size) to 7. The size of the first and second chains at this point becomes 4 and 3, respectively. Here there is a situation when the size of the search list has not yet reached a maximum for two chains of four, that is eight, but exactly the chain where the element 4 should be placed is already filled with four elements: 1,2,3,5. Next, a search is performed on the all (both) free chains to find a chain with len less of 4. Such a chain is present - it is only one and has 3 elements. Next, the leader of this chain - the iterator in the vector is shifted to one position in the direction of the chain requesting free space in exchange for an extra element. Now the leader of the chain giving up free space is changed from 6 to 5, and in the chain where the 4 will be placed there are three elements: 1,2,3 and after adding 4 becomes 1,2,3,4. This is shown in Fig. Pic. 2. Here the addition of a new element with value 7 causes a new doubling of the vector, since two chains of four elements are completely filled. A new vector of four elements is ready to accept another eight (or give four and have contraction itself in half), after which a new doubling will happen. The appearance after doubling from 4 to 8 is shown in Fig. Pic. 3.
	When you delete elements, they are extracted from the chains and the chain is rebuilt without changing the size of the vector until there is  one element left in each chain. At such a moment, the number of elements of the branch and the list are equal. In this case, further removal without decreasing the size of the vector is impossible. Decrease of the vector at norma = 2 occurs also exactly twice. While the new size of each chain is 2 and it is ready for both growth and further removal without changing the size of the vector until the boundary size of the list is reached relative to the size of the vector-the condition for changing the size of the vector.

Brief class description:

namespace PoliSearchering

Class PoliSearcher - the structure of storage and search of elements of a given type, according to predicates comparison strictly more  or  less.

Constructors:

template < typename T, size_t chain_size > class PoliSearcher {

PoliSearcher
	(
		list< T > & storage_, 
		const string & pred_description=less_default_pred_description(),
		Pred predicate_ =less_default, 
		int norma_=2
	);


template<typename FwdIt>
PoliSearcher
	(
		const FwdIt bg_,
		const FwdIt en_, 
		const string & pred_description=less_default_pred_description(),
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
Parametr predicate_ is the pointer to the default comparison predicate. By default it corresponds to the call of the operator < on type T.
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
