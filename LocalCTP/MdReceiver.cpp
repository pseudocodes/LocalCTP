
#include <chrono>
#include <iostream>

#include "LocalTraderApi.h"
#include "MdReceiver.h"

using namespace boost::interprocess;

namespace localCTP {

MdReceiver::MdReceiver(message_queue& mq, CLocalTraderApi* api)
    : mq_(mq)
    , traderApi_(api)
    , running_(false)
{
}

MdReceiver::~MdReceiver()
{
    stop();
}

void MdReceiver::start()
{
    running_ = true;
    worker_ = std::thread(&MdReceiver::threadFunc, this);
}

void MdReceiver::stop()
{
    running_ = false;
    if (worker_.joinable())
        worker_.join();
}

void MdReceiver::threadFunc()
{
    const std::chrono::milliseconds timeout(500); // 500ms超时

    while (running_) {
        CThostFtdcDepthMarketDataField md;
        size_t recv_size = 0;
        unsigned int priority = 0;

        bool received = mq_.timed_receive(&md, sizeof(md), recv_size, priority,
            boost::posix_time::microsec_clock::universal_time() + boost::posix_time::milliseconds(timeout.count()));

        if (received && recv_size == sizeof(md)) {
            traderApi_->onSnapshot(md);
        } else {
            // 超时或其他原因，可做其他处理或直接continue等待
            // std::cout << "No message received in 500ms\n";
        }
    }
}

}