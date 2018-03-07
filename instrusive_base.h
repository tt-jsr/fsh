/***************************************************************************
 *
 *                    Unpublished Work Copyright (c) 2014
 *                  Trading Technologies International, Inc.
 *                       All Rights Reserved Worldwide
 *
 *          * * *   S T R I C T L Y   P R O P R I E T A R Y   * * *
 *
 * WARNING:  This program (or document) is unpublished, proprietary property
 * of Trading Technologies International, Inc. and is to be maintained in
 * strict confidence. Unauthorized reproduction, distribution or disclosure
 * of this program (or document), or any program (or document) derived from
 * it is prohibited by State and Federal law, and by local law outside of
 * the U.S.
 *
 ***************************************************************************/
#pragma once
#include <atomic>
#include <cassert>

namespace fsh
{
    class instrusive_base
    {
    public:
        instrusive_base()
        :refcount(0)
        {}

        virtual ~instrusive_base()
        { }

        int dec_ref()
        {
            assert(refcount > 0);
            int val = refcount.fetch_add(-1);
            return val-1;
        }
        void inc_ref()
        {
            refcount.fetch_add(1);
        }
        mutable std::atomic<int> refcount;
    };
}
