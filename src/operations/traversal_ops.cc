#include "traversal_ops.h"

#include "../struct/module.h"
#include "../struct/connection.h"
#include "../sb7_exception.h"
#include "../parameters.h"

#ifndef ORIGINAL
stm_op_id_t TRAVERSE_COMPOSITE = STM_INVALID_OPID;
stm_op_id_t UPDATE_BUILD_DATE = STM_INVALID_OPID;
#endif /* ORIGINAL */

////////////////
// Traversal1 //
////////////////

int sb7::Traversal1::run() const {
	rd_ptr<Module> rd_mod(dataHolder->getModule());
	sh_ptr<ComplexAssembly> sh_designRoot = rd_mod->getDesignRoot();

	return traverse(sh_designRoot);
}

int sb7::Traversal1::traverse(sh_ptr<ComplexAssembly> sh_cassm) const {
	int partsVisited = 0;

	rd_ptr<ComplexAssembly> rd_cassm(sh_cassm);
	rd_ptr<Set<sh_ptr<Assembly> > > rd_subAssm(rd_cassm->getSubAssemblies());
	SetIterator<sh_ptr<Assembly> > iter = rd_subAssm->getIter();

	bool childrenAreBase = rd_cassm->areChildrenBaseAssemblies();

	// think about transforming this into a nicer oo design 
	while(iter.has_next()) {
		sh_ptr<Assembly> sh_assm = iter.next();

		if(!childrenAreBase) {
			partsVisited += traverse((sh_ptr<ComplexAssembly>)sh_assm);
		} else {
			partsVisited += traverse((sh_ptr<BaseAssembly>)sh_assm);
		}
	}

	return partsVisited;
}

int sb7::Traversal1::traverse(sh_ptr<BaseAssembly> bassm) const {
	int partsVisited = 0;

	rd_ptr<BaseAssembly> rd_bassm(bassm);
	rd_ptr<Bag<sh_ptr<CompositePart> > > rd_cparts(rd_bassm->getComponents());
	BagIterator<sh_ptr<CompositePart> > iter = rd_cparts->getIter();

	while(iter.has_next()) {
		partsVisited += traverse(iter.next());
	}

	return partsVisited;
}

int sb7::Traversal1::traverse(sh_ptr<CompositePart> cpart) const {
#ifndef ORIGINAL
//	tm_begin_op(TRAVERSE_COMPOSITE, NULL, cpart.getRawHandle());
#endif /* ORIGINAL */
	rd_ptr<CompositePart> rd_cpart(cpart);
	sh_ptr<AtomicPart> sh_rootPart = rd_cpart->getRootPart();
	Set<sh_ptr<AtomicPart> > visitedPartSet;
	int ret = traverse(sh_rootPart, visitedPartSet);
#ifndef ORIGINAL
//	tm_end_op(TRAVERSE_COMPOSITE, &ret);
#endif /* ORIGINAL */
	return ret;
}

int sb7::Traversal1::traverse(sh_ptr<AtomicPart> apart,
		Set<sh_ptr<AtomicPart> > &visitedPartSet) const {
	int ret;

	if(apart == NULL) {
		ret = 0;
	} else if(visitedPartSet.contains(apart)) {
		ret = 0;
	} else {
		ret = performOperationOnAtomicPart(apart, visitedPartSet);
		visitedPartSet.add(apart);

		// visit all connected parts
		rd_ptr<AtomicPart> rd_apart(apart);
		rd_ptr<Set<sh_ptr<Connection> > > rd_toConns(
			rd_apart->getToConnections());
		SetIterator<sh_ptr<Connection> > iter = rd_toConns->getIter();

		while(iter.has_next()) {
			rd_ptr<Connection> rd_conn(iter.next());
			ret += traverse(rd_conn->getDestination(), visitedPartSet);
		}
	}

	return ret;
}

