#include <fstream>
#include <sstream>
#include <getopt.h>

#include "parameters.h"
#include "helpers.h"

namespace sb7 {

	//const int Parameters::DEFAULT_NUM_ATOMIC_PER_COMP = 200;
	const int Parameters::DEFAULT_NUM_ATOMIC_PER_COMP = 20;
	const int Parameters::DEFAULT_NUM_CONN_PER_ATOMIC = 6;
	const int Parameters::DEFAULT_DOCUMENT_SIZE = 20000;
	const int Parameters::DEFAULT_MANUAL_SIZE = 1000000;
	//const int Parameters::DEFAULT_NUM_COMP_PER_MODULE = 500;
	const int Parameters::DEFAULT_NUM_COMP_PER_MODULE = 50;
	const int Parameters::DEFAULT_NUM_ASSM_PER_ASSM = 3;
	const int Parameters::DEFAULT_NUM_ASSM_LEVELS = 7;
	const int Parameters::DEFAULT_NUM_COMP_PER_ASSM = 3;
	const int Parameters::DEFAULT_NUM_MODULES = 1;

	const int Parameters::DEFAULT_MIN_MODULE_DATE = 1000;
	const int Parameters::DEFAULT_MAX_MODULE_DATE = 1999;
	const int Parameters::DEFAULT_MIN_ASSM_DATE = 1000;
	const int Parameters::DEFAULT_MAX_ASSM_DATE = 1999;
	const int Parameters::DEFAULT_MIN_ATOMIC_DATE = 1000;
	const int Parameters::DEFAULT_MAX_ATOMIC_DATE = 1999;
	const int Parameters::DEFAULT_MIN_OLD_COMP_DATE = 0;
	const int Parameters::DEFAULT_MAX_OLD_COMP_DATE = 999;
	const int Parameters::DEFAULT_MIN_YOUNG_COMP_DATE = 2000;
	const int Parameters::DEFAULT_MAX_YOUNG_COMP_DATE = 2999;
	const int Parameters::DEFAULT_YOUNG_COMP_FRAC = 10;
	const int Parameters::DEFAULT_TYPE_SIZE = 10;
	const int Parameters::DEFAULT_NUM_TYPES = 10;
	const int Parameters::DEFAULT_XY_RANGE = 100000;
	const int Parameters::DEFAULT_TITLE_SIZE = 40;

	const int Parameters::DEFAULT_TRAVERSAL_RATIO = 5;
	const int Parameters::DEFAULT_SHORT_TRAVERSAL_RATIO = 40;
	const int Parameters::DEFAULT_OPERATIONS_RATIO = 45;
	const int Parameters::DEFAULT_STRUCTURAL_MODIFICATIONS_RATIO = 10;

	// just for testing make it 70%
	const int Parameters::DEFAULT_READ_ONLY_OPERATIONS_RATIO = 70;

	const int Parameters::DEFAULT_THREAD_NUM = 1;
	const int Parameters::DEFAULT_EXPERIMENT_LENGTH_MS = 60000;

	const int Parameters::DEFAULT_MAX_LOW_TTC = 999;
	const int Parameters::DEFAULT_HIGH_TTC_ENTRIES = 200;
	const double Parameters::DEFAULT_HIGH_TTC_LOG_BASE = 1.03;

	const bool Parameters::DEFAULT_STRUCTURE_MODIFICATION_ENABLED = true;
	const bool Parameters::DEFAULT_LONG_TRAVERSALS_ENABLED = true;
	const bool Parameters::DEFAULT_REPORT_TTC_HISTOGRAMS = false;

	const int Parameters::DEFAULT_VERBOSE_LEVEL = 1;
	const char *Parameters::DEFAULT_FILE_NAME = "default.conf";

	const bool Parameters::DEFAULT_HINT_RO = true;
	const bool Parameters::DEFAULT_WRITE_ROOT = true;
	const bool Parameters::DEFAULT_INIT_SINGLE_TX = true;

	const double Parameters::MAX_TO_INITIAL_RATIO = 1.05;

	Parameters parameters;
}

