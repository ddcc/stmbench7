#ifndef SB7_OPERATIONS_H
#define SB7_OPERATIONS_H

#include <vector>

#include "../data_holder.h"
#include "../random.h"

// define number of operation types as it will be required in a few places
#define OPERATION_TYPE_NUM 7

namespace sb7 {

	enum optype {
		SHORT_TRAVERSAL = 0,
		SHORT_TRAVERSAL_RO,
		OPERATION,
		OPERATION_RO,
		STRUCTURAL_MODIFICATION,
		TRAVERSAL,
		TRAVERSAL_RO
	};

	struct OperationType {
		OperationType(enum optype ty, const char *n) :
			type(ty),
			count(0),
			probability(0.0),
			success(0),
			aborted(0),
			failure(0),
			maxttc(0),
			name(n) {
		}
			
		enum optype type;
		int count;
		double probability;
		int success;
		int aborted;
		int failure;
		int maxttc;
		const char *name;
	};

	// abstract class representing operations
	// all operations are stateless
	class Operation {
		public:
			Operation(enum optype ty, const char *n,
					DataHolder *dh = NULL)
				: type(ty), name(n), dataHolder(dh) { }

			virtual ~Operation() { }

			virtual int run() const = 0;

			bool isReadOnly() const;

		public:
			// TODO or use pointer to operation type object
			const enum optype type;

			const char *name;

		protected:
			DataHolder *dataHolder;
	};

	class Operations {
		public:
			Operations(DataHolder *dh) {
				initOperationTypes();
				initOperations(dh);
				generateCdf();
			}

		public:
			int size() const {
				return ops.size();
			}

			std::vector<OperationType> &getOperationTypes() {
				return optypes;
			}

			const std::vector<Operation *> &getOperations() const {
				return ops;
			}

			const std::vector<double> &getOperationCdf() const {
				return cdf;
			}

		private:
			void initOperationTypes();

			void initOperations(DataHolder *dh);

			void generateCdf();

		private:
			std::vector<OperationType> optypes;

			std::vector<Operation *> ops;

			std::vector<double> cdf;
	};
}

#endif
