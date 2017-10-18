#ifndef SB7_PARAMETERS_H_
#define SB7_PARAMETERS_H_

#include <cmath>
#include <iostream>

namespace sb7 {

	class ConfigParameters;

	/**
	 * These are final parameters for the benchmark.
	 */
	class Parameters {
		protected:
			static const int DEFAULT_NUM_ATOMIC_PER_COMP;
			static const int DEFAULT_NUM_CONN_PER_ATOMIC;
			static const int DEFAULT_DOCUMENT_SIZE;
			static const int DEFAULT_MANUAL_SIZE;
			static const int DEFAULT_NUM_COMP_PER_MODULE;
			static const int DEFAULT_NUM_ASSM_PER_ASSM;
			static const int DEFAULT_NUM_ASSM_LEVELS;
			static const int DEFAULT_NUM_COMP_PER_ASSM;
			static const int DEFAULT_NUM_MODULES;

			static const int DEFAULT_MIN_MODULE_DATE;
			static const int DEFAULT_MAX_MODULE_DATE;
			static const int DEFAULT_MIN_ASSM_DATE;
			static const int DEFAULT_MAX_ASSM_DATE;
			static const int DEFAULT_MIN_ATOMIC_DATE;
			static const int DEFAULT_MAX_ATOMIC_DATE;
			static const int DEFAULT_MIN_OLD_COMP_DATE;
			static const int DEFAULT_MAX_OLD_COMP_DATE;
			static const int DEFAULT_MIN_YOUNG_COMP_DATE;
			static const int DEFAULT_MAX_YOUNG_COMP_DATE;
			static const int DEFAULT_YOUNG_COMP_FRAC;
			static const int DEFAULT_TYPE_SIZE;
			static const int DEFAULT_NUM_TYPES;
			static const int DEFAULT_XY_RANGE;
			static const int DEFAULT_TITLE_SIZE;

			static const int DEFAULT_TRAVERSAL_RATIO;
			static const int DEFAULT_SHORT_TRAVERSAL_RATIO;
			static const int DEFAULT_OPERATIONS_RATIO;
			static const int DEFAULT_STRUCTURAL_MODIFICATIONS_RATIO;

			static const int DEFAULT_READ_ONLY_OPERATIONS_RATIO;

			static const int DEFAULT_THREAD_NUM;
			static const int DEFAULT_EXPERIMENT_LENGTH_MS;

			//Parameters of the output TTC histograms.
			static const int DEFAULT_MAX_LOW_TTC;
			static const int DEFAULT_HIGH_TTC_ENTRIES;
			static const double DEFAULT_HIGH_TTC_LOG_BASE;

			static const bool DEFAULT_STRUCTURE_MODIFICATION_ENABLED;
			static const bool DEFAULT_LONG_TRAVERSALS_ENABLED;
			static const bool DEFAULT_REPORT_TTC_HISTOGRAMS;

			static const int DEFAULT_VERBOSE_LEVEL;
			static const char *DEFAULT_FILE_NAME;

			static const bool DEFAULT_HINT_RO;
			static const bool DEFAULT_WRITE_ROOT;
			static const bool DEFAULT_INIT_SINGLE_TX;

			static const double MAX_TO_INITIAL_RATIO;

		protected:
			int numAtomicPerComp;
			int numConnPerAtomic;
			int documentSize;
			int manualSize;
			int numCompPerModule;
			int numAssmPerAssm;
			int numAssmLevels;
			int numCompPerAssm;
			int numModules;

			int initialTotalCompParts;
			int initialTotalBaseAssemblies;
			int initialTotalComplexAssemblies;

			int maxCompParts;
			int maxAtomicParts;
			int maxBaseAssemblies;
			int maxComplexAssemblies;

			int minModuleDate;
			int maxModuleDate;
			int minAssmDate;
			int maxAssmDate;
			int minAtomicDate;
			int maxAtomicDate;
			int minOldCompDate;
			int maxOldCompDate;
			int minYoungCompDate;
			int maxYoungCompDate;
			int youngCompFrac;
			int typeSize;
			int numTypes;
			int xyRange;
			int titleSize;