sb7::Parameters::Parameters()
	: numAtomicPerComp(0),
	numConnPerAtomic(0),
	documentSize(0),
	manualSize(0),
	numCompPerModule(0),
	numAssmPerAssm(0),
	numAssmLevels(0),
	numCompPerAssm(0),
	numModules(0),
	initialTotalCompParts(0),
	initialTotalBaseAssemblies(0),
	initialTotalComplexAssemblies(0),
	maxCompParts(0),
	maxAtomicParts(0),
	maxBaseAssemblies(0),
	maxComplexAssemblies(0),
	minModuleDate(0),
	maxModuleDate(0),
	minAssmDate(0),
	maxAssmDate(0),
	minAtomicDate(0),
	maxAtomicDate(0),
	minOldCompDate(0),
	maxOldCompDate(0),
	minYoungCompDate(0),
	maxYoungCompDate(0),
	youngCompFrac(0),
	typeSize(0),
	numTypes(0),
	xyRange(0),
	titleSize(0),
	traversalRatio(0),
	shortTraversalRatio(0),
	operationsRatio(0),
	structuralModificationsRatio(0),
	readOnlyOperationsRatio(0),
	threadNum(0),
	experimentLengthMs(0),
	maxLowTtc(0),
	highTtcEntries(0),
	highTtcLogBase(0.0),
	structureModificationEnabled(false),
	longTraversalsEnabled(false),
	reportTtcHistograms(false),
	verboseLevel(0),
	hintRo(false),
	writeRoot(false),
	initSingleTx(false) {
}

bool sb7::Parameters::init(int argc, char **argv, std::ostream &out) {
	initDefault();

	ConfigParameters configParamsCL;
	configParamsCL.clean();

	parseCommandLine(argc, argv, configParamsCL);
	bool ret;

	if(configParamsCL.printHelp) {
		ret = false;
		printHelp(out);
	} else {
		ret = true;

		if(configParamsCL.fileNameSet) {
			setFileName(configParamsCL.fileName);
		}

		ConfigParameters configParamsF;
		configParamsF.clean();
		readFile(configParamsF);

		applyParameters(configParamsF);
		applyParameters(configParamsCL);
	}

	return ret;
}

void sb7::Parameters::initDefault() {
	setNumAtomicPerComp(DEFAULT_NUM_ATOMIC_PER_COMP);
	setNumConnPerAtomic(DEFAULT_NUM_CONN_PER_ATOMIC);
	setDocumentSize(DEFAULT_DOCUMENT_SIZE);
	setManualSize(DEFAULT_MANUAL_SIZE);
	setNumCompPerModule(DEFAULT_NUM_COMP_PER_MODULE);
	setNumAssmPerAssm(DEFAULT_NUM_ASSM_PER_ASSM);
	setNumAssmLevels(DEFAULT_NUM_ASSM_LEVELS);
	setNumCompPerAssm(DEFAULT_NUM_COMP_PER_ASSM);
	setNumModules(DEFAULT_NUM_MODULES);

	setMinModuleDate(DEFAULT_MIN_MODULE_DATE);
	setMaxModuleDate(DEFAULT_MAX_MODULE_DATE);
	setMinAssmDate(DEFAULT_MIN_ASSM_DATE);
	setMaxAssmDate(DEFAULT_MAX_ASSM_DATE);
	setMinAtomicDate(DEFAULT_MIN_ATOMIC_DATE);
	setMaxAtomicDate(DEFAULT_MAX_ATOMIC_DATE);
	setMinOldCompDate(DEFAULT_MIN_OLD_COMP_DATE);
	setMaxOldCompDate(DEFAULT_MAX_OLD_COMP_DATE);
	setMinYoungCompDate(DEFAULT_MIN_YOUNG_COMP_DATE);
	setMaxYoungCompDate(DEFAULT_MAX_YOUNG_COMP_DATE);
	setYoungCompFrac(DEFAULT_YOUNG_COMP_FRAC);
	setTypeSize(DEFAULT_TYPE_SIZE);
	setNumTypes(DEFAULT_NUM_TYPES);
	setXYRange(DEFAULT_XY_RANGE);
	setTitleSize(DEFAULT_TITLE_SIZE);

	setTraversalsRatio(DEFAULT_TRAVERSAL_RATIO);
	setShortTraversalsRatio(DEFAULT_SHORT_TRAVERSAL_RATIO);
	setOperationsRatio(DEFAULT_OPERATIONS_RATIO);
	setStructuralModificationsRatio(DEFAULT_STRUCTURAL_MODIFICATIONS_RATIO);

	setReadOnlyOperationsRatio(DEFAULT_READ_ONLY_OPERATIONS_RATIO);

	setThreadNum(DEFAULT_THREAD_NUM);
	setExperimentLengthMs(DEFAULT_EXPERIMENT_LENGTH_MS);

	setMaxLowTtc(DEFAULT_MAX_LOW_TTC);
	setHighTtcEntries(DEFAULT_HIGH_TTC_ENTRIES);
	setHighTtcLogBase(DEFAULT_HIGH_TTC_LOG_BASE);

	setStructureModificationEnabled(DEFAULT_STRUCTURE_MODIFICATION_ENABLED);
	setLongTraversalsEnabled(DEFAULT_LONG_TRAVERSALS_ENABLED);
	setReportTtcHistograms(DEFAULT_REPORT_TTC_HISTOGRAMS);

	setVerboseLevel(DEFAULT_VERBOSE_LEVEL);
	setFileName(DEFAULT_FILE_NAME);

	setHintRo(DEFAULT_HINT_RO);
	setWriteRoot(DEFAULT_WRITE_ROOT);
	setInitSingleTx(DEFAULT_INIT_SINGLE_TX);
}

