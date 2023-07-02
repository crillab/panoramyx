/**
* @date 28/06/23
* @file AbstractProblemUnderAssumptionCubeGenerator.cpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#include <cassert>
#include "crillab-panoramyx/decomposition/AbstractProblemUnderAssumptionCubeGenerator.hpp"
#include "crillab-panoramyx/decomposition/StreamPriorityQueueAdapter.hpp"
#include "loguru.hpp"
using namespace std;
using namespace Universe;

namespace Panoramyx {

/**
@class AbstractProblemUnderAssumptionCubeGenerator
@brief Definition of the class AbstractProblemUnderAssumptionCubeGenerator. 
@file AbstractProblemUnderAssumptionCubeGenerator.cpp
*/
    AbstractProblemUnderAssumptionCubeGenerator::AbstractProblemUnderAssumptionCubeGenerator(int nbCubesMax)
            : AbstractCubeGenerator(nbCubesMax) {}

    Stream<vector<UniverseAssumption<BigInteger>>>* AbstractProblemUnderAssumptionCubeGenerator::generateCubes() {
        priorityQueue.emplace(std::vector<UniverseAssumption<BigInteger>>(),0);

        while(priorityQueue.size()<nbCubesMax){
            bool added=false;
            auto problem = priorityQueue.top();
            priorityQueue.pop();
            solver->reset();
            solver->solve(problem.getAssumpts());
            for(auto& x:solver->getVariablesMapping()){
                if(x.second->getDomain()->currentSize()>1){
                    for(auto a:x.second->getDomain()->getCurrentValues()){
                        std::vector<UniverseAssumption<BigInteger>> newAssumpts(problem.getAssumpts());
                        for (auto b : newAssumpts) {
                            assert(b.getVariableId()!= x.second->getName());
                        }
                        newAssumpts.emplace_back(x.first,true,a);
                        DLOG_F(INFO,"trouvé ! %s = %d",x.first.c_str(),a);
                        long s = computeScore(newAssumpts);
                        if(s>=0){
                            priorityQueue.emplace(newAssumpts, s);
                        }
                    }
                    added=true;
                    break;
                }
            }
            if(!added){
                priorityQueue.emplace(problem);
                DLOG_F(INFO,"n'a plu");
                break;
            }
        }

        return new StreamPriorityQueueAdapter(priorityQueue);
    }


} // Panoramyx