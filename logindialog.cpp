#include <QtGui>
#include <QtSql>
#include "logindialog.h"

LoginDialog::LoginDialog(QWidget * parent, Qt::WFlags f) : QDialog(parent, f)
{
	setupUi(this);	
}

void LoginDialog::on_okButton_clicked()
{
	QSqlQuery query;
		
	query.prepare("select * from users where user = :user and password = :password");
	query.bindValue(":user", loginLineEdit->text());
	query.bindValue(":password", passwordLineEdit->text());
	query.exec();

	if (!query.next())
		QMessageBox::critical(this, QString("���� � �������"),
			QString("��� ������ ���������\n��� ������� ������ ������"));
	else
	{
		emit userChanged(query.value(0).toString(), query.value(2).toInt());
		loginLineEdit->clear();
		passwordLineEdit->clear();
		done(Accepted);
	}

}

void LoginDialog::on_cancelButton_clicked()
{
	loginLineEdit->clear();
	passwordLineEdit->clear();
}