void sb7::Parameters::print(std::ostream &out) const {
	// if printing is turned off, just return
	if(verboseLevel == 0) {
		return;
	}

	printSection(out, "Parameters");

	out << "ConfigFileName \"" << fileName << '"' << std::endl;

	out << "AtomicPartsPerComponent " << numAtomicPerComp << std::endl;
	out << "ComponentsPerModule " << numCompPerModule << std::endl;

	out << "TraversalRatio " << getTraversalsRatio() << std::endl;
	out << "ShortTraversalRatio " << getShortTraversalsRatio() << std::endl;
	out << "OperationsRatio " << getOperationsRatio() << std::endl;
	out << "StructuralModificationsRatio " << getStructuralModificationsRatio()
		<< std::endl;
	out << "ReadOnlyOperationsRatio " << getReadOnlyOperationsRatio()
		<< std::endl;
	out << "WriteRoot " << boolToStr(writeRoot) << std::endl;
	out << "InitSingleTx " << boolToStr(initSingleTx) << std::endl;

	out << "ThreadNumber " << getThreadNum() << std::endl;
	out << "ExperimentLengthMs " << getExperimentLengthMs() << std::endl;

	out << "ReportTtcHistograms " <<
		boolToStr(reportTtcHistograms) << std::endl;

	out << "HintReadOnly " <<
		boolToStr(hintRo) << std::endl;

	if(verboseLevel >= 2) {
		out << "ConnectionsPerAtomicPart " << numConnPerAtomic << std::endl;
		out << "DocumentSize " << documentSize << std::endl;
		out << "ManualSize " << manualSize << std::endl;
		out << "AssembliesPerAssembly " << numAssmPerAssm << std::endl;
		out << "AssembliesPerLevel " << numAssmLevels << std::endl;
		out << "ComponentsPerAssembly " << numCompPerAssm << std::endl;
		out << "NumberOfModules " << numModules << std::endl;
		out << "InitialCompositeParts " << initialTotalCompParts << std::endl;
		out << "InitialBaseAssemblies " << initialTotalBaseAssemblies
			<< std::endl;
		out << "InitialComplexAssemblies " << initialTotalComplexAssemblies
			<< std::endl;
		out << "MaximumCompositeParts " << maxCompParts << std::endl;
		out << "MaximumAtomicParts " << maxAtomicParts << std::endl;
		out << "MaximumBaseAssemblies " << maxBaseAssemblies << std::endl;
		out << "MaximumComplexAssemblies " << maxComplexAssemblies
			<< std::endl;
	}

	if(verboseLevel >= 3) {
		out << "MinimalModuleDate " << minModuleDate << std::endl;
		out << "MaximalModuleDate " << maxModuleDate << std::endl;
		out << "MinimalAssemblyDate " << minAssmDate << std::endl;
		out << "MaximalAssemblyDate " << maxAssmDate << std::endl;
		out << "MinimalAtomicDate " << minAtomicDate << std::endl;
		out << "MaximalAtomicDate " << maxAtomicDate << std::endl;
		out << "MinimalOldComponentDate " << minOldCompDate << std::endl;
		out << "MaximalOldComponentDate " << maxOldCompDate << std::endl;
		out << "MinimalYoungComponentDate " << minYoungCompDate << std::endl;
		out << "MaximalYoungComponentDate " << maxYoungCompDate << std::endl;
		out << "YoungCompnentFraction " << youngCompFrac << std::endl;
		out << "TypeSize " << typeSize << std::endl;
		out << "NumberOfTypes " << numTypes << std::endl;
		out << "XYRange " << xyRange << std::endl;
		out << "TitleSize " << titleSize << std::endl;
	}

	if(verboseLevel >= 3) {
		out << "MaximalLowTtc " << maxLowTtc << std::endl;
		out << "HighTtcEntries " << highTtcEntries << std::endl;
		out << "HighTtcLogBase " << highTtcLogBase << std::endl;
	}

	if(verboseLevel >= 4) {
		out << "StructureModificationEnabled " <<
			boolToStr(structureModificationEnabled) << std::endl;
		out << "LongTraversalsEnabled " <<
			boolToStr(longTraversalsEnabled) << std::endl;
	}

	out << std::endl;
}

