#include <minisat/core/Solver.h>
#include <minisat/core/Dimacs.h>
#include <iostream>
#include <vector>
#include <map>

using namespace std;
using Minisat::mkLit;
using Minisat::lbool;

void readDimacsInput(int& argc, char**& argv, Minisat::Solver& S) {
    FILE* in = (argc == 1) ? fdopen(0, "rb") : fopen(argv[1], "rb");
    if (in == NULL)
        fprintf(stderr, "ERROR! Could not open file: %s\n", argc == 1 ? "<stdin>" : argv[1]), exit(1);
    parse_DIMACS(in, S);
    fclose(in);
}

void readVarRepresentations(map<int, string>& M) {
    int n; cin >> n;
    for (int i = 0; i < n; i++) {
        string var; int rep;
        cin >> var >> rep;
        M[rep] = var;
    }
}

void banSatisfyingInterpretation(Minisat::Solver& S) {
    Minisat::vec<Minisat::Lit> clause;
    for (int i = 0; i < S.nVars(); i++)
        if (S.model[i] != l_Undef)
            clause.push((S.model[i] == l_True ? ~mkLit(i) : mkLit(i)));
    S.addClause(clause);
}

void insertSolution(Minisat::Solver& S, vector<vector<int>>& V) {
    vector<int> solution;
    for (int i = 0; i < S.nVars(); i++)
        if (S.model[i] != l_Undef)
            solution.push_back((i+1)*((S.model[i] == l_True) ? 1 : -1));
    V.push_back(solution);
}

void printInterpretations_V1(vector<vector<int>>& V, map<int, string>& M) {
    int cnt = 0;
    cout << "    ";
    for (auto KV : M)
        cout << KV.second << " ";
    cout << '\n';
    for (vector<int> v : V) {
        cout << "[" << ++cnt << "] ";
        for (int i : v)
            cout << (i < 0 ? "F" : "T") << " ";
        cout << '\n';
    }
}

void printInterpretations_V2(vector<vector<int>>& V, map<int, string>& M) {
    // $\mathcal{I}(p) = T, \textbf{ } \mathcal{I}(p2) = q$
    for (vector<int> v : V) {
        cout << "\\item $";
        for (int i = 0; i < (int)v.size(); i++)
            cout << "\\mathcal{I}(" << M[abs(v[i])] << ") = " << (v[i] < 0 ? "F" : "T") << (i == (int)v.size()-1 ? "" : ", \\textbf{ } ");
        cout << "$\n";
    }
}

int main(int argc, char** argv) {
    Minisat::Solver S;
    vector<vector<int>> solutions;
    map<int, string> varRepresentations;

    readDimacsInput(argc, argv, S);
    readVarRepresentations(varRepresentations);

    while (S.solve())
        insertSolution(S, solutions), banSatisfyingInterpretation(S);

    cout << "There are " << (int)solutions.size() << " satisfying interpretations.\n";
    printInterpretations_V1(solutions, varRepresentations);
    printInterpretations_V2(solutions, varRepresentations);
}