#ifndef _PEXLINE_INCLUDED
#define _PEXLINE_INCLUDED

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

class PexLine  
{

public:
	
	Cpu				cpu;
	Uzg				uzg;
	Printer			printer;
	
	Invertor		tuInvertor;

	Termo			sgZone1;
	Termo			sgZone2;
	SgInvertor		sgInvertor;

	NestedTermo		e2pZone1;
	NestedTermo		e2pZone2;
	NestedTermo		e2pZone3;
	NestedTermo		e2pZone4;
	NestedTermo		e2pTrace;
	NestedPressure	e2pPressure;
	NestedInvertor	e2pInvertor;

	NestedTermo		e2kZone1;
	NestedTermo		e2kZone2;
	NestedTermo		e2kZone3;
	NestedTermo		e2kTrace;
	NestedPressure	e2kPressure;
	NestedInvertor	e2kInvertor;

	NestedTermo		e3pZone1;
	NestedTermo		e3pZone2;
	NestedTermo		e3pZone3;
	NestedTermo		e3pZone4;
	NestedTermo		e3pTrace;
	NestedPressure	e3pPressure;
	NestedInvertor	e3pInvertor;

	NestedTermo		e3kZone1;
	NestedTermo		e3kZone2;
	NestedTermo		e3kZone3;
	NestedTermo		e3kTrace;
	NestedPressure	e3kPressure;
	NestedInvertor	e3kInvertor;

	NestedTermo		e3mZone1;
	NestedTermo		e3mZone2;
	NestedTermo		e3mZone3;
	NestedTermo		e3mTrace;
	NestedPressure	e3mPressure;
	NestedInvertor	e3mInvertor;

	PexLine();
	virtual ~PexLine();
	
public slots:
	virtual void alarmStop();

};

#endif // _PEXLINE_INCLUDED