#define FILE_NAME_KEY "fileName"
#define HELP_KEY "help"
#define READ_ONLY_PERCENT_KEY "readOnlyPercent"
#define WORKLOAD_TYPE_KEY "workloadType"
#define TRAVERSALS_KEY "traversals"
#define STRUCTURAL_MODIFICATIONS_KEY "structuralModifications"
#define THREAD_NUMBER_KEY "threadNum"
#define EXPERIMENT_DURATION_KEY "experimentDuration"
#define SIZE_KEY "size"
#define HINT_RO_KEY "hintRo"
#define WRITE_ROOT_KEY "writeRoot"
#define INIT_SINGLE_TX_KEY "initSingleTx"

void sb7::Parameters::parseCommandLine(int argc, char **argv,
		ConfigParameters &configParams) {
	static struct option long_options[] = {
		{FILE_NAME_KEY, 1, 0, 'f'},
		{HELP_KEY, 0, 0, '?'},
		{READ_ONLY_PERCENT_KEY, 1, 0, 'p'},
		{WORKLOAD_TYPE_KEY, 1, 0, 'w'},
		{TRAVERSALS_KEY, 1, 0, 't'},
		{STRUCTURAL_MODIFICATIONS_KEY, 1, 0, 'm'},
		{THREAD_NUMBER_KEY, 1, 0, 'n'},
		{EXPERIMENT_DURATION_KEY, 1, 0, 'd'},
		{SIZE_KEY, 1, 0, 's'},
		{HINT_RO_KEY, 1, 0, 'h'},
		{WRITE_ROOT_KEY, 1, 0, 'r'},
		{INIT_SINGLE_TX_KEY, 1, 0, 'i'},
		{0, 0, 0, 0}
	};

	while(true) {
		int option_index;
		int c = getopt_long(argc, argv, "f:?p:w:t:m:n:d:s:h:r:i:",
			long_options, &option_index);

		if(c == -1) {
			break;
		}

		switch(c) {
			case '?':
				configParams.printHelp = true;
			break;
			case 'f':
				if(optarg) {
					configParams.fileNameSet = true;
					configParams.fileName = std::string(optarg);
				} else {
					std::cout << "File name parameter without value. Ignoring."
							<< std::endl;
				}
			break;
			case 'p':
				if(optarg) {
					std::string percentStr(optarg);
					int percent = strToUint(percentStr);

					if(percent >= 0 && percent <= 100) {
						configParams.readOnlyPercentSet = true;
						configParams.percent = percent;
					} else {
						std::cout << "Read only percent parameter has "
							"wrong value. Ignoring." << std::endl;
					}
				} else {
					std::cout << "Read only percent parameter without value. "
							"Ignoring." << std::endl;
				}
			break;
			case 'w':
				if(optarg) {
					std::string workloadTypeStr(optarg);
					int workloadType = strToWorkloadType(workloadTypeStr);

					if(workloadType == -1) {
						std::cout << "Workload type parameter has "
							"wrong value. Ignoring." << std::endl;
					} else {
						configParams.workloadTypeSet = true;
						(int &)configParams.workloadType = workloadType;
					}
				} else {
					std::cout << "Workload type parameter without value. "
							"Ignoring." << std::endl;
				}
			break;
			case 't':
				if(optarg) {
					std::string traversalStr(optarg);
					int val = strToBool(traversalStr);

					if(val == -1) {
						std::cout << "Traversals parameter has "
							"wrong value. Ignoring." << std::endl;
					} else {
						configParams.traversalsEnabledSet = true;
						configParams.traversalsEnabled = (val == 1);
					}
				} else {
					std::cout << "Traversals parameter without value. Ignoring."
						<< std::endl;
				}
			break;
			case 'm':
				if(optarg) {
					std::string structuralModificationsStr(optarg);
					int val = strToBool(structuralModificationsStr);

					if(val == -1) {
						std::cout << "Structural modifications parameter has "
							"wrong value. Ignoring." << std::endl;
					} else {
						configParams.structuralModificationsEnabledSet = true;
						configParams.structuralModificationsEnabled =
							(val == 1);
					}
				} else {
					std::cout << "Structural modifications parameter "
							"without value. Ignoring." << std::endl;
				}
			break;
			case 'n':
				if(optarg) {
					std::string threadNumStr(optarg);
					int threadNum = strToUint(threadNumStr);

					if(threadNum >= 1) {
						configParams.threadNumSet = true;
						configParams.threadNum = threadNum;
					} else {
						std::cout << "Thread number parameter has "
							"invalid value. Ignoring." << std::endl;
					}
				} else {
					std::cout << "Thread number parameter without value. "
							"Ignoring." << std::endl;
				}
			break;
			case 'd':
				if(optarg) {
					std::string durationStr(optarg);
					int duration = strToUint(durationStr);

					if(duration >= 1) {
						configParams.experimentLengthSet = true;
						configParams.experimentLength = duration;
					} else {
						std::cout << "Duration parameter has "
							"invalid value. Ignoring." << std::endl;
					}
				} else {
					std::cout << "Duration parameter without value. "
							"Ignoring." << std::endl;
				}
			break;
			case 's':
				if(optarg) {
					std::string sizeTypeStr(optarg);
					int sizeType = strToSizeType(sizeTypeStr);

					if(sizeType == -1) {
						std::cout << "Size type parameter has "
							"wrong value. Ignoring." << std::endl;
					} else {
						configParams.sizeSet = true;
						(int &)configParams.size = sizeType;
					}
				} else {
					std::cout << "Size type parameter without value. "
							"Ignoring." << std::endl;
				}
			break;
			case 'h':
				if(optarg) {
					std::string hintRoStr(optarg);
					int val = strToBool(hintRoStr);

					if(val == -1) {
						std::cout << "Hint read only parameter has "
							"wrong value. Ignoring." << std::endl;
					} else {
						configParams.hintRoSet = true;
						configParams.hintRo = (val == 1);
					}
				} else {
					std::cout << "Hint read only parameter "
							"without value. Ignoring." << std::endl;
				}
			break;
			case 'r':
				if(optarg) {
					std::string writeRootStr(optarg);
					int val = strToBool(writeRootStr);

					if(val == -1) {
						std::cout << "WriteRoot parameter has "
							"wrong value. Ignoring." << std::endl;
					} else {
						configParams.writeRootSet = true;
						configParams.writeRoot = (val == 1);
					}
				} else {
					std::cout << "WriterRoot parameter without " <<
						"value. Ignoring." << std::endl;
				}
			break;
			case 'i':
				if(optarg) {
					std::string initSingleTxStr(optarg);
					int val = strToBool(initSingleTxStr);

					if(val == -1) {
						std::cout << "InitSingleTx parameter has "
							"wrong value. Ignoring." << std::endl;
					} else {
						configParams.initSingleTxSet = true;
						configParams.initSingleTx = (val == 1);
					}
				} else {
					std::cout << "InitSingleTx parameter without " <<
						"value. Ignoring." << std::endl;
				}
			break;
			default:
				std::cout << "Unknown parameter. Ignoring." << std::endl;
			break;
		}
	}
}

