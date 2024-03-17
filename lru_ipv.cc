#include "mem/cache/replacement_policies/lru_ipv.hh"

#include <cassert>
#include <memory>
#include <cmath>

#include "base/intmath.hh"
#include "params/LRUIPVRP.hh"
#include "base/logging.hh"

namespace ReplacementPolicy {

//Instantiate values
LRUIPVRP::LRUIPVRP(const Params &p)
    : Base(p),num_way(p.numWays), count(0), vecInst(nullptr)
{
}

//Constructor
LRUIPVRP::LRUReplData::LRUReplData( uint64_t index, std::shared_ptr<lruIPV> vect): index(index), vect(vect)  {
}


//this function is called upon initialisation of a cache block
void
LRUIPVRP::reset(const std::shared_ptr<ReplacementData>& repl_data) const
{
    std::shared_ptr<LRUReplData> data = std::static_pointer_cast<LRUReplData>(repl_data);
    lruIPV* vect1 = data->vect.get();
       
    uint64_t index = 0;     

    while (index < vect1->size()) 
    {   
         if ((vect1->at(index) >= IPV[num_way]) && (vect1->at(index) < num_way)){ 
             vect1->at(index) = vect1->at(index) + 1;
         }
         index++;
    }
    vect1->at(data->index) = IPV[num_way];
}
//this function is used to instantiate a particular cache block
std::shared_ptr<ReplacementData>
LRUIPVRP::instantiateEntry()
{
    
    //create a vector for each set. Number of elements in the vector is equal to the set associativity (num_way)
    if(count % num_way == 0)
    {
        vecInst = new lruIPV(num_way,num_way);
    }

    LRUReplData* replData = new LRUReplData(count % num_way,std::shared_ptr<lruIPV>(vecInst));

    count++;
    return std::shared_ptr<ReplacementData>(replData);
   
}

//This fucntion is used to access a existing cache block . this is called when cache block is accesed again after entering the cache
void
LRUIPVRP::touch(const std::shared_ptr<ReplacementData>& repl_data) const
{
    std::shared_ptr<LRUReplData> data = std::static_pointer_cast<LRUReplData>(repl_data); 
    lruIPV* vect1 = data->vect.get();

    //get value for promotion from table
    uint64_t newPos = IPV[data->index];   
    uint64_t oldPos = vect1->at(data->index);
    uint64_t index = 0;
    while (index < vect1->size()) 
    {
         if((vect1->at(index) >= newPos) && (vect1->at(index)< oldPos))
         { 
             vect1->at(index) = vect1->at(index) + 1;
         }
         index++;
     }
     vect1->at(data->index) = newPos;
 
}

//this function is called to find the pseudo lru cache element in order for it to be removed from the cache. this is used to find the cache block to evict from the cache and bring in a new cache block in its place 
ReplaceableEntry*
LRUIPVRP::getVictim(const ReplacementCandidates& repl_candidates) const
{

    assert(repl_candidates.size() > 0);

    std::shared_ptr<LRUReplData> data = std::static_pointer_cast<LRUReplData>(repl_candidates[0]->replacementData); 
    lruIPV* vect1 = data->vect.get();
    uint64_t index = 0;
    uint64_t max = 0;
    uint64_t insertIndex = 0;
    while (index < vect1->size()) 
    {
        if(max < vect1->at(index))
        {
            max = vect1->at(index);
            insertIndex = index;
        }
        index++;
    }
    return repl_candidates[insertIndex];

}

//this function is called when we want to invalidate a cache block thereby removing from the cache and bringing in a new cache block
void
LRUIPVRP::invalidate(const std::shared_ptr<ReplacementData>& repl_data) const
{
    //do nothing in this function
    return; 
}
}
