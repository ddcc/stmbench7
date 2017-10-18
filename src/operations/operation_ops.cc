#include "operation_ops.h"

#include "../parameters.h"
#include "../sb7_exception.h"

////////////////
// Operation6 //
////////////////

int sb7::Operation6::run() const {
	// Generate one random number that is in range of possible complex assembly
	// identifiers. It is used to look up complex assembly.
	int cassmId = get_random()->nextInt(
		parameters.getMaxComplexAssemblies()) + 1;

	// lookup complex assembly using complex assembly index
	rd_ptr<Map<int, sh_ptr<ComplexAssembly> > > rd_cassmInd(
		dataHolder->getComplexAssemblyIdIndex());
	Map<int, sh_ptr<ComplexAssembly> >::Query query;
	query.key = cassmId;
	rd_cassmInd->get(query);

	// If complex assembly is not found throw an exception.
	// This is an easy way to get out of the transaction.
	if(!query.found) {
		throw Sb7Exception();
	}

	int ret;

	// if complex assembly was found process it
	sh_ptr<ComplexAssembly> sh_cassm(query.val);
	rd_ptr<ComplexAssembly> rd_cassm(sh_cassm);
	sh_ptr<ComplexAssembly> sh_superAssm(rd_cassm->getSuperAssembly());

	// if this assembly is root perform operation on it
	if(sh_superAssm == NULL) {
		performOperationOnComplexAssembly(sh_cassm);
		ret = 1;
	} else {
		// else perform operation on all it's siblings (including itself)
		rd_ptr<ComplexAssembly> rd_superAssm(sh_superAssm);
		rd_ptr<Set<sh_ptr<Assembly> > > rd_siblingAssms(
			rd_superAssm->getSubAssemblies());
		SetIterator<sh_ptr<Assembly> > iter = rd_siblingAssms->getIter();
		ret = 0;

		while(iter.has_next()) {
			sh_ptr<ComplexAssembly> sh_cassmSib(iter.next());
			performOperationOnComplexAssembly(sh_cassmSib);
			ret++;
		}
	}

	return ret;
}

void sb7::Operation6::performOperationOnComplexAssembly(
		sh_ptr<ComplexAssembly> cassm) const {
	rd_ptr<ComplexAssembly> rd_cassm(cassm);
	rd_cassm->nullOperation();
}

////////////////
// Operation7 //
////////////////

int sb7::Operation7::run() const {
	// Generate one random number that is in range of possible base assembly
	// identifiers. It is used to look up base assembly from index.
	int bassmId = get_random()->nextInt(parameters.getMaxBaseAssemblies()) + 1;
	
	// lookup base assembly using base assembly index
	rd_ptr<Map<int, sh_ptr<BaseAssembly> > > rd_bassmInd(
		dataHolder->getBaseAssemblyIdIndex());
	Map<int, sh_ptr<BaseAssembly> >::Query query;
	query.key = bassmId;
	rd_bassmInd->get(query);

	if(!query.found) {
		throw Sb7Exception();
	}

	// process all sibling base assemblies
	rd_ptr<BaseAssembly> rd_bassm(query.val);
	rd_ptr<ComplexAssembly> rd_superAssm(rd_bassm->getSuperAssembly());
	rd_ptr<Set<sh_ptr<Assembly> > > rd_siblingSet(
		rd_superAssm->getSubAssemblies());
	SetIterator<sh_ptr<Assembly> > iter = rd_siblingSet->getIter();
	int ret = 0;

	while(iter.has_next()) {
		performOperationOnBaseAssembly((sh_ptr<BaseAssembly>)iter.next());
		ret++;
	}

	return ret;
}

void sb7::Operation7::performOperationOnBaseAssembly(
		sh_ptr<BaseAssembly> bassm) const {
	rd_ptr<BaseAssembly> rd_bassm(bassm);
	rd_bassm->nullOperation();
}

////////////////
// Operation8 //
////////////////

