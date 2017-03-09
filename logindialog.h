#ifndef _LOGINDIALOG_INCLUDED
#define _LOGINDIALOG_INCLUDED

#include <QtGui>
#include "ui_LoginDialog.h"

class LoginDialog : public QDialog, Ui_LoginDialog
{
	Q_OBJECT
public:
	LoginDialog(QWidget * parent = 0, Qt::WFlags f = 0);

public slots:

	void on_okButton_clicked();
	void on_cancelButton_clicked();

signals:

	void userChanged(QString, int);
};


#endif /* _LOGINDIALOG_INCLUDED */

