/*
 * mpi_moses.h --- 
 *
 * Copyright (C) 2012 Poulin Holdings LLC
 *
 * Author: Linas Vepstas
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License v3 as
 * published by the Free Software Foundation and including the exceptions
 * at http://opencog.org/wiki/Licenses
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU Affero General Public License
 * along with this program; if not, write to:
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */


#ifndef _OPENCOG_MPI_MOSES_H
#define _OPENCOG_MPI_MOSES_H

#include <opencog/util/pool.h>

#include "metapopulation.h"
#include "moses_params.h"

namespace opencog { namespace moses {

#ifdef HAVE_MPI

struct dispatch_thread
{
   dispatch_thread();
   int rank;
};

class moses_mpi
{
    public:
        typedef instance_set<composite_score> deme_t;

        moses_mpi();
        ~moses_mpi();

        bool is_mpi_master();
        void dispatch_deme(const combo_tree&, int max_evals);

        int recv_more_work();
        combo_tree recv_exemplar();
        void return_deme(deme_t*, representation*, size_t);
    private:
        // master state
        std::vector<dispatch_thread> workers;
        pool<dispatch_thread> worker_pool;

};


template<typename Scoring, typename BScoring, typename Optimization>
void mpi_moses(metapopulation<Scoring, BScoring, Optimization>& mp,
               const moses_parameters& pa,
               moses_statistics& stats)
{
    typedef bscored_combo_tree_set::const_iterator mp_cit;

    logger().info("MPI MOSES starts");
    moses_mpi mompi;

    // Slave processes loop until done, then return.
    if (!mompi.is_mpi_master()) {
        while(1) {
            int max_evals = mompi.recv_more_work();
            if (0 >= max_evals)
                return;

            combo_tree exemplar = mompi.recv_exemplar();
            if (!mp._dex.create_deme(exemplar)) {
                // XXX replace this with appropriate message back to root!
                OC_ASSERT(false, "Exemplar failed to expand!\n");
            }
            size_t evals_this_deme = mp._dex.optimize_deme(max_evals);
            mompi.return_deme(mp._dex._deme, mp._dex._rep, evals_this_deme);
            mp._dex.free_deme();
        }
    }

    while ((stats.n_evals < pa.max_evals) 
           && (pa.max_gens != stats.n_expansions)
           && (mp.best_score() < pa.max_score))
    {

        mp_cit exemplar = mp.select_exemplar();
        if (exemplar == mp.end()) {
            logger().warn("There are no more exemplars in the metapopulation "
                          "that have not been visited and yet a solution was "
                          "not found.  Perhaps reduction is too strict?");
            // goto theend;

            // If there are no more exemplars in our pool, we will
            // have to wait for some more to come rolling in.
            break;
        }
        const combo_tree &extree = get_tree(*exemplar);
        mompi.dispatch_deme(extree, pa.max_evals - stats.n_evals);
    }
};

#else

class moses_mpi
{
    public:
        moses_mpi() {}
        ~moses_mpi() {}

        bool is_mpi_master() { return true; }
};

template<typename Scoring, typename BScoring, typename Optimization>
void mpi_moses(metapopulation<Scoring, BScoring, Optimization>& mp,
               const moses_parameters& pa,
               moses_statistics& stats)
{
    OC_ASSERT(0, "There is no MPI support in this version of moses");
};

#endif

} // ~namespace moses
} // ~namespace opencog

#endif // _OPENCOG_MPI_MOSES_H
