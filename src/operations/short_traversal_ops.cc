#include "short_traversal_ops.h"

#include "../sb7_exception.h"

#include "../struct/connection.h"

/////////////////////
// ShortTraversal1 //
/////////////////////

int sb7::ShortTraversal1::run() const {
	rd_ptr<Module> rd_mod(dataHolder->getModule());
	sh_ptr<ComplexAssembly> sh_designRoot = rd_mod->getDesignRoot();

	return traverse(sh_designRoot);
}

// TODO do this in a nicer way, somehow
int sb7::ShortTraversal1::traverse(sh_ptr<Assembly> assm) const {
	rd_ptr<Assembly> rd_assm(assm);

	if(rd_assm->getType() == assembly_type_complex) {
		return traverse((sh_ptr<ComplexAssembly>)assm);
	} else {
		return traverse((sh_ptr<BaseAssembly>)assm);
	}
}

int sb7::ShortTraversal1::traverse(sh_ptr<ComplexAssembly> cassm) const {
	// choose one sub assembly to traverse
	rd_ptr<ComplexAssembly> rd_cassm(cassm);
	rd_ptr<Set<sh_ptr<Assembly> > > rd_subAssm(rd_cassm->getSubAssemblies());
	int subAssembliesNum = rd_subAssm->size();
	int nextAssm = get_random()->nextInt(subAssembliesNum);

	// find chosen assembly
	int i = 0;
	SetIterator<sh_ptr<Assembly> > iter = rd_subAssm->getIter();

	while(iter.has_next()) {
		sh_ptr<Assembly> sh_subAssm(iter.next());

		if(i == nextAssm) {
			return traverse(sh_subAssm);
		}

		i++;
	}

	throw Sb7Exception(
		"ST1: size of ComplexAssemby.subAssemblies has changed!");
}

int sb7::ShortTraversal1::traverse(sh_ptr<BaseAssembly> bassm) const {
	// choose one component to traverse
	rd_ptr<BaseAssembly> rd_bassm(bassm);
	rd_ptr<Bag<sh_ptr<CompositePart> > > rd_components(
		rd_bassm->getComponents());
	int componentNum = rd_components->size();

	if(componentNum == 0) {
		throw Sb7Exception("ST1: no components in base assembly");
	}

	int nextComp = get_random()->nextInt(componentNum);

	// find chosen component and traverse it
	int i = 0;
	BagIterator<sh_ptr<CompositePart> > iter = rd_components->getIter();

	while(iter.has_next()) {
		sh_ptr<CompositePart> sh_cpart = iter.next();

		if(i == nextComp) {
			return traverse(sh_cpart);
		}

		i++;
	}

	throw Sb7Exception("ST1: size of BaseAssembly.components has changed!");
}

int sb7::ShortTraversal1::traverse(sh_ptr<CompositePart> cpart) const {
	// select one atomic part to traverse
	rd_ptr<CompositePart> rd_cpart(cpart);
	rd_ptr<Set<sh_ptr<AtomicPart> > > rd_parts(rd_cpart->getParts());
	int atomicPartNum = rd_parts->size();
	int nextPart = get_random()->nextInt(atomicPartNum);

	// find part to traverse and traverse it
	SetIterator<sh_ptr<AtomicPart> > iter = rd_parts->getIter();
	int i = 0;

	while(iter.has_next()) {
		sh_ptr<AtomicPart> sh_apart(iter.next());

		if(i == nextPart) {
			return traverse(sh_apart);
		}

		i++;
	}

	throw Sb7Exception("ST1: illegal size of CompositePart.parts!");
}

int sb7::ShortTraversal1::traverse(sh_ptr<AtomicPart> apart) const {
	rd_ptr<AtomicPart> rd_apart(apart);
	return rd_apart->getX() + rd_apart->getY();
}

/////////////////////
// ShortTraversal2 //
/////////////////////

