#ifndef __MEM_CACHE_REPLACEMENT_POLICIES_LRUIPV_RP_HH__
#define __MEM_CACHE_REPLACEMENT_POLICIES_LRUIPV_RP_HH__

#include "mem/cache/replacement_policies/base.hh"

struct LRUIPVRPParams;
namespace ReplacementPolicy {

class LRUIPVRP : public Base
{
  protected:

    //given IPV vector as per quiz
    const std::vector<uint64_t> IPV{0,0,1,0,3,0,1,0,1,7,5,1,0,0,1,11,14};
    
    uint64_t count;

    //no. of cache blocks in a set i.e ways in a cache
    const uint64_t num_way;

    typedef std::vector<uint64_t> lruIPV;

    lruIPV* vecInst;

    /** replacement data LRU*/
    struct LRUReplData : ReplacementData
    {
        /** Tick on which the entry was last touched. */
        Tick lastTouchTick;

        mutable uint64_t index;

        std::shared_ptr<lruIPV> vect;
        /**
         * Default constructor. 
         */
        LRUReplData(uint64_t assoc, std::shared_ptr<lruIPV> vect);
    };


  public:

    typedef LRUIPVRPParams Params;

    //initialize
    LRUIPVRP(const Params &p);

    //destructor
    ~LRUIPVRP() = default;

    /**
     * Touch an entry to update its replacement data.
     * Sets its last touch tick as the current tick.
     *
     * @param replacement_data Replacement data to be touched.
     */
    void touch(const std::shared_ptr<ReplacementData>& replacement_data) const
                                                                     override;

    /**
     * Reset replacement data. Used when an entry is inserted.
     * Sets its last touch tick as the current tick.
     *
     * @param replacement_data Replacement data to be reset.
     */
    void reset(const std::shared_ptr<ReplacementData>& replacement_data) const
                                                                     override;

    /**
     * Invalidate replacement data to set it as the next probable victim.
     * Sets its last touch tick as the starting tick.
     *
     * @param replacement_data Replacement data to be invalidated.
     */
    void invalidate(const std::shared_ptr<ReplacementData>& replacement_data) const
                                                              override;

    /**
     * Find replacement victim using LRU timestamps.
     *
     * @param candidates Replacement candidates, selected by indexing policy.
     * @return Replacement entry to be replaced.
     */
    ReplaceableEntry* getVictim(const ReplacementCandidates& candidates) const
                                                                     override;

    /**
     * Instantiate a replacement data entry.
     *
     * @return A shared pointer to the new replacement data.
     */
    std::shared_ptr<ReplacementData> instantiateEntry() override;
};
}
#endif // __MEM_CACHE_REPLACEMENT_POLICIES_LRUIPV_RP_HH__
