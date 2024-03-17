# Computer Architecture LRUIPV cache replacement 
 LRUIPV cache replacement policy to be implemented as a new cache policy in
the gem5 simulator. There are four main functions invoked by each cache replacement policy:
reset, touch, invalidate, and getVictim.The different functions about cache
replacement policies in the following website:
1. reset(): It is used to initialize a replacement data. It is called upon only insertion of a cache
block (at the beginning or upon replacement).
2. touch(): It is invoked upon a cache hit. It should update the replacement data of the cache
block.
3. invalidate(): It is called whenever an entry is invalidated. It should invalidate the
replacement data, seing the entry as the next probable victim for eviction.
4. getVictim(): It is called when there is a miss, and an eviction must be done. It searches
among all replacement candidates for an entry to be evicted (based on the replacement data of
candidates being searched)