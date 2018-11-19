#pragma once

#include <crypto/Common.h>
#include <core/Transaction.h>
#include <core/Block.h>

using namespace core;

namespace crypto {

bool isValidSig(Transaction& transaction);

bool isValidSig(Block& block);
}