int sb7::ShortTraversal2::traverse(sh_ptr<CompositePart> cpart) const {
	rd_ptr<CompositePart> rd_cpart(cpart);
	return traverse(rd_cpart->getDocumentation());
}

int sb7::ShortTraversal2::traverse(sh_ptr<AtomicPart> apart) const {
	throw Sb7Exception("ST2: unexpected call to traverse(AtomicPart)!");
}

int sb7::ShortTraversal2::traverse(sh_ptr<Document> doc) const {
	rd_ptr<Document> rd_doc(doc);
	return rd_doc->searchText('I');
}

/////////////////////
// ShortTraversal6 //
/////////////////////

int sb7::ShortTraversal6::traverse(sh_ptr<AtomicPart> apart) const {
	wr_ptr<AtomicPart> wr_apart(apart);
	wr_apart->swapXY();
	return ShortTraversal1::traverse(apart);
}

/////////////////////
// ShortTraversal7 //
/////////////////////

#define DOC_START_STRING_1 "I am"
#define DOC_START_STRING_2 "This is"

int sb7::ShortTraversal7::traverse(sh_ptr<Document> doc) const {
	int ret;
	wr_ptr<Document> wr_doc(doc);

	if(wr_doc->textBeginsWith(DOC_START_STRING_1)) {
		ret = wr_doc->replaceText(DOC_START_STRING_1, DOC_START_STRING_2);
	} else if (wr_doc->textBeginsWith(DOC_START_STRING_2)) {
		ret = wr_doc->replaceText(DOC_START_STRING_2, DOC_START_STRING_1);
	} else {
		throw Sb7Exception("ST7: unexpected beginning of Document.text!");
	}

	return ret;
}

/////////////////////
// ShortTraversal8 //
/////////////////////

void sb7::ShortTraversal8::performOperationOnAssembly(
		sh_ptr<Assembly> assm) const {
	wr_ptr<Assembly> wr_assm(assm);
	wr_assm->updateBuildDate();
}

/////////////////////
// ShortTraversal9 //
/////////////////////

int sb7::ShortTraversal9::traverse(sh_ptr<CompositePart> cpart) const {
	Set<sh_ptr<AtomicPart> > visitedParts;
	rd_ptr<CompositePart> rd_cpart(cpart);
	return traverse(rd_cpart->getRootPart(), visitedParts);
}

int sb7::ShortTraversal9::traverse(sh_ptr<AtomicPart> apart,
		Set<sh_ptr<AtomicPart > > &visitedParts) const {
	int ret;

	if(apart == NULL) {
		ret = 0;
	} else if(visitedParts.contains(apart)) {
		ret = 0;
	} else {
		ret = performOperationOnAtomicPart(apart);
		visitedParts.add(apart);
		rd_ptr<AtomicPart> rd_apart(apart);
		rd_ptr<Set<sh_ptr<Connection> > > rd_connections(
			rd_apart->getToConnections());
		SetIterator<sh_ptr<Connection> > iter = rd_connections->getIter();

		while(iter.has_next()) {
			rd_ptr<Connection> rd_conn(iter.next());
			ret += traverse(rd_conn->getDestination(), visitedParts);
		}
	}

	return ret;
}

int sb7::ShortTraversal9::traverse(sh_ptr<AtomicPart> apart) const {
	throw Sb7Exception("ST9: unexpected call to traverse(AtomicPart)!");
}

int sb7::ShortTraversal9::performOperationOnAtomicPart(
		sh_ptr<AtomicPart> apart) const {
	rd_ptr<AtomicPart> rd_apart(apart);
	rd_apart->nullOperation();
	return 1;
}

//////////////////////
// ShortTraversal10 //
// ///////////////////

int sb7::ShortTraversal10::performOperationOnAtomicPart(
		sh_ptr<AtomicPart> apart) const {
	wr_ptr<AtomicPart> wr_apart(apart);
	wr_apart->swapXY();
	return 1;
}
