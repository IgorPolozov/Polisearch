#include<list>
#include<map>
#include<vector>
#include<algorithm>

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

IterToIter it=upper_bound(it_start, it_last, val, *this);//this it pointing to the next to the last val 

--it; 
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
if(having_a_free_cell())//функци€ корректно работает только если все цепи заполнены
//то есть нет свободного места, поэтому тут от греха - проверка и выход если есть свободна€
//€чейка
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

void show_search_list(std::ostream & os, const std::string &name="")
	{
		if(name.length()!=0)
			{
				os<<name<<std::endl;
			}
				for(IterToIter it=search_list.begin(); it != search_list.end(); it++)
					{
						os<<(*(*it))<<' ';
					}
						os<<std::endl;
	}

void show_bin_search_vec(std::ostream & os, const std::string &name="")
	{
	if(name.length()!=0)
		{
			os<<name<<std::endl;
		}
   os<<"bin_search_vec.size() = "<< bin_search_vec.size()<<std::endl;
    os<<std::endl;
			for(IterNodePointer it=bin_search_vec.begin(); it != bin_search_vec.end(); it++)
				{
                os<<(*(*it->node_it))<<' '<<"the len of the chain is: "<<it->len<<std::endl;
				}
					os<<std::endl;
	}

void show_bin_search_vec(std::ostream & os, IterNodePointer b, IterNodePointer e, const std::string &name="")
	{
		if(name.length()!=0)
			{
				os<<name<<std::endl;
			}
				for(IterNodePointer it=b; it != e; it++)
					{                    
						os<<(*(*it->node_it))<<' '<<it->len<<std::endl;
					}
						os<<std::endl;
	}


void show_chain(std::ostream & os, size_t ind, const std::string &str="" )
	{
		os<<str<<std::endl;
			if(ind>bin_search_vec.size()-1)
				{
					os<<"show_chain returns with out of index= "
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
							os<<**lid++<<' ';
				}
					os<<"\n__________________________\n";//отладочное))
	}

void show_chains(std::ostream & os, const std::string &str="")
	{
		os<<std::endl<<str<<std::endl;
			os<<"\n__________________________\n";
				for(size_t i=0; i<bin_search_vec.size(); ++i)
					{
						os<<"# "<<i<<" #";show_chain(os,i);
					}
	}	

void show(std::ostream & os, const std::string &str="")
	{
		show_search_list(os,"show_search_list "+str);       
			show_bin_search_vec(os,"show_bin_search_vec "+str);           
				show_chains(os,"show_chains "+str);
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


void show(std::ostream& os, Pred predicate_=less_default, const std::string& str="" ){
    ListListedsIter listed_list_it =  list_Listeds.find(predicate_); 
    if(listed_list_it != list_Listeds.end()){
        os<<"pred_description= "<<listed_list_it->second.pred_description<<std::endl;
        if(predicate_==less_default)
            listed_list_it->second.show(os, PoliSearcher::less_default_pred_description());
		else
			listed_list_it->second.show(os, str);
        }
else{os<<"listed_list not found in void show(ListListedsIter listed_list_it, string str=\"\")";}
    }

void show_search_list(std::ostream& os, Pred predicate_=less_default, const std::string &str="" ){
ListListedsIter listed_list_it =  list_Listeds.find(predicate_); 
    if(listed_list_it != list_Listeds.end()){
        os<<"pred_description= "<<listed_list_it->second.pred_description<<std::endl;
        if(predicate_==less_default)str=PoliSearcher::less_default_pred_description();
            listed_list_it->second.show_search_list(os, "The search_list is sorted by "+str); 
        }
else{os<<"listed_list not found in void show(ListListedsIter listed_list_it, string str=\"\")";}
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
