/*
 *  Copyright ( c ) 2012
 *  name : mhogo mchungu
 *  email: mhogomchungu@gmail.com
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation,either version 2 of the License,or
 *  ( at your option ) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not,see <http://www.gnu.org/licenses/>.
 */

#include "dialogmsg.h"
#include "ui_dialogmsg.h"

DialogMsg::DialogMsg( QWidget * parent ) :
	QDialog( parent ),
	m_ui( new Ui::DialogMsg )
{
	m_ui->setupUi( this );
	if( parent != 0 )
		this->setFont( parent->font() );

	connect( m_ui->pbNo,SIGNAL( clicked() ),this,SLOT( pbNo() ) );
	connect( m_ui->pbYes,SIGNAL( clicked() ),this,SLOT( pbYes() ) );
	connect( m_ui->pbOk,SIGNAL( clicked() ),this,SLOT( pbOK() ) );
}

void DialogMsg::pbNo()
{
	m_status = ( int ) QMessageBox::No ;
	this->hide();
}

void DialogMsg::pbOK()
{
	m_status = ( int ) QMessageBox::Ok ;
	this->hide();
}

void DialogMsg::pbYes()
{
	m_status = ( int ) QMessageBox::Yes ;
	this->hide();
}

void DialogMsg::closeEvent( QCloseEvent * e )
{
	e->ignore();
	this->hide() ;
}

void DialogMsg::HideUI()
{
	m_status = -1 ;
	this->hide();
}

void DialogMsg::ShowUI( QString title,QString msg )
{
	m_ui->label->setText( msg );

	this->setWindowTitle( title );

	this->show();

	this->exec() ;
}

void DialogMsg::ShowPermissionProblem( QString device )
{
	Q_UNUSED( device ) ;
	QString msg = tr( "\
\"insufficent privilege to open device\" and related permission errors when\n\
attempting to operate on a device are primarily caused by not having proper \
privileges to open devices for reading or writing.\n\nmost systems will give necessary permissions \
when a user is a member of \"disk\" goups.If you get above errors,then add yourself to the group,log out and log back in and try again.\n\n\
Some operations can not be performed on devices marked as \"system partitions\" when a user is not root.\
Please see section 2 of the documentation at\n\nhttp://code.google.com/p/zulucrypt/wiki/FAQ\n\nfor further details." ) ;

	this->ShowUIInfo( tr( "INFORMATION" ),msg );
}

void DialogMsg::ShowPermissionProblem( QString msg,QString device )
{
	QString msg1 ;
	if( device.startsWith( QString( "/dev/" ) ) ){
		 msg1 = QString( "\
system permission policy does not give you %1 access to the device.\n\
Add yourself to \"disk\" group or a related group dealing with giving access to devices and try again after you have logged out and logged back in again.\n" ).arg( msg ) ;
	}else{
		msg1 = QString( "you do not seem to have proper permissions to access the encrypted file for %1" ).arg( msg ) ;
	}

	this->ShowUIOK( QString( "INFORMATION"),msg1 );
}

void DialogMsg::setDimentions( QString msg )
{
	int len = msg.size() ;

	if( len <= 30 ){
		this->setFixedSize( 270,90 );
		m_ui->label->setGeometry( 10,10,251,31 );
		m_ui->label->setFixedSize( m_ui->label->size() );
		m_ui->pbOk->setGeometry( 100,50,75,31 );
		m_ui->pbYes->setGeometry( 70,50,71,31 );
		m_ui->pbNo->setGeometry( 140,50,75,31 );
	}else if( len <= 130 ){
		this->setFixedSize( 372,118 );
		m_ui->label->setGeometry( 10,10,351,61 );
		m_ui->label->setFixedSize( m_ui->label->size() );
		m_ui->pbOk->setGeometry( 150,80,75,31 );
		m_ui->pbYes->setGeometry( 120,80,71,31 );
		m_ui->pbNo->setGeometry( 190,80,75,31 );
	}else if( len > 130 ){
		this->setFixedSize( 372,146 );
		m_ui->label->setGeometry( 10,10,351,91 );
		m_ui->label->setFixedSize( m_ui->label->size() );
		m_ui->pbOk->setGeometry( 150,110,75,31 );
		m_ui->pbYes->setGeometry( 120,110,71,31 );
		m_ui->pbNo->setGeometry( 190,110,75,31 );
	}
}

void DialogMsg::ShowLabels()
{
	m_ui->labelType->setHidden( false );
	m_ui->labelCipher->setHidden( false );
	m_ui->labelKeySize->setHidden( false );
	m_ui->labelDevice->setHidden( false );
	m_ui->labelLoop->setHidden( false );
	m_ui->labelOffset->setHidden( false );
	m_ui->labelSize->setHidden( false );
	m_ui->labelMode->setHidden( false );
	m_ui->labelFs->setHidden( false );
	m_ui->labelSize_2->setHidden( false );
	m_ui->labelUsed->setHidden( false );
	m_ui->labelUnused->setHidden( false );
	m_ui->labelUsedPerc->setHidden( false );
	m_ui->labelActiveSlots->setHidden( false );
}

