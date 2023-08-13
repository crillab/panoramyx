/**
 * PANORAMYX - Programming pArallel coNstraint sOlveRs mAde aMazingly easY.
 * Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.
 * If not, see {@link http://www.gnu.org/licenses}.
 */

/**
 * @file KahyparDecompositionSolver.cpp
 * @brief Provides an hypergraph decomposition solver based on kahypar.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#include <loguru.hpp>
#include <crillab-except/except.hpp>
#include <crillab-panoramyx/decomposition/KahyparDecompositionSolver.hpp>

using namespace std;

using namespace Except;
using namespace Panoramyx;
using namespace Universe;

KahyparDecompositionSolver::KahyparDecompositionSolver(
        double imbalance, int nbPartitions, const string &configurationFile) :
        imbalance(imbalance),
        nbBlocks(nbPartitions),
        context(kahypar_context_new()),
        partition(nullptr) {
    kahypar_configure_context_from_file(context, configurationFile.c_str());
}

KahyparDecompositionSolver::~KahyparDecompositionSolver() {
    kahypar_context_free(context);
}

UniverseSolverResult KahyparDecompositionSolver::solve() {
    if (partition == nullptr) {
        auto *hypergraph = getHypergraph();
        int objective = 0;
        partition = new int[hypergraph->getNumberOfVertices()];

        LOG_F(INFO, "starting decomposition with KaHyPar into %d blocks", nbBlocks);
        kahypar_partition(hypergraph->getNumberOfVertices(), hypergraph->getNumberOfHyperedges(), imbalance, nbBlocks,
                          hypergraph->getVertexWeights(), hypergraph->getHyperedgeWeights(),
                          reinterpret_cast<const size_t *>(hypergraph->getHyperedgeIndices()),
                          reinterpret_cast<const kahypar_hyperedge_id_t *>(hypergraph->getHyperedgeVertices()),
                          &objective,
                          context, partition);
        LOG_F(INFO, "finished decomposition with KaHyPar");

#if 0
        LOG_F(INFO, "starting decomposition improvement with KaHyPar");
        kahypar_improve_partition(hypergraph->getNumberOfVertices(), hypergraph->getNumberOfHyperedges(), imbalance,
                                  nbBlocks,
                                  hypergraph->getVertexWeights(), hypergraph->getHyperedgeWeights(),
                                  reinterpret_cast<const size_t *>(hypergraph->getHyperedgeIndices()),
                                  reinterpret_cast<const kahypar_hyperedge_id_t *>(hypergraph->getHyperedgeVertices()),
                                  tmpPartition,
                                  5, &objective, context, partition);
        LOG_F(INFO, "finished decomposition improvement with KaHyPar");
#endif
    }

    return UniverseSolverResult::SATISFIABLE;
}

UniverseSolverResult KahyparDecompositionSolver::solve(const string &filename) {
    loadInstance(filename);
    return solve();
}

UniverseSolverResult KahyparDecompositionSolver::solve(const vector<UniverseAssumption<BigInteger>> &assumptions) {
    throw UnsupportedOperationException("not implemented yet");
}

vector<vector<int>> KahyparDecompositionSolver::getPartition() {
    if (constraintPartitionAsVector.empty()) {
        vector<vector<int>> partitionAsVector(nbBlocks);
        for (int constraintId = 0; constraintId < getHypergraph()->getNumberOfVertices(); constraintId++) {
            partitionAsVector[partition[constraintId]].emplace_back(constraintId);
        }
        constraintPartitionAsVector = partitionAsVector;
    }
    return constraintPartitionAsVector;
}

vector<vector<string>> KahyparDecompositionSolver::getVariablePartition() {
    if (variablePartitionAsVector.empty()) {
        cutset();
    }
    return variablePartitionAsVector;
}

vector<string> KahyparDecompositionSolver::cutset() {
    if (cutsetVector.empty()) {
        vector<string> varInCutset;
        vector<vector<string>> varPartition(nbBlocks);
        // Looking for hyperedges in the cutset.
        auto *hypergraph = getHypergraph();
        for (int i = 0; i < hypergraph->getNumberOfHyperedges(); i++) {
            int begin = (int) hypergraph->getHyperedgeIndices()[i];
            int end = (int) hypergraph->getHyperedgeIndices()[i + 1];

            // Comparing the block of the different vertices.
            int first = partition[hypergraph->getHyperedgeVertices()[begin]];
            bool allSame = true;
            for (int j = begin + 1; j < end; j++) {
                if (partition[hypergraph->getHyperedgeVertices()[j]] != first) {
                    // The hyperedge joins two vertices that are in different blocks.
                    // It is thus in the cutset
                    varInCutset.push_back(orderedVariables[i]);

                    allSame = false;
                    break;
                }
            }
            if (allSame) {
                varPartition[first].push_back(orderedVariables[i]);
            }
        }
        cutsetVector = varInCutset;
        variablePartitionAsVector =varPartition;
    }
    return cutsetVector;
}