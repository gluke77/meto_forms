#ifndef _COMTHREADS_INCLUDED
#define _COMTHREADS_INCLUDED

#include <QThread>
#include <QtGlobal>

#include "baseport.h"
#include "pexline.h"

#define SLEEP_DELAY	(200)

extern PexLine	pexline;

void startComThreads();

class MyThread : public QThread
{
	Q_OBJECT

public:
	bool		_continue;
	BasePort	* _port;

	MyThread(BasePort * port = 0, QObject * parent = 0) : QThread(parent), _continue(true), _port(port)
	{
	};

public slots:
	
	void stop()
	{
		_continue = false;
	};
};

class Com3Thread : public MyThread
{
	Q_OBJECT
	
public:

	Com3Thread(BasePort * port = 0) : MyThread(port) {};

	void run()
	{
		while (_continue)
		{
			pexline.cpu.poll(1);
			
			msleep(SLEEP_DELAY);
		}
	};
};

class Com4Thread : public MyThread
{
	Q_OBJECT

	static const int repeat_count = 20;


public:
	Com4Thread(BasePort * port = 0) : MyThread(port) {};

	void run()
	{
		int count = 0;
		
		while (_continue)
		{
			count++;
			if (count > 2)
				count = 0;

			pexline.sgInvertor.poll(0 == count);
			msleep(2 * SLEEP_DELAY);

			pexline.uzg.poll(0, 1 == count);
			msleep(2 * SLEEP_DELAY);

			pexline.sgZone1.poll(2 == count);
			msleep(2 * SLEEP_DELAY);

			pexline.sgZone2.poll(0 == count);
			msleep(2 * SLEEP_DELAY);
			
			pexline.uzg.poll(1, 1 == count);
			msleep(2 * SLEEP_DELAY);
			
			pexline.tuInvertor.poll(2 == count);
			msleep(2 * SLEEP_DELAY);

		}
	};
};

class Com5Thread : public MyThread
{
	Q_OBJECT
public:

	Com5Thread(BasePort * port = 0) : MyThread(port) {};

	void run()
	{
		int count = 0;
	
		while (_continue)
		{
			count++;
			if (count > 1)
				count = 0;

			pexline.tuInvertor.poll(0 == count);
			msleep(2 * SLEEP_DELAY);

			pexline.printer.poll(1 == count);
			msleep(2 * SLEEP_DELAY);

		}
	};
};

class Com6Thread : public MyThread
{
	Q_OBJECT
public:

	Com6Thread(BasePort * port = 0) : MyThread(port) {};

	void run()
	{
		static int count = 0;
		
		while (_continue)
		{
			count++;
			if (2 < count)
				count = 0;
		
			pexline.e2kInvertor.poll(1 /*0 == count*/); 
			msleep(SLEEP_DELAY);
			
			pexline.e2kPressure.poll(1 /*== count*/);
			msleep(SLEEP_DELAY);
			
			pexline.e2kTrace.poll(2 == count); 
			msleep(SLEEP_DELAY);
			
			pexline.e2kZone1.poll(0 == count); 
			msleep(SLEEP_DELAY);
			
			pexline.e2pInvertor.poll(1 /*0 == count*/); 
			msleep(SLEEP_DELAY);
			
			pexline.e2pPressure.poll(1 /*== count*/);
			msleep(SLEEP_DELAY);

			pexline.e2kZone2.poll(1 == count); 
			msleep(SLEEP_DELAY);
			
			pexline.e2kZone3.poll(2 == count); 
			msleep(SLEEP_DELAY);
			
			pexline.e2kInvertor.poll(1 /*0 == count*/); 
			msleep(SLEEP_DELAY);
			
			pexline.e2kPressure.poll(1 /*== count*/);
			msleep(SLEEP_DELAY);

			pexline.e2pTrace.poll(2 == count); 
			msleep(SLEEP_DELAY);
			
			pexline.e2pZone1.poll(0 == count); 
			msleep(SLEEP_DELAY);
			
			pexline.e2pInvertor.poll(1 /*0 == count*/); 
			msleep(SLEEP_DELAY);
			
			pexline.e2pPressure.poll(1 /*== count*/);
			msleep(SLEEP_DELAY);

			pexline.e2pZone2.poll(1 == count); 
			msleep(SLEEP_DELAY);
			
			pexline.e2pZone3.poll(2 == count);
			msleep(SLEEP_DELAY);
			
			pexline.e2pZone4.poll(0 == count); 
			msleep(SLEEP_DELAY);
		}
	};
};

class Com7Thread : public MyThread
{
	Q_OBJECT
public:
	Com7Thread(BasePort * port = 0) : MyThread(port) {};

	void run()
	{
		static int count = 0;

		while (_continue)
		{
			count++;
			if (2 < count)
				count = 0;

			pexline.e3kInvertor.poll(1 /*0 == count*/);  
			msleep(SLEEP_DELAY);
			
			pexline.e3kPressure.poll(1 /* == count*/);  
			msleep(SLEEP_DELAY);
			
			pexline.e3kTrace.poll(2 == count);  
			msleep(SLEEP_DELAY);
			
			pexline.e3kZone1.poll(0 == count);  
			msleep(SLEEP_DELAY);
			
			pexline.e3pInvertor.poll(1 /*0 == count*/);  
			msleep(SLEEP_DELAY);
			
			pexline.e3pPressure.poll(1 /*== count*/);  
			msleep(SLEEP_DELAY);

			pexline.e3kZone2.poll(1 == count);  
			msleep(SLEEP_DELAY);
			
			pexline.e3kZone3.poll(2 == count);  
			msleep(SLEEP_DELAY);
			
			pexline.e3mInvertor.poll(1 /*== count*/);  
			msleep(SLEEP_DELAY);
			
			pexline.e3mPressure.poll(2 /*== count*/);  
			msleep(SLEEP_DELAY);

			pexline.e3pTrace.poll(2 == count);  
			msleep(SLEEP_DELAY);
			
			pexline.e3pZone1.poll(0 == count);  
			msleep(SLEEP_DELAY);
			

			pexline.e3kInvertor.poll(1 /*0 == count*/);  
			msleep(SLEEP_DELAY);
			
			pexline.e3kPressure.poll(1 /* == count*/);  
			msleep(SLEEP_DELAY);

			pexline.e3pZone2.poll(1 == count);  
			msleep(SLEEP_DELAY);
			
			pexline.e3pZone3.poll(2 == count);  
			msleep(SLEEP_DELAY);
			
			pexline.e3pZone4.poll(0 == count); 
			msleep(SLEEP_DELAY);
			
			pexline.e3pInvertor.poll(1 /*0 == count*/);  
			msleep(SLEEP_DELAY);
			
			pexline.e3pPressure.poll(1 /*== count*/);  
			msleep(SLEEP_DELAY);
			
			pexline.e3mTrace.poll(0 == count);  
			msleep(SLEEP_DELAY);
			
			pexline.e3mZone1.poll(1 == count);  
			msleep(SLEEP_DELAY);
			
			pexline.e3mZone2.poll(2 == count);  
			msleep(SLEEP_DELAY);
			
			pexline.e3mZone3.poll(0 == count);  
			msleep(SLEEP_DELAY);
			
		}
	};
};

extern Com3Thread * com3Thread;
extern Com4Thread * com4Thread;
extern Com5Thread * com5Thread;
extern Com6Thread * com6Thread;
extern Com7Thread * com7Thread;


#endif // _COMTHREADS_INCLUDED

