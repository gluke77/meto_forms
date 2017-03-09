#ifndef _RANGEPARAMETER_INCLUDED
#define _RANGEPARAMETER_INCLUDED

#include <QObject>

#include "storedvalue.h"

class RangeParameter : public QObject
{
	Q_OBJECT
	
public:

	enum State
	{
		Ok = 0,
		Warning,
		Critical
	};

private:

	StoredValue _currentValue;
	StoredValue _presetValue;
	StoredValue _hiWarnBound;
	StoredValue _loWarnBound;
	StoredValue _hiCriticalBound;
	StoredValue _loCriticalBound;

	State _state;

public:

	RangeParameter(const StoredValue & currentValue = StoredValue(),
		const StoredValue & presetValue = StoredValue(),
		const StoredValue & hiWarnBound = StoredValue(),
		const StoredValue & loWarnBound = StoredValue(),
		const StoredValue & hiCriticalBound = StoredValue(),
		const StoredValue & loCriticalBound = StoredValue());

	RangeParameter(const RangeParameter &);

	RangeParameter & operator = (const RangeParameter &);

	virtual ~RangeParameter();

	const StoredValue & currentValue() const
	{
		return _currentValue;
	};

	const StoredValue & presetValue() const
	{
		return _presetValue;
	};

	const StoredValue & hiWarnBound() const
	{
		return _hiWarnBound;
	};

	const StoredValue & loWarnBound() const
	{
		return _loWarnBound;
	};

	const StoredValue & hiCriticalBound() const
	{
		return _hiCriticalBound;
	};

	const StoredValue & loCriticalBound() const
	{
		return _loCriticalBound;
	};

	StoredValue & currentValue()
	{
		return _currentValue;
	};

	StoredValue & presetValue()
	{
		return _presetValue;
	};

	StoredValue & hiWarnBound()
	{
		return _hiWarnBound;
	};

	StoredValue & loWarnBound()
	{
		return _loWarnBound;
	};

	StoredValue & hiCriticalBound()
	{
		return _hiCriticalBound;
	};

	StoredValue & loCriticalBound()
	{
		return _loCriticalBound;
	};
	
	void setCurrentValue(const StoredValue &);
	void setPresetValue(const StoredValue &);
	void setHiWarnBound(const StoredValue &);
	void setLoWarnBound(const StoredValue &);
	void setHiCriticalBound(const StoredValue &);
	void setLoCriticalBound(const StoredValue &);

	State state() const;
	bool ok() const;
	bool warning() const;
	bool critical() const;

public slots:
	
	void dumbSlot() {};
	void checkCurrentValue(bool overrideEmit = false);

signals:

	void dumbSignal();

	void stateChanged(int);
	void valueOk();
	void valueOk(RangeParameter *);
	void valueWarning();
	void valueWarning(RangeParameter *);
	void valueCritical();
	void valueCritical(RangeParameter *);

};


#endif /* _RANGEPARAMETER_INCLUDED */