void sb7::Parameters::readFile(ConfigParameters &configParams) {
	std::ifstream inputFile;
	inputFile.open(fileName.c_str());

	if(!inputFile) {
		return;
	}

	// just simple parse of the file
	int lineNo = 0;

	while(inputFile) {
		std::string line;
		std::getline(inputFile, line);
		lineNo++;

		// cut off everything after comment symbol
		std::string::size_type commentPos = line.find('#');

		if(commentPos != std::string::npos) {
			line.erase(commentPos);
		}

		std::istringstream lineStream(line);

		// read key
		std::string key;
		lineStream >> key;

		// if the line was empty or comment continue
		if(key.size() == 0) {
			continue;
		}

		// read val
		std::string val;
		lineStream >> val;

		// this is error - skip line
		if(val.size() == 0) {
			std::cout << "Line " << lineNo << ": missing parameter value. "
				"Ignoring." << std:: endl;
			continue;
		}

		// check if there is anything left
		std::string tail;
		lineStream >> tail;

		// there should be nothing after value in line that has no comment
		if(tail.size() > 0) {
			std::cout << "Line " << lineNo << ": too many parameters. "
				"Ignoring." << std:: endl;
			continue;			
		}

		// now set parameters
		if(equalNoCase(key, READ_ONLY_PERCENT_KEY)) {
			int percent = strToUint(val);

			if(percent >= 0 && percent <= 100) {
				configParams.readOnlyPercentSet = true;
				configParams.percent = percent;
			} else {
				std::cout << "Line " << lineNo << ": ";
				std::cout << "Read only percent parameter has wrong value. "
					"Ignoring." << std::endl;
			}
		} else if(equalNoCase(key, WORKLOAD_TYPE_KEY)) {
			int workloadType = strToWorkloadType(val);

			if(workloadType == -1) {
				std::cout << "Line " << lineNo << ": ";
				std::cout << "Workload type parameter has "
					"wrong value. Ignoring." << std::endl;
			} else {
				configParams.workloadTypeSet = true;
				(int &)configParams.workloadType = workloadType;
			}
		} else if(equalNoCase(key, TRAVERSALS_KEY)) {
			int traversals = strToBool(val);

			if(traversals == -1) {
				std::cout << "Line " << lineNo << ": ";
				std::cout << "Traversals parameter has wrong value. Ignoring."
					<< std::endl;
			} else {
				configParams.traversalsEnabledSet = true;
				configParams.traversalsEnabled = (traversals == 1);
			}
		} else if(equalNoCase(key, STRUCTURAL_MODIFICATIONS_KEY)) {
			int structuralModifications = strToBool(val);

			if(structuralModifications == -1) {
				std::cout << "Line " << lineNo << ": ";
				std::cout << "Structural modifications parameter has "
					"wrong value. Ignoring." << std::endl;
			} else {
				configParams.structuralModificationsEnabledSet = true;
				configParams.structuralModificationsEnabled =
					(structuralModifications == 1);
			}
		} else if(equalNoCase(key, THREAD_NUMBER_KEY)) {
			int threadNum = strToUint(val);

			if(threadNum >= 1) {
				configParams.threadNumSet = true;
				configParams.threadNum = threadNum;
			} else {
				std::cout << "Line " << lineNo << ": ";
				std::cout << "Thread number parameter has "
					"invalid value. Ignoring." << std::endl;
			}
		} else if(equalNoCase(key, EXPERIMENT_DURATION_KEY)) {
			int duration = strToUint(val);

			if(duration >= 1) {
				configParams.experimentLengthSet = true;
				configParams.experimentLength = duration;
			} else {
				std::cout << "Duration parameter has "
					"invalid value. Ignoring." << std::endl;
			}
		} else if(equalNoCase(key, SIZE_KEY)) {
			int sizeType = strToSizeType(val);

			if(sizeType == -1) {
				std::cout << "Size type parameter has "
					"wrong value. Ignoring." << std::endl;
			} else {
				configParams.sizeSet = true;
				(int &)configParams.size = sizeType;
			}
		} else if(equalNoCase(key, HINT_RO_KEY)) {
			int hintRo = strToBool(val);

			if(hintRo == -1) {
				std::cout << "Line " << lineNo << ": ";
				std::cout << "Hint read only parameter has "
					"wrong value. Ignoring." << std::endl;
			} else {
				configParams.hintRoSet = true;
				configParams.hintRo = (hintRo == 1);
			}
		} else if(equalNoCase(key, WRITE_ROOT_KEY)) {
			int writeRoot = strToBool(val);

			if(writeRoot == -1) {
				std::cout << "Line " << lineNo << ": ";
				std::cout << "WriteRoot parameter has wrong value. "
					<< "Ignoring." << std::endl;
			} else {
				configParams.writeRootSet = true;
				configParams.writeRoot = (writeRoot == 1);
			}
		} else if(equalNoCase(key, INIT_SINGLE_TX_KEY)) {
			int initSingleTx = strToBool(val);

			if(initSingleTx == -1) {
				std::cout << "Line " << lineNo << ": ";
				std::cout << "InitSingleTx parameter has wrong value. "
					<< "Ignoring." << std::endl;
			} else {
				configParams.initSingleTxSet = true;
				configParams.initSingleTx = (initSingleTx == 1);
			}
		} else {
			std::cout << "Unknown parameter at line " << lineNo
				<< ". Ignoring." << std::endl;
		}
	}

	inputFile.close();
}

