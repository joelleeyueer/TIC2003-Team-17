//
// Created by Nadia Atiqah on 9/2/21.
//

#include <set>

#include "SelectClauseEvaluator.h"
#include "ClauseUtility.h"

using namespace std;

void SelectClauseEvaluator::evaluate(unordered_set<string> &result) {
    switch(synType) {
        case DesignEntity::ASSIGN:
            return evaluateSelectAssign(result);

        case DesignEntity::STMT:
            return evaluateSelectStmt(result);

        case DesignEntity::PROG_LINE:
            return evaluateSelectStmt(result);

        case DesignEntity::READ:
            return evaluateSelectRead(result);

        case DesignEntity::PRINT:
            return evaluateSelectPrint(result);

        case DesignEntity::WHILE:
            return evaluateSelectWhile(result);

        case DesignEntity::IF:
            return evaluateSelectIf(result);

        case DesignEntity::CALL:
            return evaluateSelectCall(result);

        case DesignEntity::VARIABLE:
            return evaluateSelectVar(result);

        case DesignEntity::CONSTANT:
            return evaluateSelectConst(result);

        case DesignEntity::PROCEDURE:
            return evaluateSelectProc(result);

        default:
            return;
    }
}

void SelectClauseEvaluator::evaluateSelectStmt(unordered_set<string> &result) {
    unordered_set<int> pkbResult = pkbInstance->getAllStmt();
    if (pkbResult.empty()) return;

    result = ClauseUtility::convertIntSetToString(pkbResult);
}

void SelectClauseEvaluator::evaluateSelectAssign(unordered_set<string> &result) {
    unordered_set<int> pkbResult = pkbInstance->getAllAssignmentStmt();
    if (pkbResult.empty()) return;

    result = ClauseUtility::convertIntSetToString(pkbResult);
}

void SelectClauseEvaluator::evaluateSelectRead(unordered_set<string> &result) {
    unordered_set<int> pkbResult = pkbInstance->getAllReadStmt();
    if (pkbResult.empty()) return;
    result = ClauseUtility::convertIntSetToString(pkbResult);
}

void SelectClauseEvaluator::evaluateSelectPrint(unordered_set<string> &result) {
    unordered_set<int> pkbResult = pkbInstance->getAllPrintStmt();
    if (pkbResult.empty()) return;
    result = ClauseUtility::convertIntSetToString(pkbResult);
}

void SelectClauseEvaluator::evaluateSelectCall(unordered_set<string> &result) {
    unordered_set<int> pkbResult = pkbInstance->getAllCallStmt();
    if (pkbResult.empty()) return;
    result = ClauseUtility::convertIntSetToString(pkbResult);
}

void SelectClauseEvaluator::evaluateSelectWhile(unordered_set<string> &result) {
    unordered_set<int> pkbResult = pkbInstance->getAllWhileStmt();
    if (pkbResult.empty()) return;

    result = ClauseUtility::convertIntSetToString(pkbResult);
}

void SelectClauseEvaluator::evaluateSelectIf(unordered_set<string> &result) {
    unordered_set<int> pkbResult = pkbInstance->getAllIfStmt();
    if (pkbResult.empty()) return;

    result = ClauseUtility::convertIntSetToString(pkbResult);
}

void SelectClauseEvaluator::evaluateSelectProc(unordered_set<string> &result) {
    unordered_set<string> pkbResult = pkbInstance->getAllProcedures();
    if (pkbResult.empty()) return;

    result = pkbResult;
}


void SelectClauseEvaluator::evaluateSelectVar(unordered_set<string> &result) {
    unordered_set<string> pkbResult = pkbInstance->getAllVariables();
    if (pkbResult.empty()) return;

    result = pkbResult;
}

void SelectClauseEvaluator::evaluateSelectConst(unordered_set<string> &result) {
    unordered_set<string> pkbResult = pkbInstance->getAllConstants();
    if (pkbResult.empty()) return;

    result = pkbResult;
}