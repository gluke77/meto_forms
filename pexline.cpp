#include "pexline.h"

#include "hmi.h"
#include "cpu.h"
#include "invertor.h"
#include "termo.h"
#include "pressure.h"
#include "nestedinvertor.h"
#include "nestedtermo.h"
#include "nestedpressure.h"
#include "baseport.h"
#include "basedevice.h"
#include "rangeparameter.h"
#include "storedvalue.h"
#include "triggervalue.h"
#include "uzg.h"
#include "printer.h"

//#define K_PE		(17.38095)
#define K_PE		(17.39)
//#define K_PE_FULL	(.3 / K_PE)
#define K_PE_FULL	(0.0173)

//#define K_KE		(24.13793)
#define K_KE		(24.25)
//#define K_KE_FULL	(.3 / K_KE)
#define K_KE_FULL	(0.0124)

#define K_ME		(68.29268)
//#define K_ME_FULL	(.3 / K_ME)
#define K_ME_FULL	(0.0011)

#define K_SG		(5.4925)
#define K_SG_FULL	(.3 / K_SG)
#define K_SG_FULL	(0.0236)

PexLine::PexLine()
{
	{
		TriggerValue	con(0, 0, 0, 0, 0, 1, 1, 0, 1, 0);
		cpu = Cpu(0, 0, con);
	}

	{
		TriggerValue	con(0, 0, 0, 0, 0, 1, 1, 0, 1, 0);
		uzg = Uzg(0, 4, con);
	}

	{
		TriggerValue	con(0, 0, 0, 0, 0, 1, 1, 0, 1, 0);
		printer = Printer(0, 1, con);
	}


	{
		TriggerValue	con(0, 0, 0, 0, 0, 1, 1, 0, 1, 0);
		TriggerValue	rs(0, 0x2101, 0x2000, 1, 1, 0x0003, 0x0003, 0, 0x0002, 0x0001);
		StoredValue		freq(0, K_SG_FULL, 0x2102, 0x2001, 1, 1);
		RangeParameter	current(
			StoredValue(0, 0.01, 0x2104, 0, 0, 1),
			StoredValue(0, 0.01, 0, 0, 0, 0),
			StoredValue(0, 0.01, 0, 0, 0, 0),
			StoredValue(0, 0.01, 0, 0, 0, 0),
			StoredValue(0, 0.01, 0, 0, 0, 0),
			StoredValue(0, 0.01, 0, 0, 0, 0)
			);

		sgInvertor = SgInvertor(0, 3, con, rs, current, freq);
		
		current = RangeParameter(
			StoredValue(0, 0.1, 0x2104, 0, 0, 1),
			StoredValue(0, 0.1, 0, 0, 0, 0),
			StoredValue(0, 0.1, 0, 0, 0, 0),
			StoredValue(0, 0.1, 0, 0, 0, 0),
			StoredValue(0, 0.1, 0, 0, 0, 0),
			StoredValue(0, 0.1, 0, 0, 0, 0)
			);
		rs = TriggerValue(0, 0x2101, 0x2000, 1, 1, 0x1000, 0x1000, 0, 0x0002, 0x0001);
		freq = StoredValue(0, 0.003105, 0x2102, 0x2001, 1, 1);
		tuInvertor = Invertor(0, 5, con, rs, current, freq);
	}
/*
	Termo(BasePort * port = 0, const int deviceId = 0,
		const TriggerValue & con = TriggerValue(false, 0, 0, false, false),
		const TriggerValue & rs = TriggerValue(),
		const RangeParameter & rp = RangeParameter())

*/
	{
		TriggerValue	con(0, 0, 0, 0, 0, 1, 1, 0, 1, 0);
		TriggerValue	rs(0, 0x4719, 0x4719, 1, 1, 0xFFFF, 1, 0, 1, 0);
		RangeParameter temp(
			StoredValue(0, 0.1, 0x4700, 0, 0, 1),
			StoredValue(0, 0.1, 0x4701, 0x4701, 1, 1),
			StoredValue(0, 0.1, 0, 0, 0, 0),
			StoredValue(0, 0.1, 0, 0, 0, 0),
			StoredValue(0, 0.1, 0, 0, 0, 0),
			StoredValue(0, 0.1, 0, 0, 0, 0)
			);

		sgZone1 = Termo(0, 1, con, rs, temp);
		sgZone2 = Termo(0, 2, con, rs, temp);
	}
	
	{
		TriggerValue	con(0, PEM_ON, PEM_ON, 1, 1, 1, 1, 0, 1, 0);
		TriggerValue	rs(0, R_PEM_RS, PW_PEM_RS, 1, 1, 1, 1, 0, 1, 0);
		StoredValue		freq(0, K_PE_FULL, R_PEM_SV, PW_PEM_SV, 1, 1);
		RangeParameter	current(
			StoredValue(0, 0.1, PEM_C, 0, 0, 1),
			StoredValue(0, 0.1, 0, 0, 0, 0),
			StoredValue(0, 0.1, PEM_CHW, PEM_CHW, 1, 1),
			StoredValue(0, 0.1, PEM_CLW, PEM_CLW, 1, 1),
			StoredValue(0, 0.1, PEM_CHC, PEM_CHC, 1, 1),
			StoredValue(0, 0.1, PEM_CLC, PEM_CLC, 1, 1)
			);

		e2pInvertor = NestedInvertor(0, 1, con, rs, current, freq);
		e3pInvertor = NestedInvertor(0, 2, con, rs, current, freq);
	}

	{
		TriggerValue	con(0, KLM_ON, KLM_ON, 1, 1, 1, 1, 0, 1, 0);
		TriggerValue	rs(0, R_KLM_RS, PW_KLM_RS, 1, 1, 1, 1, 0, 1, 0);
		StoredValue		freq(0, K_KE_FULL, R_KLM_SV, PW_KLM_SV, 1, 1);
		RangeParameter	current(
			StoredValue(0, 0.1, KLM_C, 0, 0, 1),
			StoredValue(0, 0.1, 0, 0, 0, 0),
			StoredValue(0, 0.1, KLM_CHW, KLM_CHW, 1, 1),
			StoredValue(0, 0.1, KLM_CLW, KLM_CLW, 1, 1),
			StoredValue(0, 0.1, KLM_CHC, KLM_CHC, 1, 1),
			StoredValue(0, 0.1, KLM_CLC, KLM_CLC, 1, 1)
			);

		e2kInvertor = NestedInvertor(0, 1, con, rs, current, freq);
		e3kInvertor = NestedInvertor(0, 2, con, rs, current, freq);
	}

	{
		TriggerValue	con(0, MEM_ON, MEM_ON, 1, 1, 1, 1, 0, 1, 0);
		TriggerValue	rs(0, R_MEM_RS, PW_MEM_RS, 1, 1, 1, 1, 0, 1, 0);
		StoredValue		freq(0, K_ME_FULL, R_MEM_SV, PW_MEM_SV, 1, 1);
		RangeParameter	current(
			StoredValue(0, 0.1, MEM_C, 0, 0, 1),
			StoredValue(0, 0.1, 0, 0, 0, 0),
			StoredValue(0, 0.1, MEM_CHW, MEM_CHW, 1, 1),
			StoredValue(0, 0.1, MEM_CLW, MEM_CLW, 1, 1),
			StoredValue(0, 0.1, MEM_CHC, MEM_CHC, 1, 1),
			StoredValue(0, 0.1, MEM_CLC, MEM_CLC, 1, 1)
			);

		e3mInvertor = NestedInvertor(0, 2, con, rs, current, freq);
	}

	{
		TriggerValue	con(0, PEZ1_ON, PEZ1_ON, 1, 1, 1, 1, 0, 1, 0);
		TriggerValue	rs(0, R_PEZ1_RS, PW_PEZ1_RS, 1, 1, 1, 1, 0, 1, 0);
		RangeParameter temp(
			StoredValue(0, 0.1, PEZ1_PV, 0, 0, 1),
			StoredValue(0, 0.1, R_PEZ1_SV, PW_PEZ1_SV, 1, 1),
			StoredValue(0, 0.1, PEZ1_PVHW, PEZ1_PVHW, 1, 1),
			StoredValue(0, 0.1, PEZ1_PVLW, PEZ1_PVLW, 1, 1),
			StoredValue(0, 0.1, PEZ1_PVHC, PEZ1_PVHC, 1, 1),
			StoredValue(0, 0.1, PEZ1_PVLC, PEZ1_PVLC, 1, 1)
			);

		e2pZone1 = NestedTermo(0, 1, con, rs, temp);
	}

	{
		TriggerValue	con(0, PEZ2_ON, PEZ2_ON, 1, 1, 1, 1, 0, 1, 0);
		TriggerValue	rs(0, R_PEZ2_RS, PW_PEZ2_RS, 1, 1, 1, 1, 0, 1, 0);
		RangeParameter temp(
			StoredValue(0, 0.1, PEZ2_PV, 0, 0, 1),
			StoredValue(0, 0.1, R_PEZ2_SV, PW_PEZ2_SV, 1, 1),
			StoredValue(0, 0.1, PEZ2_PVHW, PEZ2_PVHW, 1, 1),
			StoredValue(0, 0.1, PEZ2_PVLW, PEZ2_PVLW, 1, 1),
			StoredValue(0, 0.1, PEZ2_PVHC, PEZ2_PVHC, 1, 1),
			StoredValue(0, 0.1, PEZ2_PVLC, PEZ2_PVLC, 1, 1)
			);

		e2pZone2 = NestedTermo(0, 1, con, rs, temp);
	}

	{
		TriggerValue	con(0, PEZ3_ON, PEZ3_ON, 1, 1, 1, 1, 0, 1, 0);
		TriggerValue	rs(0, R_PEZ3_RS, PW_PEZ3_RS, 1, 1, 1, 1, 0, 1, 0);
		RangeParameter temp(
			StoredValue(0, 0.1, PEZ3_PV, 0, 0, 1),
			StoredValue(0, 0.1, R_PEZ3_SV, PW_PEZ3_SV, 1, 1),
			StoredValue(0, 0.1, PEZ3_PVHW, PEZ3_PVHW, 1, 1),
			StoredValue(0, 0.1, PEZ3_PVLW, PEZ3_PVLW, 1, 1),
			StoredValue(0, 0.1, PEZ3_PVHC, PEZ3_PVHC, 1, 1),
			StoredValue(0, 0.1, PEZ3_PVLC, PEZ3_PVLC, 1, 1)
			);

		e2pZone3 = NestedTermo(0, 1, con, rs, temp);
	}

	{
		TriggerValue	con(0, PEZ4_ON, PEZ4_ON, 1, 1, 1, 1, 0, 1, 0);
		TriggerValue	rs(0, R_PEZ4_RS, PW_PEZ4_RS, 1, 1, 1, 1, 0, 1, 0);
		RangeParameter temp(
			StoredValue(0, 0.1, PEZ4_PV, 0, 0, 1),
			StoredValue(0, 0.1, R_PEZ4_SV, PW_PEZ4_SV, 1, 1),
			StoredValue(0, 0.1, PEZ4_PVHW, PEZ4_PVHW, 1, 1),
			StoredValue(0, 0.1, PEZ4_PVLW, PEZ4_PVLW, 1, 1),
			StoredValue(0, 0.1, PEZ4_PVHC, PEZ4_PVHC, 1, 1),
			StoredValue(0, 0.1, PEZ4_PVLC, PEZ4_PVLC, 1, 1)
			);

		e2pZone4 = NestedTermo(0, 1, con, rs, temp);
	}

	{
		TriggerValue	con(0, PET_ON, PET_ON, 1, 1, 1, 1, 0, 1, 0);
		TriggerValue	rs(0, R_PET_RS, PW_PET_RS, 1, 1, 1, 1, 0, 1, 0);
		RangeParameter temp(
			StoredValue(0, 0.1, PET_PV, 0, 0, 1),
			StoredValue(0, 0.1, R_PET_SV, PW_PET_SV, 1, 1),
			StoredValue(0, 0.1, PET_PVHW, PET_PVHW, 1, 1),
			StoredValue(0, 0.1, PET_PVLW, PET_PVLW, 1, 1),
			StoredValue(0, 0.1, PET_PVHC, PET_PVHC, 1, 1),
			StoredValue(0, 0.1, PET_PVLC, PET_PVLC, 1, 1)
			);

		e2pTrace = NestedTermo(0, 1, con, rs, temp);
	}

	{
		TriggerValue	con(0, PEP_ON, PEP_ON, 1, 1, 1, 1, 0, 1, 0);
		RangeParameter	pressure(
			StoredValue(0, 1, PEP_PV, 0, 0, 1),
			StoredValue(0, 1, 0, 0, 0, 0),
			StoredValue(0, 1, PEP_PVHW, PEP_PVHW, 1, 1),
			StoredValue(0, 1, PEP_PVLW, PEP_PVLW, 1, 1),
			StoredValue(0, 1, PEP_PVHC, PEP_PVHC, 1, 1),
			StoredValue(0, 1, PEP_PVLC, PEP_PVLC, 1, 1)
			);

		e2pPressure = NestedPressure(0, 1, con, pressure);
	}

	{
		TriggerValue	con(0, KLZ1_ON, KLZ1_ON, 1, 1, 1, 1, 0, 1, 0);
		TriggerValue	rs(0, R_KLZ1_RS, PW_KLZ1_RS, 1, 1, 1, 1, 0, 1, 0);
		RangeParameter temp(
			StoredValue(0, 0.1, KLZ1_PV, 0, 0, 1),
			StoredValue(0, 0.1, R_KLZ1_SV, PW_KLZ1_SV, 1, 1),
			StoredValue(0, 0.1, KLZ1_PVHW, KLZ1_PVHW, 1, 1),
			StoredValue(0, 0.1, KLZ1_PVLW, KLZ1_PVLW, 1, 1),
			StoredValue(0, 0.1, KLZ1_PVHC, KLZ1_PVHC, 1, 1),
			StoredValue(0, 0.1, KLZ1_PVLC, KLZ1_PVLC, 1, 1)
			);

		e2kZone1 = NestedTermo(0, 1, con, rs, temp);
	}

	{
		TriggerValue	con(0, KLZ2_ON, KLZ2_ON, 1, 1, 1, 1, 0, 1, 0);
		TriggerValue	rs(0, R_KLZ2_RS, PW_KLZ2_RS, 1, 1, 1, 1, 0, 1, 0);
		RangeParameter temp(
			StoredValue(0, 0.1, KLZ2_PV, 0, 0, 1),
			StoredValue(0, 0.1, R_KLZ2_SV, PW_KLZ2_SV, 1, 1),
			StoredValue(0, 0.1, KLZ2_PVHW, KLZ2_PVHW, 1, 1),
			StoredValue(0, 0.1, KLZ2_PVLW, KLZ2_PVLW, 1, 1),
			StoredValue(0, 0.1, KLZ2_PVHC, KLZ2_PVHC, 1, 1),
			StoredValue(0, 0.1, KLZ2_PVLC, KLZ2_PVLC, 1, 1)
			);

		e2kZone2 = NestedTermo(0, 1, con, rs, temp);
	}

	{
		TriggerValue	con(0, KLZ3_ON, KLZ3_ON, 1, 1, 1, 1, 0, 1, 0);
		TriggerValue	rs(0, R_KLZ3_RS, PW_KLZ3_RS, 1, 1, 1, 1, 0, 1, 0);
		RangeParameter temp(
			StoredValue(0, 0.1, KLZ3_PV, 0, 0, 1),
			StoredValue(0, 0.1, R_KLZ3_SV, PW_KLZ3_SV, 1, 1),
			StoredValue(0, 0.1, KLZ3_PVHW, KLZ3_PVHW, 1, 1),
			StoredValue(0, 0.1, KLZ3_PVLW, KLZ3_PVLW, 1, 1),
			StoredValue(0, 0.1, KLZ3_PVHC, KLZ3_PVHC, 1, 1),
			StoredValue(0, 0.1, KLZ3_PVLC, KLZ3_PVLC, 1, 1)
			);

		e2kZone3 = NestedTermo(0, 1, con, rs, temp);
	}

	{
		TriggerValue	con(0, KLT_ON, KLT_ON, 1, 1, 1, 1, 0, 1, 0);
		TriggerValue	rs(0, R_KLT_RS, PW_KLT_RS, 1, 1, 1, 1, 0, 1, 0);
		RangeParameter temp(
			StoredValue(0, 0.1, KLT_PV, 0, 0, 1),
			StoredValue(0, 0.1, R_KLT_SV, PW_KLT_SV, 1, 1),
			StoredValue(0, 0.1, KLT_PVHW, KLT_PVHW, 1, 1),
			StoredValue(0, 0.1, KLT_PVLW, KLT_PVLW, 1, 1),
			StoredValue(0, 0.1, KLT_PVHC, KLT_PVHC, 1, 1),
			StoredValue(0, 0.1, KLT_PVLC, KLT_PVLC, 1, 1)
			);

		e2kTrace = NestedTermo(0, 1, con, rs, temp);
	}

	{
		TriggerValue	con(0, KLP_ON, KLP_ON, 1, 1, 1, 1, 0, 1, 0);
		RangeParameter	pressure(
			StoredValue(0, 1, KLP_PV, 0, 0, 1),
			StoredValue(0, 1, 0, 0, 0, 0),
			StoredValue(0, 1, KLP_PVHW, KLP_PVHW, 1, 1),
			StoredValue(0, 1, KLP_PVLW, KLP_PVLW, 1, 1),
			StoredValue(0, 1, KLP_PVHC, KLP_PVHC, 1, 1),
			StoredValue(0, 1, KLP_PVLC, KLP_PVLC, 1, 1)
			);

		e2kPressure = NestedPressure(0, 1, con, pressure);
	}

	{
		TriggerValue	con(0, PEZ1_ON, PEZ1_ON, 1, 1, 1, 1, 0, 1, 0);
		TriggerValue	rs(0, R_PEZ1_RS, PW_PEZ1_RS, 1, 1, 1, 1, 0, 1, 0);
		RangeParameter temp(
			StoredValue(0, 0.1, PEZ1_PV, 0, 0, 1),
			StoredValue(0, 0.1, R_PEZ1_SV, PW_PEZ1_SV, 1, 1),
			StoredValue(0, 0.1, PEZ1_PVHW, PEZ1_PVHW, 1, 1),
			StoredValue(0, 0.1, PEZ1_PVLW, PEZ1_PVLW, 1, 1),
			StoredValue(0, 0.1, PEZ1_PVHC, PEZ1_PVHC, 1, 1),
			StoredValue(0, 0.1, PEZ1_PVLC, PEZ1_PVLC, 1, 1)
			);

		e3pZone1 = NestedTermo(0, 2, con, rs, temp);
	}

	{
		TriggerValue	con(0, PEZ2_ON, PEZ2_ON, 1, 1, 1, 1, 0, 1, 0);
		TriggerValue	rs(0, R_PEZ2_RS, PW_PEZ2_RS, 1, 1, 1, 1, 0, 1, 0);
		RangeParameter temp(
			StoredValue(0, 0.1, PEZ2_PV, 0, 0, 1),
			StoredValue(0, 0.1, R_PEZ2_SV, PW_PEZ2_SV, 1, 1),
			StoredValue(0, 0.1, PEZ2_PVHW, PEZ2_PVHW, 1, 1),
			StoredValue(0, 0.1, PEZ2_PVLW, PEZ2_PVLW, 1, 1),
			StoredValue(0, 0.1, PEZ2_PVHC, PEZ2_PVHC, 1, 1),
			StoredValue(0, 0.1, PEZ2_PVLC, PEZ2_PVLC, 1, 1)
			);

		e3pZone2 = NestedTermo(0, 2, con, rs, temp);
	}

	{
		TriggerValue	con(0, PEZ3_ON, PEZ3_ON, 1, 1, 1, 1, 0, 1, 0);
		TriggerValue	rs(0, R_PEZ3_RS, PW_PEZ3_RS, 1, 1, 1, 1, 0, 1, 0);
		RangeParameter temp(
			StoredValue(0, 0.1, PEZ3_PV, 0, 0, 1),
			StoredValue(0, 0.1, R_PEZ3_SV, PW_PEZ3_SV, 1, 1),
			StoredValue(0, 0.1, PEZ3_PVHW, PEZ3_PVHW, 1, 1),
			StoredValue(0, 0.1, PEZ3_PVLW, PEZ3_PVLW, 1, 1),
			StoredValue(0, 0.1, PEZ3_PVHC, PEZ3_PVHC, 1, 1),
			StoredValue(0, 0.1, PEZ3_PVLC, PEZ3_PVLC, 1, 1)
			);

		e3pZone3 = NestedTermo(0, 2, con, rs, temp);
	}

	{
		TriggerValue	con(0, PEZ4_ON, PEZ4_ON, 1, 1, 1, 1, 0, 1, 0);
		TriggerValue	rs(0, R_PEZ4_RS, PW_PEZ4_RS, 1, 1, 1, 1, 0, 1, 0);
		RangeParameter temp(
			StoredValue(0, 0.1, PEZ4_PV, 0, 0, 1),
			StoredValue(0, 0.1, R_PEZ4_SV, PW_PEZ4_SV, 1, 1),
			StoredValue(0, 0.1, PEZ4_PVHW, PEZ4_PVHW, 1, 1),
			StoredValue(0, 0.1, PEZ4_PVLW, PEZ4_PVLW, 1, 1),
			StoredValue(0, 0.1, PEZ4_PVHC, PEZ4_PVHC, 1, 1),
			StoredValue(0, 0.1, PEZ4_PVLC, PEZ4_PVLC, 1, 1)
			);

		e3pZone4 = NestedTermo(0, 2, con, rs, temp);
	}

	{
		TriggerValue	con(0, PET_ON, PET_ON, 1, 1, 1, 1, 0, 1, 0);
		TriggerValue	rs(0, R_PET_RS, PW_PET_RS, 1, 1, 1, 1, 0, 1, 0);
		RangeParameter temp(
			StoredValue(0, 0.1, PET_PV, 0, 0, 1),
			StoredValue(0, 0.1, R_PET_SV, PW_PET_SV, 1, 1),
			StoredValue(0, 0.1, PET_PVHW, PET_PVHW, 1, 1),
			StoredValue(0, 0.1, PET_PVLW, PET_PVLW, 1, 1),
			StoredValue(0, 0.1, PET_PVHC, PET_PVHC, 1, 1),
			StoredValue(0, 0.1, PET_PVLC, PET_PVLC, 1, 1)
			);

		e3pTrace = NestedTermo(0, 2, con, rs, temp);
	}

	{
		TriggerValue	con(0, PEP_ON, PEP_ON, 1, 1, 1, 1, 0, 1, 0);
		RangeParameter	pressure(
			StoredValue(0, 1, PEP_PV, 0, 0, 1),
			StoredValue(0, 1, 0, 0, 0, 0),
			StoredValue(0, 1, PEP_PVHW, PEP_PVHW, 1, 1),
			StoredValue(0, 1, PEP_PVLW, PEP_PVLW, 1, 1),
			StoredValue(0, 1, PEP_PVHC, PEP_PVHC, 1, 1),
			StoredValue(0, 1, PEP_PVLC, PEP_PVLC, 1, 1)
			);

		e3pPressure = NestedPressure(0, 2, con, pressure);
	}

	{
		TriggerValue	con(0, KLZ1_ON, KLZ1_ON, 1, 1, 1, 1, 0, 1, 0);
		TriggerValue	rs(0, R_KLZ1_RS, PW_KLZ1_RS, 1, 1, 1, 1, 0, 1, 0);
		RangeParameter temp(
			StoredValue(0, 0.1, KLZ1_PV, 0, 0, 1),
			StoredValue(0, 0.1, R_KLZ1_SV, PW_KLZ1_SV, 1, 1),
			StoredValue(0, 0.1, KLZ1_PVHW, KLZ1_PVHW, 1, 1),
			StoredValue(0, 0.1, KLZ1_PVLW, KLZ1_PVLW, 1, 1),
			StoredValue(0, 0.1, KLZ1_PVHC, KLZ1_PVHC, 1, 1),
			StoredValue(0, 0.1, KLZ1_PVLC, KLZ1_PVLC, 1, 1)
			);

		e3kZone1 = NestedTermo(0, 2, con, rs, temp);
	}

	{
		TriggerValue	con(0, KLZ2_ON, KLZ2_ON, 1, 1, 1, 1, 0, 1, 0);
		TriggerValue	rs(0, R_KLZ2_RS, PW_KLZ2_RS, 1, 1, 1, 1, 0, 1, 0);
		RangeParameter temp(
			StoredValue(0, 0.1, KLZ2_PV, 0, 0, 1),
			StoredValue(0, 0.1, R_KLZ2_SV, PW_KLZ2_SV, 1, 1),
			StoredValue(0, 0.1, KLZ2_PVHW, KLZ2_PVHW, 1, 1),
			StoredValue(0, 0.1, KLZ2_PVLW, KLZ2_PVLW, 1, 1),
			StoredValue(0, 0.1, KLZ2_PVHC, KLZ2_PVHC, 1, 1),
			StoredValue(0, 0.1, KLZ2_PVLC, KLZ2_PVLC, 1, 1)
			);

		e3kZone2 = NestedTermo(0, 2, con, rs, temp);
	}

	{
		TriggerValue	con(0, KLZ3_ON, KLZ3_ON, 1, 1, 1, 1, 0, 1, 0);
		TriggerValue	rs(0, R_KLZ3_RS, PW_KLZ3_RS, 1, 1, 1, 1, 0, 1, 0);
		RangeParameter temp(
			StoredValue(0, 0.1, KLZ3_PV, 0, 0, 1),
			StoredValue(0, 0.1, R_KLZ3_SV, PW_KLZ3_SV, 1, 1),
			StoredValue(0, 0.1, KLZ3_PVHW, KLZ3_PVHW, 1, 1),
			StoredValue(0, 0.1, KLZ3_PVLW, KLZ3_PVLW, 1, 1),
			StoredValue(0, 0.1, KLZ3_PVHC, KLZ3_PVHC, 1, 1),
			StoredValue(0, 0.1, KLZ3_PVLC, KLZ3_PVLC, 1, 1)
			);

		e3kZone3 = NestedTermo(0, 2, con, rs, temp);
	}

	{
		TriggerValue	con(0, KLT_ON, KLT_ON, 1, 1, 1, 1, 0, 1, 0);
		TriggerValue	rs(0, R_KLT_RS, PW_KLT_RS, 1, 1, 1, 1, 0, 1, 0);
		RangeParameter temp(
			StoredValue(0, 0.1, KLT_PV, 0, 0, 1),
			StoredValue(0, 0.1, R_KLT_SV, PW_KLT_SV, 1, 1),
			StoredValue(0, 0.1, KLT_PVHW, KLT_PVHW, 1, 1),
			StoredValue(0, 0.1, KLT_PVLW, KLT_PVLW, 1, 1),
			StoredValue(0, 0.1, KLT_PVHC, KLT_PVHC, 1, 1),
			StoredValue(0, 0.1, KLT_PVLC, KLT_PVLC, 1, 1)
			);

		e3kTrace = NestedTermo(0, 2, con, rs, temp);
	}

	{
		TriggerValue	con(0, KLP_ON, KLP_ON, 1, 1, 1, 1, 0, 1, 0);
		RangeParameter	pressure(
			StoredValue(0, 1, KLP_PV, 0, 0, 1),
			StoredValue(0, 1, 0, 0, 0, 0),
			StoredValue(0, 1, KLP_PVHW, KLP_PVHW, 1, 1),
			StoredValue(0, 1, KLP_PVLW, KLP_PVLW, 1, 1),
			StoredValue(0, 1, KLP_PVHC, KLP_PVHC, 1, 1),
			StoredValue(0, 1, KLP_PVLC, KLP_PVLC, 1, 1)
			);

		e3kPressure = NestedPressure(0, 2, con, pressure);
	}

	{
		TriggerValue	con(0, MEZ1_ON, MEZ1_ON, 1, 1, 1, 1, 0, 1, 0);
		TriggerValue	rs(0, R_MEZ1_RS, PW_MEZ1_RS, 1, 1, 1, 1, 0, 1, 0);
		RangeParameter temp(
			StoredValue(0, 0.1, MEZ1_PV, 0, 0, 1),
			StoredValue(0, 0.1, R_MEZ1_SV, PW_MEZ1_SV, 1, 1),
			StoredValue(0, 0.1, MEZ1_PVHW, MEZ1_PVHW, 1, 1),
			StoredValue(0, 0.1, MEZ1_PVLW, MEZ1_PVLW, 1, 1),
			StoredValue(0, 0.1, MEZ1_PVHC, MEZ1_PVHC, 1, 1),
			StoredValue(0, 0.1, MEZ1_PVLC, MEZ1_PVLC, 1, 1)
			);

		e3mZone1 = NestedTermo(0, 2, con, rs, temp);
	}

	{
		TriggerValue	con(0, MEZ2_ON, MEZ2_ON, 1, 1, 1, 1, 0, 1, 0);
		TriggerValue	rs(0, R_MEZ2_RS, PW_MEZ2_RS, 1, 1, 1, 1, 0, 1, 0);
		RangeParameter temp(
			StoredValue(0, 0.1, MEZ2_PV, 0, 0, 1),
			StoredValue(0, 0.1, R_MEZ2_SV, PW_MEZ2_SV, 1, 1),
			StoredValue(0, 0.1, MEZ2_PVHW, MEZ2_PVHW, 1, 1),
			StoredValue(0, 0.1, MEZ2_PVLW, MEZ2_PVLW, 1, 1),
			StoredValue(0, 0.1, MEZ2_PVHC, MEZ2_PVHC, 1, 1),
			StoredValue(0, 0.1, MEZ2_PVLC, MEZ2_PVLC, 1, 1)
			);

		e3mZone2 = NestedTermo(0, 2, con, rs, temp);
	}

	{
		TriggerValue	con(0, MEZ3_ON, MEZ3_ON, 1, 1, 1, 1, 0, 1, 0);
		TriggerValue	rs(0, R_MEZ3_RS, PW_MEZ3_RS, 1, 1, 1, 1, 0, 1, 0);
		RangeParameter temp(
			StoredValue(0, 0.1, MEZ3_PV, 0, 0, 1),
			StoredValue(0, 0.1, R_MEZ3_SV, PW_MEZ3_SV, 1, 1),
			StoredValue(0, 0.1, MEZ3_PVHW, MEZ3_PVHW, 1, 1),
			StoredValue(0, 0.1, MEZ3_PVLW, MEZ3_PVLW, 1, 1),
			StoredValue(0, 0.1, MEZ3_PVHC, MEZ3_PVHC, 1, 1),
			StoredValue(0, 0.1, MEZ3_PVLC, MEZ3_PVLC, 1, 1)
			);

		e3mZone3 = NestedTermo(0, 2, con, rs, temp);
	}

	{
		TriggerValue	con(0, MET_ON, MET_ON, 1, 1, 1, 1, 0, 1, 0);
		TriggerValue	rs(0, R_MET_RS, PW_MET_RS, 1, 1, 1, 1, 0, 1, 0);
		RangeParameter temp(
			StoredValue(0, 0.1, MET_PV, 0, 0, 1),
			StoredValue(0, 0.1, R_MET_SV, PW_MET_SV, 1, 1),
			StoredValue(0, 0.1, MET_PVHW, MET_PVHW, 1, 1),
			StoredValue(0, 0.1, MET_PVLW, MET_PVLW, 1, 1),
			StoredValue(0, 0.1, MET_PVHC, MET_PVHC, 1, 1),
			StoredValue(0, 0.1, MET_PVLC, MET_PVLC, 1, 1)
			);

		e3mTrace = NestedTermo(0, 2, con, rs, temp);
	}

	{
		TriggerValue	con(0, MEP_ON, MEP_ON, 1, 1, 1, 1, 0, 1, 0);
		RangeParameter	pressure(
			StoredValue(0, 1, MEP_PV, 0, 0, 1),
			StoredValue(0, 1, 0, 0, 0, 0),
			StoredValue(0, 1, MEP_PVHW, MEP_PVHW, 1, 1),
			StoredValue(0, 1, MEP_PVLW, MEP_PVLW, 1, 1),
			StoredValue(0, 1, MEP_PVHC, MEP_PVHC, 1, 1),
			StoredValue(0, 1, MEP_PVLC, MEP_PVLC, 1, 1)
			);

		e3mPressure = NestedPressure(0, 2, con, pressure);
	}

}

PexLine::~PexLine()
{

}

void PexLine::alarmStop()
{
	cpu.setHeadDown(false);
	e2pInvertor.stop();
	e2kInvertor.stop();
	cpu.setGermo(true);
	e3pInvertor.stop();
	e3kInvertor.stop();
	e3mInvertor.stop();
	cpu.setTractorLock(false);
	tuInvertor.stop();
	sgInvertor.stop();
	uzg.stop();	
	if (2 == cpu.waterMode())
		cpu.setWaterMode(1);
}
