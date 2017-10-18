#include "structural_modification_ops.h"

#include "../parameters.h"
#include "../sb7_exception.h"

/////////////////////////////
// StructuralModification1 //
/////////////////////////////

int sb7::StructuralModification1::run() const {
	dataHolder->createCompositePart();
	return 0;
}

/////////////////////////////
// StructuralModification2 //
/////////////////////////////

int sb7::StructuralModification2::run() const {
	// generate random composite part id and try to look it up
	int cpartId = get_random()->nextInt(parameters.getMaxCompParts()) + 1;
	sh_ptr<CompositePart> cpart = dataHolder->getCompositePart(cpartId);

	if(cpart == NULL) {
		throw Sb7Exception();
	}

	dataHolder->deleteCompositePart(cpart);

	return 0;
}

/////////////////////////////
// StructuralModification3 //
/////////////////////////////

int sb7::StructuralModification3::run() const {
	// generate random composite part id
	int cpartId = get_random()->nextInt(parameters.getMaxCompParts()) + 1;
	sh_ptr<CompositePart> cpart = dataHolder->getCompositePart(cpartId);

	if(cpart == NULL) {
		throw Sb7Exception();
	}

	// generate random base assembly id
	int bassmId = get_random()->nextInt(parameters.getMaxBaseAssemblies()) + 1;
	sh_ptr<BaseAssembly> bassm = dataHolder->getBaseAssembly(bassmId);

	if(bassm == NULL) {
		throw Sb7Exception();
	}	

	rd_ptr<BaseAssembly> rd_bassm(bassm);
	rd_bassm->addComponent(cpart);

	return 0;
}

/////////////////////////////
// StructuralModification4 //
/////////////////////////////

int sb7::StructuralModification4::run() const {
	// generate random base assembly id
	int bassmId = get_random()->nextInt(parameters.getMaxBaseAssemblies()) + 1;
	sh_ptr<BaseAssembly> bassm = dataHolder->getBaseAssembly(bassmId);

	if(bassm == NULL) {
		throw Sb7Exception();
	}	

	// select one of composite parts used in the base assembly
	rd_ptr<BaseAssembly> rd_bassm(bassm);
	rd_ptr<Bag<sh_ptr<CompositePart> > > rd_cpartBag(
		rd_bassm->getComponents());
	int compNum = rd_cpartBag->size();

	if(compNum == 0) {
		throw Sb7Exception();
	}

	int nextId = get_random()->nextInt(compNum);

	// find component with that ordinal number
	BagIterator<sh_ptr<CompositePart> > iter = rd_cpartBag->getIter();
	int i = 0;

	while(iter.has_next()) {
		sh_ptr<CompositePart> cpart = iter.next();

		if(nextId == i) {
			wr_ptr<BaseAssembly> wr_bassm(bassm);
			wr_bassm->removeComponent(cpart);
			return 0;
		}

		i++;
	}

	throw Sb7Exception(
		"SM4: concurrent modification of BaseAssembly.components!");
}

/////////////////////////////
// StructuralModification5 //
/////////////////////////////

int sb7::StructuralModification5::run() const {
	// generate random base assembly id
	int bassmId = get_random()->nextInt(parameters.getMaxBaseAssemblies()) + 1;
	sh_ptr<BaseAssembly> bassm = dataHolder->getBaseAssembly(bassmId);

	if(bassm == NULL) {
		throw Sb7Exception();
	}	

	// create sibling base assembly
	rd_ptr<BaseAssembly> rd_bassm(bassm);
	dataHolder->createBaseAssembly(rd_bassm->getSuperAssembly());

	return 0;
}

/////////////////////////////
// StructuralModification6 //
/////////////////////////////

int sb7::StructuralModification6::run() const {
	// generate random base assembly id
	int bassmId = get_random()->nextInt(parameters.getMaxBaseAssemblies()) + 1;
	sh_ptr<BaseAssembly> bassm = dataHolder->getBaseAssembly(bassmId);

	if(bassm == NULL) {
		throw Sb7Exception();
	}	

	// get parent and check that it has at least one more child
	rd_ptr<BaseAssembly> rd_bassm(bassm);
	rd_ptr<ComplexAssembly> rd_cassm(rd_bassm->getSuperAssembly());
	rd_ptr<Set<sh_ptr<BaseAssembly> > > rd_subAssmSet(
		rd_cassm->getSubAssemblies());

	// don't let the tree break
	if(rd_subAssmSet->size() == 1) {
		throw Sb7Exception();
	}

	dataHolder->deleteBaseAssembly(bassm);

	return 0;
}

/////////////////////////////
// StructuralModification7 //
/////////////////////////////

int sb7::StructuralModification7::run() const {
	// generate random complex assembly id
	int cassmId = get_random()->nextInt(
		parameters.getMaxComplexAssemblies()) + 1;
	sh_ptr<ComplexAssembly> cassm = dataHolder->getComplexAssembly(cassmId);

	if(cassm == NULL) {
		throw Sb7Exception();
	}

	// create sub assembly
	dataHolder->createSubAssembly(cassm, parameters.getNumAssmPerAssm());

	return 1;
}

/////////////////////////////
// StructuralModification8 //
/////////////////////////////

int sb7::StructuralModification8::run() const {
	// generate random complex assembly id
	int cassmId = get_random()->nextInt(
		parameters.getMaxComplexAssemblies()) + 1;
	sh_ptr<ComplexAssembly> cassm = dataHolder->getComplexAssembly(cassmId);

	if(cassm == NULL) {
		throw Sb7Exception();
	}

	// get super assembly
	rd_ptr<ComplexAssembly> rd_cassm(cassm);
	sh_ptr<ComplexAssembly> sh_superAssm(rd_cassm->getSuperAssembly());

	// don't continue if we got root complex assembly
	if(sh_superAssm == NULL) {
		throw Sb7Exception();
	}

	// check if this would break the tree structure
	rd_ptr<ComplexAssembly> rd_superAssm(sh_superAssm);
	rd_ptr<Set<sh_ptr<Assembly> > > rd_assmSet(
		rd_superAssm->getSubAssemblies());

	if(rd_assmSet->size() == 1) {
		throw Sb7Exception();
	}

	// delete selected complex assembly
	dataHolder->deleteComplexAssembly(cassm);

	return 1;
}