#define READ_DOMINATED_WORKLOAD_PERCENT 90
#define READ_WRITE_WORKLOAD_PERCENT 60
#define WRITE_DOMINATED_WORKLOAD_PERCENT 10

#define SMALL_NUM_ATOMIC_PER_COMP 20
#define BIG_NUM_ATOMIC_PER_COMP 200
#define SMALL_NUM_COMP_PER_MODULE 50
#define BIG_NUM_COMP_PER_MODULE 500

void sb7::Parameters::applyParameters(ConfigParameters &configParams) {
	if(configParams.readOnlyPercentSet) {
		if(configParams.workloadTypeSet) {
			std::cout << "Both read only percent and workload type set. "
				"Ignoring read only percent parameter." << std::endl;
		} else {
			readOnlyOperationsRatio = configParams.percent;
		}
	}

	if(configParams.workloadTypeSet) {
		int percent;

		if(configParams.workloadType == ConfigParameters::read_dominated) {
			percent = READ_DOMINATED_WORKLOAD_PERCENT;
		} else if(configParams.workloadType == ConfigParameters::read_write) {
			percent = READ_WRITE_WORKLOAD_PERCENT;
		} else {
			percent = WRITE_DOMINATED_WORKLOAD_PERCENT;
		}

		readOnlyOperationsRatio = percent;
	}

	if(configParams.traversalsEnabledSet) {
		longTraversalsEnabled = configParams.traversalsEnabled;
	}

	if(configParams.structuralModificationsEnabledSet) {
		structureModificationEnabled =
			configParams.structuralModificationsEnabled; 
	}

	if(configParams.threadNumSet) {
		threadNum = configParams.threadNum;
	}

	if(configParams.experimentLengthSet) {
		experimentLengthMs = configParams.experimentLength;
	}

	if(configParams.sizeSet) {
		int apc;
		int cpm;

		if(configParams.size == ConfigParameters::small) {
			apc = SMALL_NUM_ATOMIC_PER_COMP;
			cpm = SMALL_NUM_COMP_PER_MODULE;
		} else if(configParams.size == ConfigParameters::medium) {
			apc = SMALL_NUM_ATOMIC_PER_COMP;
			cpm = BIG_NUM_COMP_PER_MODULE;
		} else {
			apc = BIG_NUM_ATOMIC_PER_COMP;
			cpm = BIG_NUM_COMP_PER_MODULE;
		}

		setNumAtomicPerComp(apc);
		setNumCompPerModule(cpm);
	}

	if(configParams.hintRoSet) {
		hintRo = configParams.hintRo;
	}

	if(configParams.writeRootSet) {
		writeRoot = configParams.writeRoot;
	}

	if(configParams.initSingleTxSet) {
		initSingleTx = configParams.initSingleTx;
	}
}

