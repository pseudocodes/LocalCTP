#include <boost/interprocess/ipc/message_queue.hpp>
#include <iostream>

#include "ThostFtdcUserApiStruct.h"

#include "MarketDataPublisher.h"

using namespace boost::interprocess;

struct MarketDataPublisher::Impl {
    message_queue mq;
    std::string mq_name;

    Impl(const std::string& name, std::size_t max_msg_num, std::size_t max_msg_size)
        : mq_name(name)
        , mq(open_or_create, name.c_str(), max_msg_num, max_msg_size)
    {
        message_queue::remove(mq_name.c_str()); // 清理旧队列
    }

    ~Impl()
    {
        message_queue::remove(mq_name.c_str());
    }

    bool publish(const CThostFtdcDepthMarketDataField& md)
    {
        try {
            mq.send(&md, sizeof(md), 0);
            return true;
        } catch (interprocess_exception& e) {
            std::cerr << "Publish error: " << e.what() << std::endl;
            return false;
        }
    }
};

MarketDataPublisher::MarketDataPublisher(
    const std::string& mq_name,
    std::size_t max_msg_num,
    std::size_t max_msg_size)
    : impl_(std::make_unique<Impl>(mq_name, max_msg_num, max_msg_size))
{
}

MarketDataPublisher::MarketDataPublisher(const std::string& mq_name, std::size_t max_msg_num)
    : MarketDataPublisher(mq_name, max_msg_num, sizeof(CThostFtdcDepthMarketDataField))
{
}

MarketDataPublisher::~MarketDataPublisher() = default;

bool MarketDataPublisher::publish(const CThostFtdcDepthMarketDataField& md)
{
    return impl_->publish(md);
}