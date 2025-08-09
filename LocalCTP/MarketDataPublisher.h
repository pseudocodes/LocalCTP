#pragma once
#include <memory>
#include <string>

class MarketDataPublisher {
public:
    MarketDataPublisher(const std::string& mq_name, std::size_t max_msg_num, std::size_t max_msg_size);

    MarketDataPublisher(
        const std::string& mq_name = "md_mq",
        std::size_t max_msg_num = 1024);

    ~MarketDataPublisher();

    // 单条推送行情
    bool publish(const CThostFtdcDepthMarketDataField& md);

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};