void sb7::Parameters::printHelp(std::ostream &out) {
	out << "Command line parameters:" << std::endl
		<< "\t--" << HELP_KEY << " (-?) - prints help" << std::endl
		<< "\t--" << FILE_NAME_KEY
			<< " (-f) <file_name> - sets options file name" << std::endl
		<< "\t--" << READ_ONLY_PERCENT_KEY << " (-p) <number> "
			"- set percent of read-only operations" << std::endl
		<< "\t--" << WORKLOAD_TYPE_KEY << " (-w) r|rw|w "
			"- set workload type" << std::endl
		<< "\t--" << TRAVERSALS_KEY << " (-t) true|false "
			"- enable/disable long traversals" << std::endl
		<< "\t--" << STRUCTURAL_MODIFICATIONS_KEY << " (-m) true|false "
			"- enable/disable structural modifications" << std::endl
		<< "\t--" << THREAD_NUMBER_KEY << " (-n) <number> "
			"- set number of simultaneous threads " << std::endl
		<< "\t--" << SIZE_KEY << " (-s) s|m|b "
			"- set size of data structure" << std::endl
		<< "\t--" << HINT_RO_KEY << " (-h) true|false "
			"- set hint read only flag" << std::endl
		<< "\t--" << WRITE_ROOT_KEY << " (-r) true|false "
			"- set whether to change root or not" << std::endl
		<< "\t--" << INIT_SINGLE_TX_KEY << " (-i) true|false "
			"- set whether to initialize data in single or multiple "
			"transaction" << std::endl

		<< std::endl;
}

