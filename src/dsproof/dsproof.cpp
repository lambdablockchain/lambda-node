// Copyright (C) 2019-2020 Tom Zander <tomz@freedommail.ch>
// Copyright (C) 2020 Calin Culianu <calin.culianu@gmail.com>
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <dsproof/dsproof.h>
#include <hash.h>

#include <limits>
#include <stdexcept>

/* static */
bool DoubleSpendProof::s_enabled = true;

bool DoubleSpendProof::isEmpty() const
{
    return prevOutIndex() > static_cast<uint32_t>(std::numeric_limits<int32_t>::max())
            || prevTxId().IsNull() || GetId().IsNull();
}

void DoubleSpendProof::setHash()
{
    m_hash = SerializeHash(*this);
}

void DoubleSpendProof::checkSanityOrThrow() const
{
    if (isEmpty()) 
        { throw std::runtime_error("DSProof is empty"); }
   

    for (auto *pushData : {&m_spender1.pushData, &m_spender2.pushData}) {
        if (pushData->size() != 1)
           { throw std::runtime_error("DSProof must contain exactly 1 pushData"); }
       
        if (!pushData->empty() && pushData->front().size() > MaxPushDataSize) 
           { throw std::runtime_error("DSProof script size limit exceeded"); }
       
    }
    if (m_spender1 == m_spender2) 
       { throw std::runtime_error("DSProof both spenders are the same"); } 
   
}
