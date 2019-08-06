#include <client/JsonRequest.h>

using namespace core::js_util;

namespace client {

Json::Value toRequestBlock(uint64_t number)
{
    Json::Value result;
    result["blockNumber"] = number;
    return result;
}

Json::Value toRequestBeProducer(Address const& address)
{
    Json::Value result;
    result["sender"] = toJS(address);
    return result;
}

Json::Value toRequestVersion()
{
    Json::Value result;
    return result;
}

Json::Value toRequestHeight()
{
    Json::Value result;
    return result;
}

Json::Value toRequestSolidifyHeight()
{
    Json::Value result;
    return result;
}

Json::Value toRequestProducerList()
{
    Json::Value result;
    return result;
}

Json::Value toRequestTransaction(std::string const& txHash)
{
    Json::Value result;
    result["txHash"] = toJS(txHash);
    return result;
}

Json::Value toRequestTransfer(Address const& sender, Address const& recipient,bytes const& data, uint64_t value)
{
    Json::Value result;
    result["sender"] = toJS(sender);
    result["recipient"] = toJS(recipient);
    result["data"]  = toJS(data);
    result["value"] = value;
    return result;
}

Json::Value toRequestVote(Address const& sender, Ballot& ballot)
{
    Json::Value result;
    result["sender"] = toJS(sender);
    result["data"] = toJS(ballot.getRLPData());
    return result;
}

Json::Value toRequestBalance(Address const& address)
{
    Json::Value result;
    result["address"] = toJS(address);
    return result;
}

Json::Value toRequestProducer(Address const& address)
{
    Json::Value result;
    result["address"] = toJS(address);
    return result;
}

Json::Value toRequestAccount(Address const& address)
{
    Json::Value result;
    result["address"] = toJS(address);
    return result;
}
}
