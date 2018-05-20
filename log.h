/**
 * @file
 *
 * @brief Testing support for network simulation Frameworks.
 *
 * @version 1.0
 * @author  Fuzzier Tang <gauchyler@gmail.com>
 * @date    2018-05-20
 *
 * @copyright Copyright (c) 2018.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef LOG_H__6E655F82_08BD_4CD4_A3CA_BB109465DFA4
#define LOG_H__6E655F82_08BD_4CD4_A3CA_BB109465DFA4


#include <nsfx/log/config.h>

// Core
#include <nsfx/log/core/exception.h>

#include <nsfx/log/core/attribute-value/attribute-value.h>
#include <nsfx/log/core/attribute-value/const-attribute-value.h>

#include <nsfx/log/core/attribute/attribute.h>
#include <nsfx/log/core/attribute/const-attribute.h>
#include <nsfx/log/core/attribute/i-attribute-set.h>
#include <nsfx/log/core/attribute/attribute-set.h>

#include <nsfx/log/core/record/record.h>
#include <nsfx/log/core/record/attribute-value-info.h>

#include <nsfx/log/core/logger/i-logger.h>
#include <nsfx/log/core/logger/logger.h>

#include <nsfx/log/core/filter/i-filter.h>
#include <nsfx/log/core/filter/filter.h>
#include <nsfx/log/core/filter/i-filter-chain.h>
#include <nsfx/log/core/filter/filter-chain.h>

#include <nsfx/log/core/formatter/i-stream-formatter.h>
#include <nsfx/log/core/formatter/stream-formatter.h>

#include <nsfx/log/core/sink/i-stream-sink.h>
#include <nsfx/log/core/sink/stream-sink.h>
#include <nsfx/log/core/sink/i-file-sink.h>
#include <nsfx/log/core/sink/file-sink.h>

// Default
#include <nsfx/log/default/common-attribute-value-info.h>

#include <nsfx/log/default/severity-level.h>
#include <nsfx/log/default/i-severity-level-filter.h>
#include <nsfx/log/default/severity-level-filter.h>

#include <nsfx/log/default/timestamp-attribute.h>

#include <nsfx/log/default/tool.h>


#endif // LOG_H__6E655F82_08BD_4CD4_A3CA_BB109465DFA4

