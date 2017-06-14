/*********************************************************************
 * Project        System
 * (c) copyright  2015
 * Company        Harman/Becker Automotive Systems GmbH
 *                All rights reserved
 * Secrecy Level STRICTLY CONFIDENTIAL 
 *********************************************************************
 *    _______  ____  ______
 *   / __/ _ \/ __ \/_  __/
 *  _\ \/ ___/ /_/ / / /
 * /___/_/   \____/ /_/
 *
 * SPOT - System Profiling and Optimization Team
 *
 *********************************************************************
 * @brief
 * KPI MARKER Macro
 */

#ifndef __KPI_MARKER_H_INCLUDED
#define __KPI_MARKER_H_INCLUDED

#if defined(__QNX__)
#include <stdio.h>
#include <sys/slog.h>
#define SLOG_KPI_MARKER(MINORCODE, ...) slogf(_SLOG_SETCODE(20022, MINORCODE), _SLOG_INFO, __VA_ARGS__);
#elif defined(__linux__)
#define SLOG_KPI_MARKER(MINORCODE, ...) printf("SLOG_MARKER: to be done\n");
#endif

// Call the following macro for printing a kpi marker
// Example: KPIMARKER(11, "ABCDEFG");
#define KPIMARKER(MINORCODE, TEXT) SLOG_KPI_MARKER(MINORCODE, "KPI_MARKER: " TEXT);

#endif // __KPI_MARKER_H_INCLUDED