int sb7::Parameters::strToWorkloadType(std::string &val) const {
	if(equalNoCase(val, std::string("readDominated")) ||
			equalNoCase(val, std::string("read_dominated")) ||
			equalNoCase(val, std::string("r"))) {
		return ConfigParameters::read_dominated;
	} else 	if(equalNoCase(val, std::string("readWrite")) ||
			equalNoCase(val, std::string("read_write")) ||
			equalNoCase(val, std::string("rw")) ||
			equalNoCase(val, std::string("wr"))) {
		return ConfigParameters::read_write;
	} else 	if(equalNoCase(val, std::string("writeDominated")) ||
			equalNoCase(val, std::string("write_dominated")) ||
			equalNoCase(val, std::string("w"))) {
		return ConfigParameters::write_dominated;
	} else {
		return -1;
	}
}

int sb7::Parameters::strToSizeType(std::string &val) const {
	if(equalNoCase(val, std::string("small")) ||
			equalNoCase(val, std::string("s"))) {
		return ConfigParameters::small;
	} else 	if(equalNoCase(val, std::string("medium")) ||
			equalNoCase(val, std::string("m"))) {
		return ConfigParameters::medium;
	} else 	if(equalNoCase(val, std::string("big")) ||
			equalNoCase(val, std::string("b"))) {
		return ConfigParameters::big;
	} else {
		return -1;
	}
}