			int traversalRatio;
			int shortTraversalRatio;
			int operationsRatio;
			int structuralModificationsRatio;

			int readOnlyOperationsRatio;

			int threadNum;
			int experimentLengthMs;

			//Parameters of the output TTC histograms.
			int maxLowTtc;
			int highTtcEntries;
			double highTtcLogBase;

			bool structureModificationEnabled;
			bool longTraversalsEnabled;
			bool reportTtcHistograms;

			int verboseLevel;
			std::string fileName;

			bool hintRo;
			bool writeRoot;
			bool initSingleTx;

		public:
			Parameters();

		//////////////////////////////////////////////////////////////
		// getters are public as they are used to access parameters //
		//////////////////////////////////////////////////////////////

		public:
			int getNumAtomicPerComp() const {
				return numAtomicPerComp;
			}

			int getNumConnPerAtomic() const {
				return numConnPerAtomic;
			}

			int getDocumentSize() const {
				return documentSize;
			}

			int getManualSize() const {
				return manualSize;
			}

			int getNumCompPerModule() const {
				return numCompPerModule;
			}

			int getNumAssmPerAssm() const {
				return numAssmPerAssm;
			}

			int getNumAssmLevels() const {
				return numAssmLevels;
			}

			int getNumCompPerAssm() const {
				return numCompPerAssm;
			}

			int getNumModules() const {
				return numModules;
			}

			int getInitialTotalCompParts() const {
				return initialTotalCompParts;
			}

			int getInitialTotalBaseAssemblies() const {
				return initialTotalBaseAssemblies;
			}

			int getInitialTotalComplexAssemblies() const {
				return initialTotalComplexAssemblies;
			}

			int getMaxCompParts() const {
				return maxCompParts;
			}

			int getMaxAtomicParts() const {
				return maxAtomicParts;
			}

			int getMaxBaseAssemblies() const {
				return maxBaseAssemblies;
			}

			int getMaxComplexAssemblies() const {
				return maxComplexAssemblies;
			}

			int getMinModuleDate() const {
				return minModuleDate;
			}

			int getMaxModuleDate() const {
				return maxModuleDate;
			}

			int getMinAssmDate() const {
				return minAssmDate;
			}

			int getMaxAssmDate() const {
				return maxAssmDate;
			}

			int getMinAtomicDate() const {
				return minAtomicDate;
			}

			int getMaxAtomicDate() const {
				return maxAtomicDate;
			}

			int getMinOldCompDate() const {
				return minOldCompDate;
			}

			int getMaxOldCompDate() const {
				return maxOldCompDate;
			}

			int getMinYoungCompDate() const {
				return minYoungCompDate;
			}

			int getMaxYoungCompDate() const {
				return maxYoungCompDate;
			}

			int getYoungCompFrac() const {
				return youngCompFrac;
			}

			int getTypeSize() const {
				return typeSize;
			}

			int getNumTypes() const {
				return numTypes;
			}

			int getXYRange() const {
				return xyRange;
			}

			int getTitleSize() const {
				return titleSize;
			}

			int getTraversalsRatio() const {
				return areLongTraversalsEnabled() ?
					traversalRatio :
					0;
			}

			int getShortTraversalsRatio() const {
				return shortTraversalRatio;
			}

			int getOperationsRatio() const {
				return operationsRatio;
			}

			int getStructuralModificationsRatio() const {
				return isStructureModificationEnabled() ?
					structuralModificationsRatio :
					0;
			}

			int getReadOnlyOperationsRatio() const {
				return readOnlyOperationsRatio;
			}

			int getThreadNum() const {
				return threadNum;
			}

			int getExperimentLengthMs() const {
				return experimentLengthMs;
			}

			int getMaxLowTtc() const {
				return maxLowTtc;
			}

			int getHighTtcEntries() const {
				return highTtcEntries;
			}

