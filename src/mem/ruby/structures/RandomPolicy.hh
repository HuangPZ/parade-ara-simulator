/*
 * Copyright (c) 2007 Mark D. Hill and David A. Wood
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __MEM_RUBY_STRUCTURES_RANDOMPOLICY_HH__
#define __MEM_RUBY_STRUCTURES_RANDOMPOLICY_HH__

#include "mem/ruby/structures/AbstractReplacementPolicy.hh"
#include "base/random.hh"
/* Simple true LRU replacement policy */

class RandomPolicy : public AbstractReplacementPolicy
{
  public:
    RandomPolicy(int64 num_sets, int64 assoc);
    ~RandomPolicy();

    void touch(int64 set, int64 way, Tick time);
    int64 getVictim(int64 set) const;
};

inline
RandomPolicy::RandomPolicy(int64 num_sets, int64 assoc)
    : AbstractReplacementPolicy(num_sets, assoc)
{
}

inline
RandomPolicy::~RandomPolicy()
{
}

inline void
RandomPolicy::touch(int64 set, int64 index, Tick time)
{
    assert(index >= 0 && index < m_assoc);
    assert(set >= 0 && set < m_num_sets);

    m_last_ref_ptr[set][index] = time;
}

inline int64
RandomPolicy::getVictim(int64 set) const
{
    //  assert(m_assoc != 0);
    Tick time, smallest_time;
    int64 smallest_index;

    smallest_index = 0;
    smallest_time = m_last_ref_ptr[set][0];

    for (unsigned i = 0; i < m_assoc; i++) {
        time = m_last_ref_ptr[set][i];
        // assert(m_cache[cacheSet][i].m_Permission !=
        //     AccessPermission_NotPresent);

        if (time < smallest_time) {
            smallest_index = i;
            smallest_time = time;
        }
    }

    //  DEBUG_EXPR(CACHE_COMP, MedPrio, cacheSet);
    //  DEBUG_EXPR(CACHE_COMP, MedPrio, smallest_index);
    //  DEBUG_EXPR(CACHE_COMP, MedPrio, m_cache[cacheSet][smallest_index]);
    //  DEBUG_EXPR(CACHE_COMP, MedPrio, *this);

    return random_mt.random<int>(0, m_assoc - 1);
}

#endif // __MEM_RUBY_STRUCTURES_LRUPOLICY_HH__
