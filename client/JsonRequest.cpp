#include <client/JsonRequest.h>

using namespace core::js_util;

namespace client {

Json::Value toRequestBlock(uint64_t number)
{
    Json::Value result;
    result["blockNumber"] = toJS(number);
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

Json::Value toRequestTransfer(Address const& sender, Address const& recipient, uint64_t value)
{
    Json::Value result;
    result["sender"] = toJS(sender);
    result["recipient"] = toJS(recipient);
    result["value"] = toJS(value);
    return result;
}

Json::Value toRequestVote(Address const& sender, Ballot& ballot)
{
    Json::Value result;
    result["sender"] = toJS(sender);
    result["data"] = toJS(ballot.getRLPData());
    return result;
}
}