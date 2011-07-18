/** moses-ann-pole2.cc --- 
 *
 * Copyright (C) 2010-2011 OpenCog Foundation
 *
 * Author: Joel Lehman
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2, or (at
 * your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */
#include <iostream>

#include <opencog/util/mt19937ar.h>
#include <opencog/util/Logger.h>

#include <opencog/comboreduct/combo/eval.h>

#include "../moses/representation.h"
#include "../moses/moses.h"
#include "../moses/optimization.h"
#include "../moses/scoring_functions.h"
#include "../moses/scoring.h"
#include "../moses/ann_scoring.h"

using namespace std;
using namespace boost;
using namespace opencog;
using namespace moses;
using namespace reduct;


int main(int argc, char** argv)
{

    //set flag to print only cassert and other ERROR level logs on stdout
    opencog::logger().setPrintErrorLevelStdout();
    //read maximum evaluations and RNG seed from command line
    int max_evals;
    int seed;
    bool reduce=true;
    try {
       // if (argc!=3)
        //    throw "foo";
        max_evals=lexical_cast<int>(argv[1]);
        seed=lexical_cast<int>(argv[2]);
        set_stepsize(1.25); //lexical_cast<double>(argv[3]));
        set_expansion(1.5); //lexical_cast<double>(argv[4]));
        set_depth(4) ; //exical_cast<int>(argv[5]));
        reduce = true;
    } catch (...) {
        cerr << "usage: " << argv[0] << " maxevals seed" << endl;
        exit(1);
    }
    
    //read in seed tree
    combo_tree tr;
    cin >> tr; 

    opencog::MT19937RandGen rng(seed);

    type_tree tt(id::lambda_type);
    tt.append_children(tt.begin(), id::ann_type, 1);

    //DOUBLE MARKOVIAN POLE TASK`
    const reduct::rule* si = &(ann_reduction());
    if(!reduce)
        si = &(clean_reduction());
    
    ann_pole2_score p2_score;
    ann_pole2_bscore p2_bscore; 

    metapopulation<ann_pole2_score, ann_pole2_bscore, univariate_optimization>
        metapop_pole2(rng, tr, tt, *si, p2_score, p2_bscore,
                      univariate_optimization(rng));

    moses::moses(metapop_pole2);

    //change best combo tree back into ANN
    tree_transform trans; 
    combo_tree best = metapop_pole2.best_tree();
    ann bestnet = trans.decodify_tree(best);
    
    //show best network
    cout << "Best network: " << endl;
    cout << &bestnet << endl;
    //write out in dot format
    bestnet.write_dot("best_nn.dot"); 

    //for parameter sweet
    cout << metapop_pole2.best_score() << endl;
}