void DialogMsg::HideLabels()
{
	m_ui->labelActiveSlots->setHidden( true );
	m_ui->labelType->setHidden( true );
	m_ui->labelCipher->setHidden( true );
	m_ui->labelKeySize->setHidden( true );
	m_ui->labelDevice->setHidden( true );
	m_ui->labelLoop->setHidden( true );
	m_ui->labelOffset->setHidden( true );
	m_ui->labelSize->setHidden( true );
	m_ui->labelMode->setHidden( true );
	m_ui->labelFs->setHidden( true );
	m_ui->labelSize_2->setHidden( true );
	m_ui->labelUsed->setHidden( true );
	m_ui->labelUnused->setHidden( true );
	m_ui->labelUsedPerc->setHidden( true );
}

void DialogMsg::ShowUIVolumeProperties( QString title,QString msg )
{
	m_ui->pbYes->setHidden( true );
	m_ui->pbNo->setHidden( true );
	m_ui->pbOk->setHidden( false );

	m_ui->label->setHidden( true );

	msg.replace( "   ","" ) ;

	QStringList stl = msg.split("\n") ;

	this->setFixedSize( this->size() );

	m_ui->labelType->setText( stl.at( 0 ) );
	m_ui->labelCipher->setText( stl.at( 1 ) );
	m_ui->labelKeySize->setText( stl.at( 2 ) );
	m_ui->labelDevice->setText( stl.at( 3 ) );
	m_ui->labelLoop->setText( stl.at( 4 ) );
	m_ui->labelOffset->setText( stl.at( 5 ) );
	m_ui->labelSize->setText( stl.at( 6 ) );
	m_ui->labelMode->setText( stl.at( 7 ) );
	m_ui->labelActiveSlots->setText( stl.at( 8 ) );
	m_ui->labelFs->setText( stl.at( 9 ) );
	m_ui->labelSize_2->setText( stl.at( 10 ) );
	m_ui->labelUsed->setText( stl.at( 11 ) );
	m_ui->labelUnused->setText( stl.at( 12 ) );
	m_ui->labelUsedPerc->setText( stl.at( 13 ) );

	this->ShowUI( title,msg );
}

void DialogMsg::ShowUIInfo( QString title,QString msg )
{
	m_ui->pbYes->setHidden( true );
	m_ui->pbNo->setHidden( true );
	m_ui->pbOk->setHidden( false );

	m_ui->label->setAlignment( Qt::AlignLeft|Qt::AlignVCenter );
	this->setFixedSize( 562,338 );
	m_ui->label->setGeometry( 20,10,531,271 );
	m_ui->label->setFixedSize( m_ui->label->size() );
	m_ui->pbOk->setGeometry( 240,290,81,31 );

	this->HideLabels();
	this->ShowUI( title,msg );
}

void DialogMsg::ShowUIOK( QString title,QString msg )
{
	m_ui->pbYes->setHidden( true );
	m_ui->pbNo->setHidden( true );
	m_ui->pbOk->setHidden( false );

	this->HideLabels();
	this->setDimentions( msg );
	this->ShowUI( title,msg );
}

void DialogMsg::SetUpButtons()
{
	m_ui->pbYes->setHidden( false );
	m_ui->pbNo->setHidden( false );
	m_ui->pbOk->setHidden( true );
	this->HideLabels();
}

int DialogMsg::ShowUIYesNo( QString title,QString msg )
{
	this->SetUpButtons();
	this->setDimentions( msg );
	this->ShowUI( title,msg );
	return m_status;
}

int  DialogMsg::ShowUIYesNoDefaultNo( QString title,QString msg )
{
	this->SetUpButtons();
	this->setDimentions( msg );
	m_ui->pbNo->setFocus();
	this->ShowUI( title,msg );
	return m_status;
}

bool DialogMsg::ShowUIOKDoNotShowOption( QString title,QString msg )
{
	QCheckBox * checkBox = new QCheckBox( tr( "do not show this dialog again" ),this ) ;

	this->setFixedSize( 270,110 );

	checkBox->setGeometry( 30,40,251,31 );

	m_ui->label->setGeometry( 10,10,251,31 );
	m_ui->label->setFixedSize( m_ui->label->size() );
	m_ui->pbOk->setGeometry( 100,70,75,31 );

	m_ui->pbYes->setHidden( true );
	m_ui->pbNo->setHidden( true );
	m_ui->pbOk->setHidden( false );

	this->HideLabels();

	this->ShowUI( title,msg ) ;

	bool st = checkBox->isChecked() ;

	checkBox->deleteLater();

	return st ;
}

DialogMsg::~DialogMsg()
{
	delete m_ui;
}