			double getHighTtcLogBase() const {
				return highTtcLogBase;
			}

			bool isStructureModificationEnabled() const {
				return structureModificationEnabled;
			}

			bool areLongTraversalsEnabled() const {
				return longTraversalsEnabled;
			}

			bool shouldReportTtcHistograms() const {
				return reportTtcHistograms;
			}

			bool shouldHintRo() const {
				return hintRo;
			}

			bool shouldWriteRoot() const {
				return writeRoot;
			}

			bool shouldInitSingleTx() const {
				return initSingleTx;
			}

		////////////////////////////////////////////////////////////////
		// setters are protected as only initialization of parameters //
		// is done through init functions                             //
		////////////////////////////////////////////////////////////////

		protected:
			void setNumAtomicPerComp(int val) {
				numAtomicPerComp = val;
				calcMaxAtomicParts();
			}

			void setNumConnPerAtomic(int val) {
				numConnPerAtomic = val;
			}

			void setDocumentSize(int val) {
				documentSize = val;
			}

			void setManualSize(int val) {
				manualSize = val;
			}

			void setNumCompPerModule(int val) {
				numCompPerModule = val;
				calcInitialTotalCompParts();
			}

			void setNumAssmPerAssm(int val) {
				numAssmPerAssm = val;
				calcInitialTotalBaseAssemblies();
			}

			void setNumAssmLevels(int val) {
				numAssmLevels = val;
				calcInitialTotalBaseAssemblies();
			}

			void setNumCompPerAssm(int val) {
				numCompPerAssm = val;
				calcInitialTotalCompParts();
			}

			void setNumModules(int val) {
				numModules = val;
				calcInitialTotalCompParts();
			}

			void calcInitialTotalCompParts() {
				initialTotalCompParts =
					getNumModules() * getNumCompPerModule();
				calcMaxCompParts();
			}

			void calcInitialTotalBaseAssemblies() {
				initialTotalBaseAssemblies = 
					(int)::pow(getNumAssmPerAssm(),
					getNumAssmLevels() - 1);
				calcInitialTotalComplexAssemblies();
				calcMaxBaseAssemblies();
			}

			void calcInitialTotalComplexAssemblies() {
				initialTotalComplexAssemblies =
					(1 - getInitialTotalBaseAssemblies()) /
					(1 - getNumAssmPerAssm());
				calcMaxComplexAssemblies();
			}

			void calcMaxCompParts() {
				maxCompParts =
					(int)(getInitialTotalCompParts() * MAX_TO_INITIAL_RATIO);
				calcMaxAtomicParts();
			}

			void calcMaxAtomicParts() {
				maxAtomicParts =
					getMaxCompParts() * getNumAtomicPerComp();
			}

			void calcMaxBaseAssemblies() {
				maxBaseAssemblies =
					(int)(getInitialTotalBaseAssemblies() *
					MAX_TO_INITIAL_RATIO);
			}

			void calcMaxComplexAssemblies() {
				maxComplexAssemblies =
					(int)(getInitialTotalComplexAssemblies() *
					MAX_TO_INITIAL_RATIO);
			}

			void setMinModuleDate(int val) {
				minModuleDate = val;
			}

			void setMaxModuleDate(int val) {
				maxModuleDate = val;
			}

			void setMinAssmDate(int val) {
				minAssmDate = val;
			}

			void setMaxAssmDate(int val) {
				maxAssmDate = val;
			}

			void setMinAtomicDate(int val) {
				minAtomicDate = val;
			}

			void setMaxAtomicDate(int val) {
				maxAtomicDate = val;
			}

			void setMinOldCompDate(int val) {
				minOldCompDate = val;
			}

			void setMaxOldCompDate(int val) {
				maxOldCompDate = val;
			}

			void setMinYoungCompDate(int val) {
				minYoungCompDate = val;
			}

			void setMaxYoungCompDate(int val) {
				maxYoungCompDate = val;
			}

			void setYoungCompFrac(int val) {
				youngCompFrac = val;
			}

