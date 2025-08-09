
#pragma once
#ifndef MD_RECEIVER_H
#define MD_RECEIVER_H

#include <atomic>
#include <thread>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>

// 需要完整类型以与交易API交互
#include "ThostFtdcUserApiStruct.h"

namespace localCTP {

class CLocalTraderApi;

class MdReceiver {
public:
    MdReceiver(boost::interprocess::message_queue& mq, CLocalTraderApi* api);
    ~MdReceiver();

    void start();
    void stop();

private:
    void threadFunc();

    boost::interprocess::message_queue& mq_;
    CLocalTraderApi* traderApi_;
    std::atomic<bool> running_;
    std::thread worker_;
};
}
#endif