#include "operation_ops.h"

#include "../parameters.h"
#include "../sb7_exception.h"

#ifndef ORIGINAL
stm_op_id_t OP11 = STM_INVALID_OPID;
#endif /* ORIGINAL */

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
#ifndef ORIGINAL
	tm_begin_op(OP11, NULL, manual.getRawHandle()->getRawPtr());
#endif /* ORIGINAL */

	wr_ptr<Manual> wr_man(manual);
	int ret;

	if(wr_man->startsWith(MANUAL_TEXT_START_1)) {
		ret = wr_man->replaceChar(MANUAL_TEXT_START_1, MANUAL_TEXT_START_2);
	} else if(wr_man->startsWith(MANUAL_TEXT_START_2)) {
		ret = wr_man->replaceChar(MANUAL_TEXT_START_2, MANUAL_TEXT_START_1);
	} else {
		throw Sb7Exception("OP11: unexpected Manual.text!");
	}

#ifndef ORIGINAL
	tm_end_op(OP11, &ret);
#endif /* ORIGINAL */
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

#ifndef ORIGINAL
stm_merge_t ops_merge(stm_merge_context_t *params) {
	const stm_op_id_t op = stm_get_op_opcode(params->current);

	/* Delayed merge only */
	assert(!STM_SAME_OP(stm_get_current_op(), params->current));

	const stm_union_t *args;
	const ssize_t nargs = stm_get_op_args(params->current, &args);

	if (STM_SAME_OPID(op, OP11)) {
		assert(nargs == 1);
		const Manual **sh = static_cast<const Manual**>(args[0].ptr);
		assert(sh);

		assert(params->leaf == 1);
		assert(ENTRY_VALID(params->conflict.entries->e1));
		stm_read_t r = ENTRY_GET_READ(params->conflict.entries->e1);
		assert(STM_VALID_READ(r));
		const uintptr_t tag = stm_get_load_tag(r);

		/* Conflict is at the Manual pointer */
		if (params->addr == sh && tag == typeid(Manual).hash_code()) {
			Manual *old_sh = NULL, *new_sh = NULL;
			stm_load_value_ptr(r, reinterpret_cast<void **>(&old_sh));
			stm_load_update_ptr(r, reinterpret_cast<void **>(&new_sh));

			const stm_write_t w = stm_did_store(reinterpret_cast<stm_word_t *>(sh));
			assert(STM_VALID_WRITE(w));
			Manual *write_sh = NULL;
			stm_store_value_ptr(w, reinterpret_cast<void **>(&write_sh));

# ifndef NDEBUG
			printf("\nOP11 addr:%p Manual read (old):%p (new):%p (write):%p\n", params->addr, old_sh, new_sh, write_sh);
# endif

			if (!new_sh->isCompatible(*write_sh))
				return STM_MERGE_UNSUPPORTED;

			const sb7::string &oldText = old_sh->getText();
			const sb7::string &newText = new_sh->getText();

			/* Alternates between one of two states. If the old and new texts are in different states, assign back our previous state, instead of incurring the transition penalty. */
			if (oldText[0] != newText[0])
				write_sh->assignText(old_sh->getText());

			return STM_MERGE_OK;
		}
	}

# ifndef NDEBUG
	printf("\nOPS_MERGE UNSUPPORTED addr:%p\n", params->addr);
# endif
	return STM_MERGE_UNSUPPORTED;
}

__attribute__((constructor)) void ops_init() {
	static ffi_type *argp[] = { &ffi_type_pointer };
	static ffi_type rets = ffi_type_sint;

	stm_merge_policy_t policy[2] = { STM_MERGE_POLICY_MANUAL, STM_MERGE_POLICY_UNSUPPORTED };

	OP11 = sb7::tm_new_op("OP11", &rets, argp, sizeof(argp) / sizeof(*argp), NULL, ops_merge, policy);
}
#endif /* ORIGINAL */
