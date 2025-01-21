#pragma once
#include "BasicCore.h"
//#include "EngineComponent.h"
#include "utils/Singleton.h"
#include "renderer/Renderer.h"
#include "renderer/window/Window.h"

namespace Atlas {
	struct EngineConfig {
	};

	class TechstormEngine : Singleton<TechstormEngine> {
	private:

	public:
		Renderer renderer;

		TechstormEngine() {}


	};
}

/*
		enum class EGlobalVariableType : int8_t {
			VOID = 0,
			BOOL = 1,
			CHAR = 2,
			UCHAR = 3,
			SHORT = 4,
			USHORT = 5,
			INT = 6,
			UINT = 7,
			LONG = 8,
			ULONG = 9,
			FLOAT = 10,
			DOUBLE = 11
		};

		enum class EGlobalVariableLockState : int8_t {
			LOCKED = 0,
			UNLOCKED = 1
		};

		enum class EGlobalVariableScope : int8_t {
			GLOBAL = 0,
			LOCAL = 1
		};

		enum class EGlobalVariableReadWriteState : int8_t {
			READ = 0, // Read only
			WRITE = 1 // Read and write
		};

#ifdef ATLAS_CUSTOM_GLOBAL_VARIABLES_ENUMS
		template<typename T, enum VT, enum VL, enum VS, enum VR>
#else
		template<typename T>
#endif
		class GlobalVariable {
		public:

#ifdef ATLAS_CUSTOM_GLOBAL_VARIABLES_ENUMS
			VT typeEnum;
			VL lockStateEnum;
			VS scopeEnum;
			VR readWriteStateEnum;
#else
			EGlobalVariableType typeEnum;
			EGlobalVariableLockState lockStateEnum;
			EGlobalVariableScope scopeEnum;
			EGlobalVariableReadWriteState readWriteStateEnum;
#endif

			T value;
			std::mutex<std::recursive_mutex> mutex;

			int write(T newValue) {
				if (readWriteStateEnum == EGlobalVariableReadWriteState::WRITE && lockStateEnum == EGlobalVariableLockState::UNLOCKED) {
					value = newValue;
				}
				return 0;
			}

		private:
			bool canWrite() {
				return readWriteStateEnum == EGlobalVariableReadWriteState::WRITE && lockStateEnum == EGlobalVariableLockState::UNLOCKED;
			}
		};

		class GlobalVariablesRegistry {
		public:

		private:
			std::map<std::string, GlobalVariable<int>> mIntVariables;
			std::map<std::string, GlobalVariable<bool>> mBoolVariables;
			std::map<std::string, GlobalVariable<char>> mCharVariables;
			std::map<std::string, GlobalVariable<unsigned char>> mUcharVariables;
			std::map<std::string, GlobalVariable<short>> mShortVariables;
			std::map<std::string, GlobalVariable<unsigned short>> mUshortVariables;
			std::map<std::string, GlobalVariable<unsigned int>> mUintVariables;
			std::map<std::string, GlobalVariable<long>> mLongVariables;
			std::map<std::string, GlobalVariable<unsigned long>> mUlongVariables;
			std::map<std::string, GlobalVariable<float>> mFloatVariables;
			std::map<std::string, GlobalVariable<double>> mDoubleVariables;
		};

		class GameEngineThreadFactory {};

		class GameEngine {
		public:
			std::shared_ptr<GlobalVariablesRegistry> globalVariablesRegistry = std::make_shared<GlobalVariablesRegistry>();
			std::shared_ptr<std::thread> renderThread = nullptr;
			std::shared_ptr<std::thread> workerThread = nullptr;

			//std::shared_ptr<StateMachine> stateMachine = nullptr;

			void init();
			void run();
			void stop();

		private:
			bool shouldClose();
			void renderLoop();
			void workerLoop();
		};
*/
