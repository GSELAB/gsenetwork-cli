#include <core/JsonHelper.h>
#include <core/Ballot.h>

using namespace core;

namespace client {

Json::Value toRequestBlock(uint64_t number);

Json::Value toRequestBeProducer(Address const& address);

Json::Value toRequestVersion();

Json::Value toRequestTransfer(Address const& sender, Address const& recipient, uint64_t value);

Json::Value toRequestVote(Address const& sender, Ballot& ballot);
}