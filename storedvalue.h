#ifndef _STOREDVALUE_INCLUDED
#define _STOREDVALUE_INCLUDED

#include <QObject>

////////////////////////////////////////////////////////////

class StoredValue : public QObject
{
	Q_OBJECT

protected:

	int _rawValue;
	int _readAddr;
	int _writeAddr;
	double _coefficient;
	bool _storable;
	bool _loadable;

public:

	StoredValue(int rawValue = 0, double coefficient = 1.,
		int readAddr = 0, int writeAddr = 0,
		bool storable = true, bool loadable = true);

	StoredValue(const StoredValue & sv);
	StoredValue & operator = (const StoredValue & sv);

	int rawValue() const;
	virtual double scaledValue() const;
	double coefficient() const;
	int readAddr() const;
	int writeAddr() const;
	bool storable() const;
	bool loadable() const;

	virtual void setScaledValue(double scaledValue, bool emitSignals, bool overrideEmit = false);
	virtual void setRawValue(int rawValue, bool emitSignals, bool overrideEmit = false);
	virtual void setCoefficient(double coefficient, bool changeRawValue = true, bool emitSignals = true);

	virtual int rawForScaled(double scaledValue);
	virtual double scaledForRaw(int rawValue);

signals:

	void dumbSignal();

	void rawValueChanged(int);
	void scaledValueChanged(double);
	void valueChanged(StoredValue &);
	void valueChanged();
};


#endif /* _STOREDVALUE_INCLUDED */
