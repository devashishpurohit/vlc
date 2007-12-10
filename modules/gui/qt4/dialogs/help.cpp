/*****************************************************************************
 * Help.cpp : Help and About dialogs
 ****************************************************************************
 * Copyright (C) 2007 the VideoLAN team
 * $Id$
 *
 * Authors: Jean-Baptiste Kempf <jb (at) videolan.org>
 *          R�mi Duraffort <ivoire (at) via.ecp.fr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#include "dialogs/help.hpp"
#include <vlc_about.h>
#include <vlc_update.h>

#include "dialogs_provider.hpp"

#include <vlc_intf_strings.h>

#include <QTextBrowser>
#include <QTabWidget>
#include <QFile>
#include <QLabel>
#include <QString>
#include <QDialogButtonBox>


HelpDialog *HelpDialog::instance = NULL;

HelpDialog::HelpDialog( intf_thread_t *_p_intf ) : QVLCFrame( _p_intf )
{
    setWindowTitle( qtr( "Help" ) );
    resize( 600, 560 );

    QGridLayout *layout = new QGridLayout( this );
    QTextBrowser *helpBrowser = new QTextBrowser( this );
    helpBrowser->setOpenExternalLinks( true );
    helpBrowser->setHtml( I_LONGHELP );
    QPushButton *closeButton = new QPushButton( qtr( "&Close" ) );
    closeButton->setDefault( true );

    layout->addWidget( helpBrowser, 0, 0, 1, 0 );
    layout->addWidget( closeButton, 1, 3 );

    BUTTONACT( closeButton, close() );
}

HelpDialog::~HelpDialog()
{
}
void HelpDialog::close()
{
    this->toggleVisible();
}

AboutDialog *AboutDialog::instance = NULL;

AboutDialog::AboutDialog( intf_thread_t *_p_intf) :  QVLCFrame( _p_intf )
{
    setWindowTitle( qtr( "About" ) );
    resize( 600, 500 );

    QGridLayout *layout = new QGridLayout( this );
    QTabWidget *tab = new QTabWidget( this );

    QPushButton *closeButton = new QPushButton( qtr( "&Close" ) );
    closeButton->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );
    closeButton->setDefault( true );

    QLabel *introduction = new QLabel(
            qtr( "Information about VLC media player." ) );
    QLabel *iconVLC = new QLabel;
    iconVLC->setPixmap( QPixmap( ":/vlc48.png" ) );
    layout->addWidget( iconVLC, 0, 0, 1, 1 );
    layout->addWidget( introduction, 0, 1, 1, 7 );
    layout->addWidget( tab, 1, 0, 1, 8 );
    layout->addWidget( closeButton, 2, 6, 1, 2 );

    /* Main Introduction */
    QWidget *infoWidget = new QWidget( this );
    QHBoxLayout *infoLayout = new QHBoxLayout( infoWidget );
    QLabel *infoLabel = new QLabel( "VLC media player " PACKAGE_VERSION "\n\n"
            "(c) 1996-2007 - the VideoLAN Team\n\n" +
            qtr( "VLC media player is a free media player, made by the "
                 "VideoLAN Team.\nIt is a standalone multimedia player, "
                 "encoder and streamer, that can read from many supports "
                 "(files, CDs, DVDs, networks, capture cards...) and that "
                 "works on many platforms.\n\n" )
            + qtr( "You are using the new Qt4 Interface.\n" )
            + qtr( "Compiled by " ) + qfu( VLC_CompileBy() )+ "@"
            + qfu( VLC_CompileDomain() ) + ".\n"
            + "Compiler: " + qfu( VLC_Compiler() ) + ".\n"
            + qtr( "Based on SVN revision: " ) + qfu( VLC_Changeset() )
            + ".\n\n"
            + qtr( "This program comes with NO WARRANTY, to the extent "
                "permitted by the law; read the distribution tab.\n\n" )
            + "The VideoLAN team <videolan@videolan.org> \n"
              "http://www.videolan.org/\n" );
    infoLabel->setWordWrap( infoLabel );

    QLabel *iconVLC2 = new QLabel;
    iconVLC2->setPixmap( QPixmap( ":/vlc128.png" ) );
    infoLayout->addWidget( iconVLC2 );
    infoLayout->addWidget( infoLabel );

    /* GPL License */
    QTextEdit *licenseEdit = new QTextEdit( this );
    licenseEdit->setFontFamily( "Monospace" );
    licenseEdit->setText( qfu( psz_license ) );
    licenseEdit->setReadOnly( true );

    /* People who helped */
    QWidget *thanksWidget = new QWidget( this );
    QVBoxLayout *thanksLayout = new QVBoxLayout( thanksWidget );

    QLabel *thanksLabel = new QLabel( qtr( "We would like to thank the whole "
                "community, the testers, our users and the following people "
                "(and the missing ones...) for their collaboration to "
                "provide the best software." ) );
    thanksLabel->setWordWrap( true );
    thanksLayout->addWidget( thanksLabel );
    QTextEdit *thanksEdit = new QTextEdit( this );
    thanksEdit->setText( qfu( psz_thanks ) );
    thanksEdit->setReadOnly( true );
    thanksLayout->addWidget( thanksEdit );

    /* People who wrote the software */
    QTextEdit *authorsEdit = new QTextEdit( this );
    authorsEdit->setText( qfu( psz_authors ) );
    authorsEdit->setReadOnly( true );

    /* add the tabs to the Tabwidget */
    tab->addTab( infoWidget, qtr( "General Info" ) );
    tab->addTab( authorsEdit, qtr( "Authors" ) );
    tab->addTab( thanksWidget, qtr("Thanks") );
    tab->addTab( licenseEdit, qtr("Distribution License") );

    BUTTONACT( closeButton, close() );
}

