/*
 * Copyright (c) 2018 GSENetwork
 *
 * This file is part of GSENetwork.
 *
 * GSENetwork is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or any later version.
 *
 */

#pragma once

#include <core/Object.h>
#include <core/RLP.h>
#include <core/Log.h>
#include <core/Block.h>
#include <core/Exceptions.h>

#include <boost/variant/variant.hpp>
#include <boost/variant/get.hpp>

using namespace core;

namespace config {

// Genesis block time stamp is 2018-10-01 UTC 00:00:00
#define GENESIS_TIMESTAMP (1538352000000)

//
#define MAX_TRANSACTION_QUEUE_SIZE  (1024 * 128)

#define MAX_BLOCK_QUEUE_SIZE (1024 * 2)

// Max transaction size 512 Bytes (0.5 KB)
#define MAX_TRANSACTION_SIZE (512)

// Maximum 750 transactions per block
#define MAX_TRANSACTIONS_PER_BLOCK (750)

// Max block size 1024 KB (1 M)
#define MAX_BLOCK_SIZE (1024 * 1024)

// 21 delegated blocks
#define NUM_DELEGATED_BLOCKS (7)    //21

// 0.1 second = 100 ms
#define PRODUCER_SLEEP_INTERVAL  (100)

// 2 second = 2000 ms
#define PRODUCER_INTERVAL (2000)

// Time per round
#define TIME_PER_ROUND ((PRODUCER_INTERVAL) * (NUM_DELEGATED_BLOCKS))

// Schedule list gets updated every 500 rounds
#define SCHEDULE_UPDATE_ROUNDS (500)

// Time period that schedule gets updated
#define SCHEDULE_UPDATE_INTERVAL ((SCHEDULE_UPDATE_ROUNDS) * (TIME_PER_ROUND))

} // end namespace
