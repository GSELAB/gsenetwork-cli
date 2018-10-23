#pragma once

#include <crypto/Common.h>
#include <core/Transaction.h>
#include <core/Block.h>

using namespace core;
using namespace chain;

namespace crypto {

bool isValidSig(Transaction& transaction);

bool isValidSig(Block& block);

}