AboutDialog::~AboutDialog()
{
}
void AboutDialog::close()
{
    this->toggleVisible();
}


UpdateDialog *UpdateDialog::instance = NULL;

UpdateDialog::UpdateDialog( intf_thread_t *_p_intf ) : QVLCFrame( _p_intf )
{
    setWindowTitle( qtr( "Update" ) );
    resize( 320, 120 );

    QGridLayout *layout = new QGridLayout( this );

    updateBrowser = new QTextBrowser( this );
    updateBrowser->setOpenExternalLinks( true );
    updateBrowser->setHtml( qtr( "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=\"utf-8\" /></head> \
                                  <body><center>Push the update button to get the updates</center></body></html>" ) );
    
    QPushButton *closeButton = new QPushButton( qtr( "&Close" ) );
    QPushButton *updateButton = new QPushButton( qtr( "&Update List" ) );
    updateButton->setDefault( true );
    QDialogButtonBox *buttonBox = new QDialogButtonBox( Qt::Horizontal );
    buttonBox->addButton( updateButton, QDialogButtonBox::ActionRole );
    buttonBox->addButton( closeButton, QDialogButtonBox::AcceptRole );

    layout->addWidget( updateBrowser, 0, 0 );
    layout->addWidget( buttonBox, 1, 0 );

    BUTTONACT( updateButton, updateOrUpload() );
    BUTTONACT( closeButton, close() );

    p_update = update_New( _p_intf );
}

UpdateDialog::~UpdateDialog()
{
    update_Delete( p_update );
}

void UpdateDialog::close()
{
    toggleVisible();
}

void UpdateDialog::updateOrUpload()
{
    update_Check( p_update );

    if( update_CompareReleaseToCurrent( p_update ) == UpdateReleaseStatusNewer )
    {
        updateBrowser->setHtml( qtr( "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=\"utf-8\" /></head> \
                                      <body><center><p>" ) + qtu( (QString)p_update->release.psz_desc ) +
                                      qtr( "</p>You can download the latest version of VLC <a href=\"" ) +
                                      qtu( (QString)p_update->release.psz_url ) + qtr( "\">here</a></center></body></html>" ) );
    }
    else
    {
        updateBrowser->setHtml( "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=\"utf-8\" /></head> \
                                 <body><center>You have the latest version of VLC.</center></body></html>" );
    }
}