int sb7::Traversal1::performOperationOnAtomicPart(sh_ptr<AtomicPart> apart,
		Set<sh_ptr<AtomicPart> > &visitedPartSet) const {
	rd_ptr<AtomicPart> rd_apart(apart);
	rd_apart->nullOperation();
	return 1;
}

/////////////////
// Traversal2a //
/////////////////

int sb7::Traversal2a::performOperationOnAtomicPart(sh_ptr<AtomicPart> apart,
		Set<sh_ptr<AtomicPart> > &visitedPartSet) const {
	int ret;

	if(visitedPartSet.size() == 0) {
		wr_ptr<AtomicPart> wr_apart(apart);
		wr_apart->swapXY();
		ret = 1;
	} else {
		rd_ptr<AtomicPart> rd_apart(apart);
		rd_apart->nullOperation();
		ret = 1;
	}

	return ret;
}

/////////////////
// Traversal2b //
/////////////////

int sb7::Traversal2b::performOperationOnAtomicPart(sh_ptr<AtomicPart> apart,
		Set<sh_ptr<AtomicPart> > &visitedPartSet) const {
	wr_ptr<AtomicPart> wr_apart(apart);
	wr_apart->swapXY();
	return 1;
}

/////////////////
// Traversal2c //
/////////////////

int sb7::Traversal2c::performOperationOnAtomicPart(sh_ptr<AtomicPart> apart,
		Set<sh_ptr<AtomicPart> > &visitedPartSet) const {
	wr_ptr<AtomicPart> wr_apart(apart);

	wr_apart->swapXY();
	wr_apart->swapXY();
	wr_apart->swapXY();
	wr_apart->swapXY();

	return 4;
}

/////////////////
// Traversal3a //
/////////////////

int sb7::Traversal3a::performOperationOnAtomicPart(sh_ptr<AtomicPart> apart,
		Set<sh_ptr<AtomicPart> > &visitedPartSet) const {
	int ret;

	if(visitedPartSet.size() == 0) {
		updateBuildDate(apart);
		ret = 1;
	} else {
		rd_ptr<AtomicPart> rd_apart(apart);
		rd_apart->nullOperation();
		ret = 1;
	}

	return ret;
}

void sb7::Traversal3a::updateBuildDate(sh_ptr<AtomicPart> apart) const {
#ifndef ORIGINAL
	tm_begin_op(UPDATE_BUILD_DATE, NULL, dataHolder, apart.getRawHandle());
#endif /* ORIGINAL */
	dataHolder->removeAtomicPartFromBuildDateIndex(apart);
	wr_ptr<AtomicPart> wr_apart(apart);
	wr_apart->updateBuildDate();
	dataHolder->addAtomicPartToBuildDateIndex(apart);
#ifndef ORIGINAL
	tm_end_op(UPDATE_BUILD_DATE, NULL);
#endif /* ORIGINAL */
}

/////////////////
// Traversal3b //
/////////////////

int sb7::Traversal3b::performOperationOnAtomicPart(sh_ptr<AtomicPart> apart,
		Set<sh_ptr<AtomicPart> > &visitedPartSet) const {
	updateBuildDate(apart);
	return 1;
}

/////////////////
// Traversal3c //
/////////////////

int sb7::Traversal3c::performOperationOnAtomicPart(sh_ptr<AtomicPart> apart,
		Set<sh_ptr<AtomicPart> > &visitedPartSet) const {
	updateBuildDate(apart);
	updateBuildDate(apart);
	updateBuildDate(apart);
	updateBuildDate(apart);
	return 4;
}

////////////////
// Traversal4 //
////////////////

int sb7::Traversal4::traverse(sh_ptr<CompositePart> cpart) const {
	rd_ptr<CompositePart> rd_cpart(cpart);
	return traverse(rd_cpart->getDocumentation());
}

int sb7::Traversal4::traverse(sh_ptr<Document> doc) const {
	rd_ptr<Document> rd_doc(doc);
	return rd_doc->searchText('I');
}

