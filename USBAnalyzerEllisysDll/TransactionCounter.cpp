#include "TransactionCounter.h"

TransactionCounter::TransactionCounter(void)
{
	packetsTokenIn = 0;
	packetsTokenOut = 0;
	packetsTokenSetup = 0;
	packetsTokenSOF = 0;
	packetsData0 = 0;
	packetsData1 = 0;
	packetsHandshakeACK = 0;
	packetsHandshakeNAK = 0;
}

TransactionCounter::~TransactionCounter(void)
{
}


unsigned long TransactionCounter::GetCountTokenIn(void)
{
	return packetsTokenIn;
}

unsigned long TransactionCounter::GetCountTokenOut(void)
{
	return packetsTokenOut;
}

unsigned long TransactionCounter::GetCountTokenSetup(void)
{
	return packetsTokenSetup;
}

unsigned long TransactionCounter::GetCountTokenSOF(void)
{
	return packetsTokenSOF;
}

unsigned long TransactionCounter::GetCountData0(void)
{
	return packetsData0;
}

unsigned long TransactionCounter::GetCountData1(void)
{
	return packetsData1;
}

unsigned long TransactionCounter::GetCountACK(void)
{
	return packetsHandshakeACK;
}

unsigned long TransactionCounter::GetCountNAK(void)
{
	return packetsHandshakeNAK;
}

void TransactionCounter::IncrementTokenIn(void)
{
	packetsTokenIn++;
}

void TransactionCounter::IncrementTokenOut(void)
{
	packetsTokenOut++;
}

void TransactionCounter::IncrementTokenSetup(void)
{
	packetsTokenSetup++;
}

void TransactionCounter::IncrementTokenSOF(void)
{
	packetsTokenSOF++;
}

void TransactionCounter::IncrementData0(void)
{
	packetsData0++;
}

void TransactionCounter::IncrementData1(void)
{
	packetsData1++;
}

void TransactionCounter::IncrementACK(void)
{
	packetsHandshakeACK++;
}

void TransactionCounter::IncrementNAK(void)
{
	packetsHandshakeNAK++;
}
