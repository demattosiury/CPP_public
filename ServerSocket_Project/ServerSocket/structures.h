#ifndef STRUCTURES_H
#define STRUCTURES_H
#pragma once
#include <string>

struct MqlRates {
	long	 time;         // Period start time
	double   open;         // Open price
	double   high;         // The highest price of the period
	double   low;          // The lowest price of the period
	double   close;        // Close price
	long     tick_volume;  // Tick volume
	int      spread;       // Spread
	long     real_volume;  // Trade volume
};

struct Quote
{
	long     time;
	double   open;         // Open price
	double   high;         // The highest price of the period
	double   low;          // The lowest price of the period
	double   close;        // Close price
	int		 symb;
	long     tick_volume;  // Tick volume
	int      spread;       // Spread
	long     real_volume;  // Trade volume
};

#endif