			void setTypeSize(int val) {
				typeSize = val;
			}

			void setNumTypes(int val) {
				numTypes = val;
			}

			void setXYRange(int val) {
				xyRange = val;
			}

			void setTitleSize(int val) {
				titleSize = val;
			}

			void setTraversalsRatio(int val) {
				traversalRatio = val;
			}

			void setShortTraversalsRatio(int val) {
				shortTraversalRatio = val;
			}

			void setOperationsRatio(int val) {
				operationsRatio = val;
			}

			void setStructuralModificationsRatio(int val) {
				structuralModificationsRatio = val;
			}

			void setReadOnlyOperationsRatio(int val) {
				readOnlyOperationsRatio = val;
			}

			void setThreadNum(int val) {
				threadNum = val;
			}

			void setExperimentLengthMs(int val) {
				experimentLengthMs = val;
			}

			void setMaxLowTtc(int val) {
				maxLowTtc = val;
			}

			void setHighTtcEntries(int val) {
				highTtcEntries = val;
			}

			void setHighTtcLogBase(double val) {
				highTtcLogBase = val;
			}

			void setStructureModificationEnabled(bool val) {
				structureModificationEnabled = val;
			}

			void setLongTraversalsEnabled(bool val) {
				longTraversalsEnabled = val;
			}

			void setReportTtcHistograms(bool val) {
				reportTtcHistograms = val;
			}

			void setVerboseLevel(int val) {
				verboseLevel = val;
			}

			void setFileName(const std::string &val) {
				fileName = val;
			}

			void setHintRo(bool val) {
				hintRo = val;
			}

			void setWriteRoot(bool val) {
				writeRoot = val;
			}

			void setInitSingleTx(bool val) {
				initSingleTx = val;
			}

		///////////////////////////////////////////
		// functions for initializing parameters //
		///////////////////////////////////////////

		public:
			bool init(int argc, char **argv, std::ostream &out);

		protected:
			void initDefault();

			void parseCommandLine(int argc, char **argv,
				ConfigParameters &configParams);
			void readFile(ConfigParameters &configParams);
			void applyParameters(ConfigParameters &configParams);
			void printHelp(std::ostream &out);

			int strToWorkloadType(std::string &val) const;
			int strToSizeType(std::string &val) const;

		//////////////////////
		// print parameters //
		//////////////////////

		public:
			void print(std::ostream &out = std::cout) const;
	};

	extern Parameters parameters;

	/**
	 * These are parameters read from the command line or from the config
	 * file.
	 * TODO think about splitting this into two classes - one for command line
	 * parameters and the other one for file based parameters
	 */
	struct ConfigParameters {
		// filename can be set only from command line
		bool fileNameSet;
		std::string fileName;

		// printing help and exiting can be set only from command line
		bool printHelp;

		bool readOnlyPercentSet;
		int percent;

		// takes precedence over previous parameter
		bool workloadTypeSet;
		enum {
			read_dominated = 0,
			read_write,
			write_dominated
		} workloadType;

		bool traversalsEnabledSet;
		bool traversalsEnabled;

		bool structuralModificationsEnabledSet;
		bool structuralModificationsEnabled;

		bool threadNumSet;
		int threadNum;

		bool experimentLengthSet;
		int experimentLength;

		bool sizeSet;
		enum {
			small = 0,
			medium,
			big
		} size;

		bool hintRoSet;
		bool hintRo;

		bool writeRootSet;
		bool writeRoot;

		bool initSingleTxSet;
		bool initSingleTx;

		void clean() {
			fileNameSet = false;
			printHelp = false;
			readOnlyPercentSet = false;
			workloadTypeSet = false;
			traversalsEnabledSet = false;
			structuralModificationsEnabledSet = false;
			threadNumSet = false;
			experimentLengthSet = false;
			sizeSet = false;
			hintRoSet = false;
			writeRootSet = false;
			initSingleTxSet = false;
		}
	};
}

#endif /*SB7_PARAMETERS_H_*/
