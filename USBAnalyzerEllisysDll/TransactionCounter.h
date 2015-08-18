#pragma once

class TransactionCounter
{

public:
	TransactionCounter(void);
	~TransactionCounter(void);

private:
	unsigned long packetsTokenIn;
	unsigned long packetsTokenOut;
	unsigned long packetsTokenSetup;
	unsigned long packetsTokenSOF;
	unsigned long packetsData0;
	unsigned long packetsData1;
	unsigned long packetsHandshakeACK;
	unsigned long packetsHandshakeNAK;
public:
	unsigned long GetCountTokenIn(void);
	unsigned long GetCountTokenOut(void);
	unsigned long GetCountTokenSetup(void);
	unsigned long GetCountTokenSOF(void);
	unsigned long GetCountData0(void);
	unsigned long GetCountData1(void);
	unsigned long GetCountACK(void);
	unsigned long GetCountNAK(void);
	void IncrementTokenIn(void);
	void IncrementTokenOut(void);
	void IncrementTokenSetup(void);
	void IncrementTokenSOF(void);
	void IncrementData0(void);
	void IncrementData1(void);
	void IncrementACK(void);
	void IncrementNAK(void);
};
