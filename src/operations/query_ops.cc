#include "query_ops.h"

#include "../helpers.h"
#include "../containers.h"
#include "../parameters.h"
#include "../struct/assembly.h"

////////////
// Query1 //
////////////

#define QUERY1_ITER 10

int sb7::Query1::run() const {
	int count = 0;
		
	for(int i = 0; i < QUERY1_ITER; i++) {
		int apartId = get_random()->nextInt(
			parameters.getMaxAtomicParts()) + 1;

		rd_ptr<Map<int, sh_ptr<AtomicPart> > > rd_apartInd(
			dataHolder->getAtomicPartIdIndex());

		Map<int, sh_ptr<AtomicPart> >::Query query;
		query.key = apartId;
		rd_apartInd->get(query);

		if(query.found) {
			performOperationOnAtomicPart(query.val);
			count++;
		}
	}

	return count;
}

void sb7::Query1::performOperationOnAtomicPart(
		sh_ptr<AtomicPart> apart) const {
	rd_ptr<AtomicPart> rd_apart(apart);
	rd_apart->nullOperation();
}

////////////
// Query2 //
////////////

sb7::Query2::Query2(DataHolder *dh, optype t, const char *n, int percent)
		: Operation(t, n, dh) {
	maxAtomicDate = parameters.getMaxAtomicDate();
	minAtomicDate = parameters.getMaxAtomicDate() -
		percent * (parameters.getMaxAtomicDate() -
					parameters.getMinAtomicDate()) / 100;
}

int sb7::Query2::run() const {
	int count = 0;
	rd_ptr<Map<int, sh_ptr<Set<sh_ptr<AtomicPart> > > > > rd_setInd(
		dataHolder->getAtomicPartBuildDateIndex());
	MapIterator<int, sh_ptr<Set<sh_ptr<AtomicPart> > > > iter =
		rd_setInd->getRange(minAtomicDate, maxAtomicDate);

	while(iter.has_next()) {
		rd_ptr<Set<sh_ptr<AtomicPart> > > rd_apartSet(iter.next());
		SetIterator<sh_ptr<AtomicPart> > apartIter = rd_apartSet->getIter();

		while(apartIter.has_next()) {
			sh_ptr<AtomicPart> apart = apartIter.next();
			performOperationOnAtomicPart(apart);
			count++;
		}
	}

	return count;
}

void sb7::Query2::performOperationOnAtomicPart(
		sh_ptr<AtomicPart> apart) const {
	rd_ptr<AtomicPart> rd_apart(apart);
	rd_apart->nullOperation();
}

////////////
// Query4 //
////////////

#define QUERY4_ITER 100

int sb7::Query4::run() const {
	int ret = 0;

	for(int i = 0;i < QUERY4_ITER;i++) {
		// construct name of documentation for composite part
		int partId = get_random()->nextInt(parameters.getMaxCompParts()) + 1;
		// TODO move all these constants to separate header file
		ITOA(itoa_buf, partId);
		string title = "Composite Part #" + (string)itoa_buf;

		// search for document with that name
		rd_ptr<Map<string, sh_ptr<Document> > > rd_docInd(
			dataHolder->getDocumentTitleIndex());
		Map<string, sh_ptr<Document> >::Query query;
		query.key = title;
		rd_docInd->get(query);

		if(query.found) {
			rd_ptr<Document> rd_doc(query.val);
			rd_ptr<CompositePart> rd_cpart(rd_doc->getCompositePart());
			rd_ptr<Bag<sh_ptr<BaseAssembly> > > rd_usedIn(
				rd_cpart->getUsedIn());
			BagIterator<sh_ptr<BaseAssembly> > iter = rd_usedIn->getIter();

			while(iter.has_next()) {
				rd_ptr<BaseAssembly> rd_bassm(iter.next());
				rd_bassm->nullOperation();
				ret++;
			}
		}
	}

	return ret;
}

////////////
// Query5 //
////////////

int sb7::Query5::run() const {
	int ret = 0;

	rd_ptr<Map<int, sh_ptr<BaseAssembly> > > rd_bassmInd(
			dataHolder->getBaseAssemblyIdIndex());
	MapIterator<int, sh_ptr<BaseAssembly> > iter = rd_bassmInd->getAll();

	while(iter.has_next()) {
		ret += checkBaseAssembly(iter.next());
	} 

	return ret;
}

int sb7::Query5::checkBaseAssembly(sh_ptr<BaseAssembly> bassm) const {
	rd_ptr<BaseAssembly> rd_bassm(bassm);
	int assmBuildDate = rd_bassm->getBuildDate();
	rd_ptr<Bag<sh_ptr<CompositePart> > > rd_cparts(rd_bassm->getComponents());
	BagIterator<sh_ptr<CompositePart> > iter = rd_cparts->getIter();

	while(iter.has_next()) {
		rd_ptr<CompositePart> rd_cpart(iter.next());

		if(rd_cpart->getBuildDate() > assmBuildDate) {
			rd_bassm->nullOperation();
			return 1;
		}
	}

	return 0;
}

////////////
// Query6 //
////////////

int sb7::Query6::run() const {
	rd_ptr<Module> rd_module(dataHolder->getModule());
	return checkComplexAssembly(rd_module->getDesignRoot());
}

int sb7::Query6::checkAssembly(sh_ptr<Assembly> assembly) const {
	rd_ptr<Assembly> rd_assm(assembly);

	if(rd_assm->getType() == assembly_type_complex) {
		return checkComplexAssembly(assembly);
	} else {
		return checkBaseAssembly(assembly);
	}
}

int sb7::Query6::checkComplexAssembly(sh_ptr<ComplexAssembly> assembly) const {
	int ret = 0;

	rd_ptr<ComplexAssembly> rd_assembly(assembly);
	rd_ptr<Set<sh_ptr<Assembly> > > rd_subAssmSet(
		rd_assembly->getSubAssemblies());
	SetIterator<sh_ptr<Assembly> > iter = rd_subAssmSet->getIter();

	while(iter.has_next()) {
		ret += checkAssembly(iter.next());
	}

	if(ret) {
		rd_assembly->nullOperation();
		ret++;
	}

	return ret;
}

////////////
// Query7 //
////////////

int sb7::Query7::run() const {
	int ret = 0;

	rd_ptr<Map<int, sh_ptr<AtomicPart> > > rd_apartInd(
		dataHolder->getAtomicPartIdIndex());
	MapIterator<int, sh_ptr<AtomicPart> > iter = rd_apartInd->getAll();

	while(iter.has_next()) {
		rd_ptr<AtomicPart> rd_apart(iter.next());
		rd_apart->nullOperation();
		ret++;
	}

	return ret;
}