int sb7::Operation8::run() const {
	// Generate one random number that is in range of possible base assembly
	// identifiers. It is used to look up base assembly from index.
	int bassmId = get_random()->nextInt(
		parameters.getMaxBaseAssemblies()) + 1;
	
	// lookup base assembly using base assembly index
	rd_ptr<Map<int, sh_ptr<BaseAssembly> > > rd_bassmInd(
		dataHolder->getBaseAssemblyIdIndex());
	Map<int, sh_ptr<BaseAssembly> >::Query query;
	query.key = bassmId;
	rd_bassmInd->get(query);

	if(!query.found) {
		throw Sb7Exception();
	}

	rd_ptr<BaseAssembly> rd_bassm(query.val);
	rd_ptr<Bag<sh_ptr<CompositePart> > > rd_componentBag(
		rd_bassm->getComponents());
	BagIterator<sh_ptr<CompositePart> > iter = rd_componentBag->getIter();
	int ret = 0;

	while(iter.has_next()) {
		performOperationOnComponent(iter.next());
		ret++;
	}

	return ret;
}

void sb7::Operation8::performOperationOnComponent(
		sh_ptr<CompositePart> comp) const {
	rd_ptr<CompositePart> rd_comp(comp);
	rd_comp->nullOperation();
}

////////////////
// Operation9 //
////////////////

void sb7::Operation9::performOperationOnAtomicPart(
		sh_ptr<AtomicPart> apart) const {
	wr_ptr<AtomicPart> wr_apart(apart);
	wr_apart->swapXY();
}

////////////////
// Operation10 //
////////////////

void sb7::Operation10::performOperationOnAtomicPart(
		sh_ptr<AtomicPart> apart) const {
	wr_ptr<AtomicPart> wr_apart(apart);
	wr_apart->swapXY();
}

/////////////////
// Operation11 //
/////////////////

#define MANUAL_TEXT_START_1 'I'
#define MANUAL_TEXT_START_2 'i'

#include <iostream>

int sb7::Operation11::traverse(sh_ptr<Manual> manual) const {
	wr_ptr<Manual> wr_man(manual);
	int ret;

	if(wr_man->startsWith(MANUAL_TEXT_START_1)) {
		ret = wr_man->replaceChar(MANUAL_TEXT_START_1, MANUAL_TEXT_START_2);
	} else if(wr_man->startsWith(MANUAL_TEXT_START_2)) {
		ret = wr_man->replaceChar(MANUAL_TEXT_START_2, MANUAL_TEXT_START_1);
	} else {
		throw Sb7Exception("OP11: unexpected Manual.text!");
	}

	return ret;
}

/////////////////
// Operation12 //
/////////////////

void sb7::Operation12::performOperationOnComplexAssembly(
		sh_ptr<ComplexAssembly> cassm) const {
	wr_ptr<ComplexAssembly> wr_cassm(cassm);
	wr_cassm->updateBuildDate();
}

/////////////////
// Operation13 //
/////////////////

void sb7::Operation13::performOperationOnBaseAssembly(
		sh_ptr<BaseAssembly> bassm) const {
	wr_ptr<BaseAssembly> wr_bassm(bassm);
	wr_bassm->updateBuildDate();
}

/////////////////
// Operation14 //
/////////////////

void sb7::Operation14::performOperationOnComponent(
		sh_ptr<CompositePart> cpart) const {
	wr_ptr<CompositePart> wr_cpart(cpart);
	wr_cpart->updateBuildDate();
}

/////////////////
// Operation15 //
/////////////////

void sb7::Operation15::performOperationOnAtomicPart(
		sh_ptr<AtomicPart> apart) const {
	dataHolder->removeAtomicPartFromBuildDateIndex(apart);

	wr_ptr<AtomicPart> wr_apart(apart);
	wr_apart->updateBuildDate();

	dataHolder->addAtomicPartToBuildDateIndex(apart);
}