int sb7::Traversal4::traverse(sh_ptr<AtomicPart> part,
		Set<sh_ptr<AtomicPart> > &setOfVisitedParts) const {
	throw Sb7Exception("T4: traverse(AtomicPart, HashSet<AtomicPart>) called!");
}

int sb7::Traversal4::performOperationOnAtomicPart(sh_ptr<AtomicPart> apart,
		Set<sh_ptr<AtomicPart> > &setOfVisitedPartIds) const {
	throw Sb7Exception("T4: performOperationInAtomicPart(..) called!");
}

////////////////
// Traversal5 //
////////////////

int sb7::Traversal5::traverse(sh_ptr<Document> doc) const {
	int ret;
	wr_ptr<Document> wr_doc(doc);

	if(wr_doc->textBeginsWith("I am")) {
		ret = wr_doc->replaceText("I am", "This is");
	} else if(wr_doc->textBeginsWith("This is")) {
		ret = wr_doc->replaceText("This is", "I am");
	} else {
		throw Sb7Exception(
			("T5: illegal document text: " + wr_doc->getText()).c_str());
	}

	if(!ret) {
		throw Sb7Exception("T5: concurrent modification!");
	}

	return ret;
}

////////////////
// Traversal6 //
////////////////

int sb7::Traversal6::traverse(sh_ptr<CompositePart> cpart) const {
	rd_ptr<CompositePart> rd_cpart(cpart);
	rd_ptr<AtomicPart> rd_rootPart(rd_cpart->getRootPart());
	rd_rootPart->nullOperation();
	return 1;
}

//////////////////////////////////////////////
// Traversal 7 - actually a short traversal //
//////////////////////////////////////////////

int sb7::Traversal7::run() const {
	int apartInd = get_random()->nextInt(parameters.getMaxAtomicParts()) + 1;
	rd_ptr<Map<int, sh_ptr<AtomicPart> > > rd_apartInd(
			dataHolder->getAtomicPartIdIndex());

	Map<int, sh_ptr<AtomicPart> >::Query query;
	query.key = apartInd;
	rd_apartInd->get(query);

	if(!query.found) {
		throw Sb7Exception();
	}

	rd_ptr<AtomicPart> rd_apart(query.val);
	return traverse(rd_apart->getPartOf());
}

int sb7::Traversal7::traverse(sh_ptr<CompositePart> cpart) const {
	Set<sh_ptr<Assembly> > visitedAssemblies;
	int ret = 0;

	rd_ptr<CompositePart> rd_cpart(cpart);
	rd_ptr<Bag<sh_ptr<BaseAssembly> > > rd_bag(rd_cpart->getUsedIn());
	BagIterator<sh_ptr<BaseAssembly> > iter = rd_bag->getIter();

	while(iter.has_next()) {
		sh_ptr<Assembly> sh_assembly(iter.next());
		ret += traverse(sh_assembly, visitedAssemblies);
	}

	return ret;
}

int sb7::Traversal7::traverse(sh_ptr<Assembly> assembly,
		Set<sh_ptr<Assembly> > &visitedAssemblies) const {
	int ret;

	if(assembly == NULL) {
		ret = 0;
	} else if(visitedAssemblies.contains(assembly)) {
		ret = 0;
	} else {
		visitedAssemblies.add(assembly);
		performOperationOnAssembly(assembly);
		rd_ptr<Assembly> rd_assembly(assembly);
    	ret = traverse(rd_assembly->getSuperAssembly(), visitedAssemblies) + 1;
	}

	return ret;
}

void sb7::Traversal7::performOperationOnAssembly(
		sh_ptr<Assembly> assembly) const {
	rd_ptr<Assembly> rd_assembly(assembly);
	rd_assembly->nullOperation();
}

//////////////////////////////////////////
// Traversal8 - actually a ro operation //
//////////////////////////////////////////

int sb7::Traversal8::run() const {
	rd_ptr<Module> rd_mod(dataHolder->getModule());
	return traverse(rd_mod->getManual());
}

int sb7::Traversal8::traverse(sh_ptr<Manual> manual) const {
	rd_ptr<Manual> rd_manual(manual);
	return rd_manual->countOccurences('I');
}

