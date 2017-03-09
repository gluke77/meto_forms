#include "termoframe.h"

#include <QPalette>
#include <QColor>

TermoFrame::TermoFrame(QWidget * parent) : QWidget(parent)
{
	setupUi(this);
	vboxLayout->setMargin(3);
	
	runStopButton->setBlinkable(true);
	preset->setBlinkable(true);
	
	_termo = 0;
	
	connect(current, SIGNAL(valueChanged()), this, SLOT(showDelta()));
	connect(preset, SIGNAL(valueChanged()), this, SLOT(showDelta()));
	
	linkButton->setChecked(true);
}

void TermoFrame::showDelta()
{
	delta->setValue(current->value() - preset->value());	
}

void TermoFrame::setLoCritBound(double bound)
{
	minDelta->setValue(preset->value() - bound);
	_termo->setMinCritDelta(minDelta->value());
}

void TermoFrame::setHiCritBound(double bound)
{
	maxDelta->setValue(bound - preset->value());
	_termo->setMaxCritDelta(maxDelta->value());
}

void TermoFrame::connectTermo(Termo * t)
{
	if (_termo)
	{
		disconnect(preset, SIGNAL(valueChanged(double)), _termo, SLOT(setTemperature(double)));
		disconnect(runStopButton, SIGNAL(clicked(bool)), _termo, SLOT(setRunStop(bool)));
		disconnect(linkButton, SIGNAL(clicked(bool)), _termo, SLOT(setConnected(bool)));
		disconnect(maxDelta, SIGNAL(valueChanged(double)), _termo, SLOT(setMaxCritDelta(double)));
		disconnect(minDelta, SIGNAL(valueChanged(double)), _termo, SLOT(setMinCritDelta(double)));
//		disconnect(maxDelta, SIGNAL(valueChanged(double)), _termo, SLOT(setMaxWarnDelta(double)));
//		disconnect(minDelta, SIGNAL(valueChanged(double)), _termo, SLOT(setMinWarnDelta(double)));

		disconnect(&(_termo->temperature().currentValue()), SIGNAL(scaledValueChanged(double)), current, SLOT(setValue(double)));
		disconnect(&(_termo->temperature().presetValue()), SIGNAL(scaledValueChanged(double)), preset, SLOT(setAlterValue(double)));
		disconnect(&(_termo->temperature().loCriticalBound()), SIGNAL(scaledValueChanged(double)), this, SLOT(setLoCritBound(double)));
		disconnect(&(_termo->temperature().hiCriticalBound()), SIGNAL(scaledValueChanged(double)), this, SLOT(setHiCritBound(double)));
		disconnect(&(_termo->tvConnected()), SIGNAL(trigged(bool)), linkButton, SLOT(setChecked(bool)));
		disconnect(&(_termo->runStop()), SIGNAL(trigged(bool)), runStopButton, SLOT(setAlterChecked(bool)));
	
		disconnect(&(_termo->temperature()), SIGNAL(valueOk()), delta, SLOT(setGreen()));
		disconnect(&(_termo->temperature()), SIGNAL(valueWarning()), delta, SLOT(setRed()));
		disconnect(&(_termo->temperature()), SIGNAL(valueCritical()), delta, SLOT(setRed()));	
	}

	if (!t)
		return;
		
	_termo = t;
		
	connect(preset, SIGNAL(valueChanged(double)), _termo, SLOT(setTemperature(double)));
	connect(runStopButton, SIGNAL(clicked(bool)), _termo, SLOT(setRunStop(bool)));
	connect(linkButton, SIGNAL(clicked(bool)), _termo, SLOT(setConnected(bool)));
	connect(maxDelta, SIGNAL(valueChanged(double)), _termo, SLOT(setMaxCritDelta(double)));
	connect(minDelta, SIGNAL(valueChanged(double)), _termo, SLOT(setMinCritDelta(double)));
//	connect(maxDelta, SIGNAL(valueChanged(double)), _termo, SLOT(setMaxWarnDelta(double)));
//	connect(minDelta, SIGNAL(valueChanged(double)), _termo, SLOT(setMinWarnDelta(double)));


	connect(&(_termo->temperature().currentValue()), SIGNAL(scaledValueChanged(double)), current, SLOT(setValue(double)));
	connect(&(_termo->temperature().presetValue()), SIGNAL(scaledValueChanged(double)), preset, SLOT(setAlterValue(double)));
	connect(&(_termo->temperature().loCriticalBound()), SIGNAL(scaledValueChanged(double)), this, SLOT(setLoCritBound(double)));
	connect(&(_termo->temperature().hiCriticalBound()), SIGNAL(scaledValueChanged(double)), this, SLOT(setHiCritBound(double)));
	connect(&(_termo->tvConnected()), SIGNAL(trigged(bool)), linkButton, SLOT(setChecked(bool)));
	connect(&(_termo->runStop()), SIGNAL(trigged(bool)), runStopButton, SLOT(setAlterChecked(bool)));
	
	connect(&(_termo->temperature()), SIGNAL(valueOk()), delta, SLOT(setGreen()));
	connect(&(_termo->temperature()), SIGNAL(valueWarning()), delta, SLOT(setRed()));
	connect(&(_termo->temperature()), SIGNAL(valueCritical()), delta, SLOT(setRed()));
}