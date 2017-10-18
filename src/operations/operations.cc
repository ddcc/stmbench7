#include "operations.h"

#include "traversal_ops.h"
#include "query_ops.h"
#include "short_traversal_ops.h"
#include "operation_ops.h"
#include "structural_modification_ops.h"

#include "../parameters.h"

using namespace sb7;

static bool isOperationTypeReadOnly(const enum optype ot) {
	return (ot == TRAVERSAL_RO ||
		ot == SHORT_TRAVERSAL_RO ||
		ot == OPERATION_RO);
}

bool sb7::Operation::isReadOnly() const {
	return isOperationTypeReadOnly(type);
}

void sb7::Operations::initOperationTypes() {
	optypes.push_back(
		OperationType(SHORT_TRAVERSAL, "SHORT_TRAVERSAL"));
	optypes.push_back(
		OperationType(SHORT_TRAVERSAL_RO, "SHORT_TRAVERSAL_RO"));
	optypes.push_back(OperationType(OPERATION, "OPERATION"));
	optypes.push_back(OperationType(OPERATION_RO, "OPERATION_RO"));
	optypes.push_back(OperationType(STRUCTURAL_MODIFICATION,
		"STRUCTURAL_MODIFICATION"));

	// initialize operation types in raising order of enum values
	if(parameters.areLongTraversalsEnabled()) {
		optypes.push_back(OperationType(TRAVERSAL, "TRAVERSAL"));
		optypes.push_back(OperationType(TRAVERSAL_RO, "TRAVERSAL_RO"));
	}	
}

void sb7::Operations::initOperations(DataHolder *dh) {
	if(parameters.areLongTraversalsEnabled()) {
		ops.push_back(new Traversal1(dh));
		ops.push_back(new Traversal2a(dh));
		ops.push_back(new Traversal2b(dh));
		ops.push_back(new Traversal2c(dh));
		ops.push_back(new Traversal3a(dh));
		ops.push_back(new Traversal3b(dh));
		ops.push_back(new Traversal3c(dh));
		ops.push_back(new Traversal4(dh));
		ops.push_back(new Traversal5(dh));
		ops.push_back(new Traversal6(dh));
		ops.push_back(new Query6(dh));
		ops.push_back(new Query7(dh));
	}

	ops.push_back(new ShortTraversal1(dh));
	ops.push_back(new ShortTraversal2(dh));
	ops.push_back(new Traversal7(dh));
	ops.push_back(new Query4(dh));
	ops.push_back(new Query5(dh));
	ops.push_back(new ShortTraversal6(dh));
	ops.push_back(new ShortTraversal7(dh));
	ops.push_back(new ShortTraversal8(dh));
	ops.push_back(new ShortTraversal9(dh));
	ops.push_back(new ShortTraversal10(dh));

	ops.push_back(new Query1(dh));
	ops.push_back(new Query2(dh));
	ops.push_back(new Query3(dh));
	ops.push_back(new Traversal8(dh));
	ops.push_back(new Traversal9(dh));
	ops.push_back(new Operation6(dh));
	ops.push_back(new Operation7(dh));
	ops.push_back(new Operation8(dh));
	ops.push_back(new Operation9(dh));
	ops.push_back(new Operation10(dh));

	if(parameters.shouldWriteRoot()) {
		ops.push_back(new Operation11(dh));
	}

	ops.push_back(new Operation12(dh));
	ops.push_back(new Operation13(dh));
	ops.push_back(new Operation14(dh));
	ops.push_back(new Operation15(dh));

	ops.push_back(new StructuralModification1(dh));
	ops.push_back(new StructuralModification2(dh));
	ops.push_back(new StructuralModification3(dh));
	ops.push_back(new StructuralModification4(dh));
	ops.push_back(new StructuralModification5(dh));
	ops.push_back(new StructuralModification6(dh));
	ops.push_back(new StructuralModification7(dh));
	ops.push_back(new StructuralModification8(dh));
}

void sb7::Operations::generateCdf() {
	// count number of operations of each type
	for(std::vector<Operation *>::iterator iter = ops.begin();
			iter != ops.end();iter++) {
		Operation *op = *iter;
		optypes[op->type].count++;
	}

	// calculate ratio of each operation type
	double shortTraversalsRatio = parameters.getShortTraversalsRatio() / 100.0;
	double operationsRatio = parameters.getOperationsRatio() / 100.0;
	double traversalsRatio;

	if(parameters.areLongTraversalsEnabled()) {
		traversalsRatio = parameters.getTraversalsRatio() / 100.0;
	}

	double structuralModificationsRatio =
		parameters.getStructuralModificationsRatio() / 100.0;

	double readOnlyOperationsRatio =
		parameters.getReadOnlyOperationsRatio() / 100.0;
	double updateOperationsRatio = 1.0 - readOnlyOperationsRatio;

	double sumRatios = shortTraversalsRatio +
		operationsRatio +
		structuralModificationsRatio * updateOperationsRatio;

	if(parameters.areLongTraversalsEnabled()) {
		sumRatios += traversalsRatio;
	}

	shortTraversalsRatio /= sumRatios;
	operationsRatio /= sumRatios;

	if(parameters.areLongTraversalsEnabled()) {
		traversalsRatio /= sumRatios;
	}

	structuralModificationsRatio /= sumRatios;

	// calculate ratio of each operation belonging to a type
	if(parameters.areLongTraversalsEnabled()) {
		optypes[TRAVERSAL].probability =
			traversalsRatio * updateOperationsRatio /
			optypes[TRAVERSAL].count;

		optypes[TRAVERSAL_RO].probability =
			traversalsRatio * readOnlyOperationsRatio /
			optypes[TRAVERSAL_RO].count;
	}

	optypes[SHORT_TRAVERSAL].probability =
		shortTraversalsRatio * updateOperationsRatio /
		optypes[SHORT_TRAVERSAL].count;
	optypes[SHORT_TRAVERSAL_RO].probability =
		shortTraversalsRatio * readOnlyOperationsRatio /
		optypes[SHORT_TRAVERSAL_RO].count;

	optypes[OPERATION].probability =
		operationsRatio * updateOperationsRatio /
		optypes[OPERATION].count;
	optypes[OPERATION_RO].probability =
		operationsRatio * readOnlyOperationsRatio /
		optypes[OPERATION_RO].count;

	optypes[STRUCTURAL_MODIFICATION].probability =
		structuralModificationsRatio * updateOperationsRatio /
		optypes[STRUCTURAL_MODIFICATION].count;

	// calculate cdf
	double currCdf = 0;

	for(std::vector<Operation *>::iterator iter = ops.begin();
			iter != --ops.end();iter++) {
		Operation *op = *iter;
		currCdf += optypes[op->type].probability;
		cdf.push_back(currCdf);
	}

	cdf.push_back(1.0);
}