//////////////////////////////////////////
// Traversal9 - actually a ro operation //
//////////////////////////////////////////

int sb7::Traversal9::traverse(sh_ptr<Manual> manual) const {
	rd_ptr<Manual> rd_manual(manual);
	return rd_manual->checkFirstLastCharTheSame();
}

#ifndef ORIGINAL
stm_merge_t traversal_merge(stm_merge_context_t *params) {
	const stm_op_id_t op = stm_get_op_opcode(params->current);

	/* Delayed merge only */
	assert(!STM_SAME_OP(stm_get_current_op(), params->current));

	const stm_union_t *args;
	const ssize_t nargs = stm_get_op_args(params->current, &args);

	if (STM_SAME_OPID(op, TRAVERSE_COMPOSITE)) {
		assert(nargs == 1);
		CompositePart *cp = static_cast<CompositePart *>(args[0].ptr);
		assert(cp);

		assert(params->leaf == 1);
		assert(ENTRY_VALID(params->conflict.entries->e1));
		stm_read_t r = ENTRY_GET_READ(params->conflict.entries->e1);
		assert(STM_VALID_READ(r));
		const uintptr_t tag = stm_get_load_tag(r);

		if (tag == typeid(AtomicPart).hash_code()) {
			AtomicPart *old_ap = NULL, *new_ap = NULL;

			stm_load_value_ptr(r, reinterpret_cast<void **>(&old_ap));
			stm_load_update_ptr(r, reinterpret_cast<void **>(&new_ap));

# ifndef NDEBUG
			printf("\nTRAVERSE_COMPOSITE addr:%p AtomicPart read (old):%p (new):%p\n", params->addr, old_ap, new_ap);
# endif
			if (old_ap->getId() == new_ap->getId())
				return STM_MERGE_OK;
		}
	} else if (STM_SAME_OPID(op, UPDATE_BUILD_DATE)) {
		assert(nargs == 2);
		DataHolder *dh = static_cast<DataHolder *>(args[0].ptr);
		assert(dh);
		Handle<AtomicPart> *h_ap = static_cast<Handle<AtomicPart> *>(args[1].ptr);
		assert(h_ap);
		AtomicPart **ap = h_ap->getRawPtr();
		assert(ap);

		assert(params->leaf == 1);
		assert(ENTRY_VALID(params->conflict.entries->e1));
		stm_read_t r = ENTRY_GET_READ(params->conflict.entries->e1);
		assert(STM_VALID_READ(r));
		const uintptr_t tag = stm_get_load_tag(r);

		auto **dateIndex = dh->getAtomicPartBuildDateIndex().getRawHandle()->getRawPtr();
		/* Conflict is at the global AtomicPartBuildDateIndex */
		if (params->addr == dateIndex && tag == typeid(std::remove_reference<decltype(**dateIndex)>::type).hash_code()) {

			std::remove_reference<decltype(**dateIndex)>::type *old_di = NULL, *new_di = NULL;
			stm_load_value_ptr(r, reinterpret_cast<void **>(&old_di));
			stm_load_update_ptr(r, reinterpret_cast<void **>(&new_di));

			if (old_di == new_di)
				return STM_MERGE_OK;

# ifndef NDEBUG
			printf("\nUPDATE_BUILD_DATE addr:%p DataHolder->m_atomicPartBuildDateIndex read (old):%p (new):%p\n", params->addr, old_di, new_di);
# endif

			stm_read_t rap = stm_did_load(reinterpret_cast<stm_word_t *>(ap));
			assert(STM_VALID_READ(rap));
			AtomicPart *old_ap = NULL;
			stm_load_value_ptr(rap, reinterpret_cast<void **>(&old_ap));

			std::remove_reference<decltype(**dateIndex)>::type::Query old_q, new_q;
			old_q.key = new_q.key = old_ap->getBuildDate();
			old_di->get(old_q);
			new_di->get(new_q);

# ifndef NDEBUG
			printf("UPDATE_BUILD_DATE AtomicPart->m_buildDate:%d DataHolder->m_atomicPartBuildDateIndex query (old):%p (new):%p\n", old_q.key, old_q.val.getRawHandle()->getRawPtr(), new_q.val.getRawHandle()->getRawPtr());
# endif

			/* DateIndex is a shallow copy, only need to check that the key/value pair is unchanged */
			if (old_q.found != new_q.found || old_q.val != new_q.val)
				goto bd_unsup;

			stm_free_t f = stm_did_free(old_di);
			if (STM_VALID_FREE(f)) {
				stm_undo_free(f);
				tx_free(new_di);
			}

			return STM_MERGE_OK;
		/* Conflict is at the AtomicPart pointer */
		} else if (params->addr == ap && tag == typeid(AtomicPart).hash_code()) {

			AtomicPart *old_ap = NULL, *new_ap = NULL;
			stm_load_value_ptr(r, reinterpret_cast<void **>(&old_ap));
			stm_load_update_ptr(r, reinterpret_cast<void **>(&new_ap));

			const stm_write_t w = stm_did_store(reinterpret_cast<stm_word_t *>(ap));
			assert(STM_VALID_WRITE(w));
			AtomicPart *write_ap = NULL;
			stm_store_value_ptr(w, reinterpret_cast<void **>(&write_ap));

			if (old_ap == new_ap)
				return STM_MERGE_OK;

# ifndef NDEBUG
			printf("\nUPDATE_BUILD_DATE addr:%p AtomicPart read (old):%p (new):%p (write):%p\n", params->addr, old_ap, new_ap, write_ap);
# endif

			if (new_ap->getId() != write_ap->getId())
				goto bd_unsup;

			if (new_ap->getX() != write_ap->getX() || new_ap->getY() != write_ap->getY()) {
# ifndef NDEBUG
				printf("UPDATE_BUILD_DATE AtomicPart->m_x read (old):%d (new):%d, AtomicPart->m_y read (old):%d (new):%d\n", old_ap->getX(), new_ap->getX(), old_ap->getY(), new_ap->getY());
# endif

				if (new_ap->getX() == write_ap->getY() && new_ap->getY() == write_ap->getX())
					write_ap->swapXY();
				else
					goto bd_unsup;
			}

			if (old_ap->getBuildDate() != new_ap->getBuildDate()) {
# ifndef NDEBUG
				printf("UPDATE_BUILD_DATE AtomicPart->m_buildDate read (old):%d (new):%d write (old):%d\n", old_ap->getBuildDate(), new_ap->getBuildDate(), write_ap->getBuildDate());
# endif

				/* Alternates between one of two states. If the old and new dates are different, check the written date to determine if the number of state transitions was odd or even. Then, assign the new date, and transition once if odd. */
				if (old_ap->getBuildDate() == new_ap->getBuildDate())
					return STM_MERGE_OK;

				/* Get the DataIndex */
				stm_read_t rdi = stm_did_load(reinterpret_cast<stm_word_t *>(dateIndex));
				assert(STM_VALID_READ(rdi));
				std::remove_reference<decltype(**dateIndex)>::type *di = NULL;
				stm_load_value_ptr(rdi, reinterpret_cast<void **>(&di));

				stm_write_t wdi = stm_did_store(reinterpret_cast<stm_word_t *>(dateIndex));
				if (STM_VALID_WRITE(wdi))
					stm_store_value_ptr(wdi, reinterpret_cast<void **>(&di));

				sh_ptr<AtomicPart> sh_ap(h_ap);
				std::remove_reference<decltype(**dateIndex)>::type::Query q;
				std::remove_reference<decltype(*q.val.getRawHandle()->getRawPtr())>::type s = NULL;

				/* Undo insertion at the stale write key */
				q.key = write_ap->getBuildDate();
				di->get(q);
				assert(q.found);
				stm_write_t write_w = stm_did_store(reinterpret_cast<stm_word_t *>(q.val.getRawHandle()->getRawPtr()));
				if (!STM_VALID_WRITE(write_w))
					return STM_MERGE_ABORT;
				stm_store_value_ptr(write_w, reinterpret_cast<void **>(&s));
				assert(s->contains(sh_ap));
				s->remove(sh_ap);

				/* Update the new date */
				int write = write_ap->getBuildDate();
				write_ap->setBuildDate(new_ap->getBuildDate());
				if (old_ap->getBuildDate() != write)
					write_ap->updateBuildDate();

				/* Insert at the new write key */
				q.key = write_ap->getBuildDate();
				di->get(q);
				assert(q.found);
				stm_write_t new_w = stm_did_store(reinterpret_cast<stm_word_t *>(q.val.getRawHandle()->getRawPtr()));
				if (!STM_VALID_WRITE(new_w))
					return STM_MERGE_ABORT;
				stm_store_value_ptr(new_w, reinterpret_cast<void **>(&s));
				assert(!s->contains(sh_ap));
				s->add(sh_ap);
			}

			stm_free_t f = stm_did_free(old_ap);
			assert(STM_VALID_FREE(f));
			stm_undo_free(f);
			tx_free(new_ap);

			return STM_MERGE_OK;
		/* Conflict is at the Set result of the AtomicPartBuildDateIndex query */
		} else if (tag == typeid(Set<sh_ptr<AtomicPart>>).hash_code()) {

			Set<sh_ptr<AtomicPart>> *old_s = NULL, *new_s = NULL;
			stm_load_value_ptr(r, reinterpret_cast<void **>(&old_s));
			stm_load_update_ptr(r, reinterpret_cast<void **>(&new_s));

			const stm_write_t w = stm_did_store((stm_word_t *)params->addr);
			assert(STM_VALID_WRITE(w));
			Set<sh_ptr<AtomicPart>> *write_s = NULL;
			stm_store_value_ptr(w, reinterpret_cast<void **>(&write_s));

			if (old_s == new_s)
				return STM_MERGE_OK;

# ifndef NDEBUG
			printf("\nUPDATE_BUILD_DATE addr:%p Set read (old):%p (new):%p (write):%p\n", params->addr, old_s, new_s, write_s);
# endif

return STM_MERGE_UNSUPPORTED;
			if (old_s->get() == new_s->get())
				return STM_MERGE_OK;
			else if (old_s->get() == write_s->get()) {
				write_s->assign(new_s->get());
				return STM_MERGE_OK;
			}

			stm_free_t f = stm_did_free(old_s);
			assert(STM_VALID_FREE(f));
			stm_undo_free(f);
			tx_free(new_s);
		}

bd_unsup:;
# ifndef NDEBUG
		printf("\nUPDATE_BUILD_DATE UNSUPPORTED\n");
# endif
	}

# ifndef NDEBUG
	printf("\nTRAVERSAL_MERGE UNSUPPORTED addr:%p\n", params->addr);
# endif
	return STM_MERGE_UNSUPPORTED;
}

__attribute__((constructor)) void traversal_init() {
	static ffi_type *argp[] = { &ffi_type_pointer };
	static ffi_type *argpp[] = { &ffi_type_pointer, &ffi_type_pointer };
	static ffi_type reti = ffi_type_sint;
	static ffi_type retv = ffi_type_void;

	stm_merge_policy_t policy[2] = { STM_MERGE_POLICY_FUNCTION, STM_MERGE_POLICY_FUNCTION };

	TRAVERSE_COMPOSITE = sb7::tm_new_op("TRAVERSE_COMPOSITE", &reti, argpp, sizeof(argp) / sizeof(*argp), NULL, traversal_merge, policy);
	UPDATE_BUILD_DATE = sb7::tm_new_op("UPDATE_BUILD_DATE", &retv, argpp, sizeof(argpp) / sizeof(*argpp), NULL, traversal_merge, policy);
}
#endif /* ORIGINAL */
