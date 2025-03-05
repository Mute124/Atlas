#pragma once
#include <string>
#include <vector>
#include <mutex>
#include <functional>

#include "GameThread.h"

namespace Atlas {


    class ThreadGroup {
    protected:
        std::vector<GameThread> mThreads;
        
        bool mShouldStop = false;
        
        std::mutex mMutex;
        
        std::condition_variable mConditionalVariable;

        std::string mGroupName;


    public:
        ThreadGroup() = default;

        virtual ~ThreadGroup();

        virtual void addThread(const std::string& name, std::function<void()> task);

        virtual void stopAll();

        virtual void waitForAll();


    };
}