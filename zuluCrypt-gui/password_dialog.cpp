/*
 * 
 *  Copyright (c) 2011
 *  name : mhogo mchungu
 *  email: mhogomchungu@gmail.com
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "password_dialog.h"
#include "zulucrypt.h"

#include <Qt>
#include <QObject>
#include <QFileDialog>
#include <QDir>
#include <iostream>
#include <QRadioButton>
#include <QPushButton>
#include <QMessageBox>

#include "miscfunctions.h"

passwordDialog::passwordDialog(QTableWidget * table,QWidget *parent ) : QDialog(parent)
{
	m_ui = new Ui::PasswordDialog() ;
	m_ui->setupUi(this);

	this->setFixedSize(this->size());
	this->setWindowFlags(Qt::Window | Qt::Dialog);
	this->setFont(parent->font());

	m_ui->PushButtonMountPointPath->setIcon(QIcon(QString(":/folder.png")));

	m_isWindowClosable = true ;

	m_table = table ;

	connect(m_ui->PushButtonCancel,SIGNAL(clicked()),this,SLOT(HideUI())) ;
	connect(m_ui->PushButtonOpen,SIGNAL(clicked()),this,SLOT(buttonOpenClicked())) ;
	connect(m_ui->PushButtonMountPointPath,SIGNAL(clicked()),this,SLOT(mount_point()));
	connect(m_ui->PushButtonVolumePath,SIGNAL(clicked()),this,SLOT(file_path())) ;
	connect(m_ui->pushButtonPassPhraseFromFile,SIGNAL(clicked()),this,SLOT(clickedPassPhraseFromFileButton()));
	connect(m_ui->radioButtonPassPhraseFromFile,SIGNAL(clicked()),this,SLOT(passphraseFromFileOption())) ;
	connect(m_ui->radioButtonPassPhrase,SIGNAL(clicked()),this,SLOT(passphraseOption())) ;
	connect(m_ui->OpenVolumePath,SIGNAL(textChanged(QString)),this,SLOT(mountPointPath(QString)));
}

void passwordDialog::closeEvent(QCloseEvent *e)
{
	e->ignore();
	if( m_isWindowClosable == true )
		HideUI() ;
}

void passwordDialog::ShowUI(QString volumePath, QString mount_point)
{
	m_ui->OpenVolumePath->setText(volumePath);
	m_ui->checkBoxReadOnly->setChecked( true );
	m_ui->PassPhraseField->setFocus();
	m_ui->PassPhraseField->clear();
	m_ui->radioButtonPassPhrase->setChecked( true );
	m_ui->labelPassphrase->setText(tr("passphrase"));
	m_ui->PassPhraseField->setEchoMode(QLineEdit::Password);
	m_ui->pushButtonPassPhraseFromFile->setEnabled( false );
	m_ui->pushButtonPassPhraseFromFile->setIcon(QIcon(QString(":/passphrase.png")));
	m_ui->OpenVolumePath->setEnabled(false);
	m_ui->PushButtonVolumePath->setEnabled(false);
	m_ui->MountPointPath->setText(mount_point);
	if( volumePath.left(5) == QString("/dev/") || volumePath.left(5) == QString("UUID="))
		m_ui->PushButtonVolumePath->setIcon(QIcon(QString(":/partition.png")));
	else
		m_ui->PushButtonVolumePath->setIcon(QIcon(QString(":/file.png")));
	this->show();
}

void passwordDialog::ShowUI()
{
	m_ui->OpenVolumePath->clear();
	m_ui->OpenVolumePath->setFocus();
	m_ui->PassPhraseField->clear();
	m_ui->radioButtonPassPhrase->setChecked(true);
	m_ui->labelPassphrase->setText(tr("passphrase"));
	m_ui->pushButtonPassPhraseFromFile->setEnabled(false);
	m_ui->PassPhraseField->setEchoMode(QLineEdit::Password);
	m_ui->checkBoxReadOnly->setChecked(true);
	m_ui->OpenVolumePath->setEnabled(true);
	m_ui->PushButtonVolumePath->setEnabled(true);
	m_ui->PushButtonVolumePath->setIcon(QIcon(QString(":/file.png")));
	m_ui->pushButtonPassPhraseFromFile->setIcon(QIcon(QString(":/passphrase.png")));
	this->show();
}

void passwordDialog::mountPointPath(QString path)
{
	QString p = QDir::homePath() + QString("/") + path.split("/").last() ;
	m_ui->MountPointPath->setText(p) ;
}

void passwordDialog::passphraseOption()
{
	m_ui->PassPhraseField->setEchoMode(QLineEdit::Password);
	m_ui->PassPhraseField->clear();
	m_ui->pushButtonPassPhraseFromFile->setEnabled(false) ;
	m_ui->labelPassphrase->setText(tr("passphrase"));
	m_ui->pushButtonPassPhraseFromFile->setIcon(QIcon(QString(":/passphrase.png")));
}

void passwordDialog::passphraseFromFileOption()
{
	m_ui->PassPhraseField->setEchoMode(QLineEdit::Normal);
	m_ui->PassPhraseField->clear();
	m_ui->pushButtonPassPhraseFromFile->setEnabled(true) ;
	m_ui->labelPassphrase->setText(tr("key file"));
	m_ui->pushButtonPassPhraseFromFile->setIcon(QIcon(QString(":/keyfile.png")));
}

void passwordDialog::clickedPassPhraseFromFileButton()
{
	QString Z = QFileDialog::getOpenFileName(this,tr("Select passphrase file"),QDir::homePath(),0);
	m_ui->PassPhraseField->setText( Z );
}

void passwordDialog::clickedPartitionOption(QString dev)
{
	QString m_point = QDir::homePath() + QString("/") + dev.split("/").last();
	ShowUI(dev,m_point) ;
}

void passwordDialog::mount_point(void )
{	
	QString Z = QFileDialog::getExistingDirectory(this,tr("Select Path to mount point folder"),QDir::homePath(),QFileDialog::ShowDirsOnly) ;
	m_ui->MountPointPath->setText( Z );
}

void passwordDialog::file_path(void )
{	
	QString Z = QFileDialog::getOpenFileName(this,tr("Select encrypted volume"),QDir::homePath(),0);
	m_ui->OpenVolumePath->setText( Z );
}

void passwordDialog::HideUI()
{	
	this->hide();
	emit HideUISignal();
}

void passwordDialog::buttonOpenClicked(void )
{	
	bool A = m_ui->checkBoxReadOnly->isChecked() ;
	bool B = m_ui->radioButtonPassPhraseFromFile->isChecked() ;
	m_volumePath = m_ui->OpenVolumePath->text() ;
	QString mountPointPath = m_ui->MountPointPath->text() ;
	QString passPhraseField = m_ui->PassPhraseField->text() ;

	if(m_volumePath == QString("")){
		UIMessage(tr("ERROR"),tr("volume path field is empty")) ;
		m_ui->OpenVolumePath->setFocus();
		return ;
	}
	if( m_volumePath.mid(0,5) != QString("UUID=")){
		if( m_volumePath.mid(0,2) == QString("~/"))
			m_volumePath = QDir::homePath() + QString("/") + m_volumePath.mid(2) ;

		QDir d(m_volumePath) ;

		m_volumePath = d.canonicalPath() ;
		if( miscfunctions::exists( m_volumePath ) == false ){
			UIMessage(tr("ERROR"),tr("No file or device exist on given path")) ;
			return ;
		}
	}
	if(mountPointPath == QString("")){
		UIMessage(tr("ERROR"),tr("mount point path field is empty")) ;
		m_ui->MountPointPath->setFocus();
		return ;
	}
	if( mountPointPath.mid(0,2) == QString("~/"))
		mountPointPath = QDir::homePath() + QString("/") + mountPointPath.mid(2) ;

	QString pos = mountPointPath.mid(0,mountPointPath.lastIndexOf(QChar('/'))) ;
	if( miscfunctions::exists(pos) == false ){
		UIMessage(tr("ERROR"),tr("invalid path to mount point")) ;
		return ;
	}

	if( miscfunctions::exists(mountPointPath) == true ){
		UIMessage(tr("ERROR"),tr("mount point path is already taken")) ;
		return ;
	}
	if(passPhraseField == QString("")){
		if( B == true )
			UIMessage(tr("ERROR"),tr("key file field is empty")) ;
		else
			UIMessage(tr("ERROR"),tr("passphrase field is empty")) ;
		m_ui->PassPhraseField->setFocus();
		return ;
	}	
	if(m_ui->radioButtonPassPhraseFromFile->isChecked() == true){

		if( passPhraseField.mid(0,2) == QString("~/"))
			passPhraseField = QDir::homePath() +  QString("/") + passPhraseField.mid(2);

		if(miscfunctions::exists(passPhraseField) == false){
			UIMessage(tr("ERROR"),tr("invalid path to a key file")) ;
			return ;
		}
	}
	QString mode ;

	if ( A == true )
		mode = "ro" ;
	else
		mode = "rw" ;

	QString passtype ;

	if ( B == true )
		passtype = "-f" ;
	else
		passtype = "-p" ;

	passPhraseField.replace("\"","\"\"\"") ;

	QString vp = m_volumePath ;

	vp.replace("\"","\"\"\"") ;

	mountPointPath.replace("\"","\"\"\"") ;

	QString exe = QString(ZULUCRYPTzuluCrypt) + QString(" -o ") + QString("-d \"") + vp + QString("\" ") ;
	exe = exe + QString("-m \"") + mountPointPath + QString("\" -e ") + mode + QString(" ") + passtype ;
	exe = exe + QString(" \"") + passPhraseField + QString("\"");

	runInThread * ovt = new runInThread(exe) ;
	connect(ovt,SIGNAL(finished(int)),this,SLOT(threadfinished(int))) ;
	m_isWindowClosable = false ;
	disableAll();
	QThreadPool::globalInstance()->start(ovt);
}

void passwordDialog::disableAll()
{
	m_ui->checkBoxReadOnly->setEnabled(false);
	m_ui->groupBox->setEnabled(false);
	m_ui->labelMoutPointPath->setEnabled(false);
	m_ui->labelPassphrase->setEnabled(false);
	m_ui->labelVolumePath->setEnabled(false);
	m_ui->MountPointPath->setEnabled(false);
	m_ui->OpenVolumePath->setEnabled(false);
	m_ui->PassPhraseField->setEnabled(false);
	m_ui->PushButtonCancel->setEnabled(false);
	m_ui->PushButtonMountPointPath->setEnabled(false);
	m_ui->PushButtonOpen->setEnabled(false);
	m_ui->pushButtonPassPhraseFromFile->setEnabled(false);
	m_ui->PushButtonVolumePath->setEnabled(false);
	m_ui->radioButtonPassPhrase->setEnabled(false);
	m_ui->radioButtonPassPhraseFromFile->setEnabled(false);
	m_ui->radioButtonPassPhrase->setEnabled(false);
}

void passwordDialog::enableAll()
{
	m_ui->checkBoxReadOnly->setEnabled(true);
	m_ui->groupBox->setEnabled(true);
	m_ui->labelMoutPointPath->setEnabled(true);
	m_ui->labelPassphrase->setEnabled(true);
	m_ui->labelVolumePath->setEnabled(true);
	m_ui->MountPointPath->setEnabled(true);
	m_ui->OpenVolumePath->setEnabled(true);
	m_ui->PassPhraseField->setEnabled(true);
	m_ui->PushButtonCancel->setEnabled(true);
	m_ui->PushButtonMountPointPath->setEnabled(true);
	m_ui->PushButtonOpen->setEnabled(true);
	m_ui->pushButtonPassPhraseFromFile->setEnabled(true);
	m_ui->PushButtonVolumePath->setEnabled(true);
	m_ui->radioButtonPassPhrase->setEnabled(true);
	m_ui->radioButtonPassPhraseFromFile->setEnabled(true);
	m_ui->radioButtonPassPhrase->setEnabled(true);
}
void passwordDialog::UIMessage(QString title, QString message)
{
	QMessageBox m ;
	m.setFont(this->font());
	m.setParent(this);
	m.setWindowFlags(Qt::Window | Qt::Dialog);
	m.setText(message);
	m.setWindowTitle(title);
	m.addButton(QMessageBox::Ok);
	m.exec() ;
}

void passwordDialog::success(void)
{
	checkvolumetype * cvt = new checkvolumetype(m_ui->OpenVolumePath->text());
	connect(cvt,SIGNAL(done(QString)),this,SLOT(done(QString)));
	QThreadPool::globalInstance()->start(cvt) ;
}

void passwordDialog::done(QString type)
{
	miscfunctions::addItemToTableWithType(m_table,m_ui->OpenVolumePath->text(),m_ui->MountPointPath->text(),type);
	HideUI();
}

void passwordDialog::threadfinished(int status)
{
	m_isWindowClosable = true ;

	switch ( status ){
		case 0: success();
			return ;
		case 1 : UIMessage(tr("ERROR"),tr("failed to mount ntfs file system using ntfs-3g,is ntfs-3g package installed?" )) ;		break ;					
		case 2 : UIMessage(tr("ERROR"),tr("there seem to be an open volume accociated with given address" ));				break ;				
		case 3 : UIMessage(tr("ERROR"),tr("no file or device exist on given path" )) ; 							break ;		
		case 4 : UIMessage(tr("ERROR"),tr("wrong passphrase" ));					
			 enableAll();
			 m_ui->PassPhraseField->clear();
			 m_ui->PassPhraseField->setFocus();
			 return ;			
		case 5 : UIMessage(tr("ERROR"),tr("could not create mount point, invalid path or path already taken") ) ;			break ;		
		case 6 : UIMessage(tr("ERROR"),tr("passphrase file does not exist" ));								break ;	
		case 8 : UIMessage(tr("ERROR"),tr("failed to open volume" ));									break ;	
		case 9 : UIMessage(tr("ERROR"),tr("mount point path is already taken" ));							break ;					 
		case 10: UIMessage(tr("ERROR"),tr("\",\" ( comma ) is not a valid mount point" ));						break ;
		case 11: UIMessage(tr("ERROR"),tr("one or more required argument(s) for this operation is missing\n" ));			break ;				
		case 12: UIMessage(tr("ERROR"),tr("could not get a lock on /etc/mtab~" ));							break ;
		case 13: UIMessage(tr("ERROR"),tr("wrong argument for mode, valid options are \"rw\" or \"ro\"" ));				break ;
		case 14: UIMessage(tr("ERROR"),tr("could not get enought memory to hold the key file" ));					break ;
		case 15: UIMessage(tr("ERROR"),tr("failed to open volume and failed to close the mapper, advice to do it manunally" ));		break ;
		case 16: UIMessage(tr("ERROR"),tr("could not resolve full path of mount point" ));						break ;
		case 17: UIMessage(tr("ERROR"),tr("could not resolve full path of device address" ));						break ;
		case 18: UIMessage(tr("ERROR"),tr("-O and -m options can not be used together" ));						break ;
		case 19: UIMessage(tr("ERROR"),tr("insufficient privilege to create mount point" ));						break ;	
		case 20: UIMessage(tr("ERROR"),tr("insufficient privilege to open device" ));							break ;	
		case 21: UIMessage(tr("ERROR"),tr("insufficient privilege to create mount point" ));						break ;	
		case 22: UIMessage(tr("ERROR"),tr("insufficient privilege to open key file for reading" ));					break ;	
		case 110:UIMessage(tr("ERROR"),tr("can not find a partition that match presented UUID" ));					break ;
		default: UIMessage(tr("ERROR"),tr("unrecognized error with status number %1 encountered").arg(status ));	
	}
	enableAll();
}

passwordDialog::~passwordDialog()
{
	delete m_ui ;
}

