#ifndef _ICM_CONFIG_H_
#define _ICM_CONFIG_H_

namespace ICM
{
	class Config
	{
	public:
		Config(bool DebugMode, bool PrintResult, bool PrintIntervalTime)
			: DebugMode(DebugMode), PrintAST(DebugMode),
			PrintFlagWord(DebugMode), PrintResult(PrintResult), PrintIntervalTime(PrintIntervalTime) {}
		void SetDebugMode(bool value) {
			DebugMode = value;
			PrintAST = DebugMode;
			PrintFlagWord = DebugMode;
		}

		bool DebugMode;
		bool PrintAST = DebugMode;
		bool PrintFlagWord = DebugMode;
		bool PrintResult = true;
		bool PrintIntervalTime;
		bool PrintOrder = false;
		bool PrintObjectAllot = false;
	};
}

extern ICM::Config GlobalConfig;


#endif
