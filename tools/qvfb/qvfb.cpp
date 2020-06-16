/****************************************************************************
**
** Qt/Embedded virtual framebuffer
**
** Created : 20000605
**
** Copyright (C) 1992-2000 Trolltech AS.  All rights reserved.
**
** This file is part of the Qt GUI Toolkit.
**
** Licensees holding valid Qt Professional Edition licenses may use this
** file in accordance with the Qt Professional Edition License Agreement
** provided with the Qt Professional Edition.
**
** See http://www.trolltech.com/pricing.html or email sales@trolltech.com for
** information about the Professional Edition licensing.
**
*****************************************************************************/

#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qapplication.h>
#include <qmessagebox.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qfiledialog.h>
#include <qslider.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qradiobutton.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qcheckbox.h>
#include <qcursor.h>

#include "qvfb.h"
#include "qvfbview.h"
#include "qvfbratedlg.h"
#include "config.h"
#include "skin.h"

#include <unistd.h>


static const char * logo[] = {
/* width height ncolors chars_per_pixel */
"50 50 17 1",
/* colors */
"  c #000000",
". c #495808",
"X c #2A3304",
"o c #242B04",
"O c #030401",
"+ c #9EC011",
"@ c #93B310",
"# c #748E0C",
"$ c #A2C511",
"% c #8BA90E",
"& c #99BA10",
"* c #060701",
"= c #181D02",
"- c #212804",
"; c #61770A",
": c #0B0D01",
"/ c None",
/* pixels */
"/$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$/",
"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$",
"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$",
"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$",
"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$",
"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$",
"$$$$$$$$$$$$$$$$$$$$$$$+++$$$$$$$$$$$$$$$$$$$$$$$$",
"$$$$$$$$$$$$$$$$$$$@;.o=::=o.;@$$$$$$$$$$$$$$$$$$$",
"$$$$$$$$$$$$$$$$+#X*         **X#+$$$$$$$$$$$$$$$$",
"$$$$$$$$$$$$$$$#oO*         O  **o#+$$$$$$$$$$$$$$",
"$$$$$$$$$$$$$&.* OO              O*.&$$$$$$$$$$$$$",
"$$$$$$$$$$$$@XOO            * OO    X&$$$$$$$$$$$$",
"$$$$$$$$$$$@XO OO  O  **:::OOO OOO   X@$$$$$$$$$$$",
"$$$$$$$$$$&XO      O-;#@++@%.oOO      X&$$$$$$$$$$",
"$$$$$$$$$$.O  :  *-#+$$$$$$$$+#- : O O*.$$$$$$$$$$",
"$$$$$$$$$#*OO  O*.&$$$$$$$$$$$$+.OOOO **#$$$$$$$$$",
"$$$$$$$$+-OO O *;$$$$$$$$$$$&$$$$;*     o+$$$$$$$$",
"$$$$$$$$#O*  O .+$$$$$$$$$$@X;$$$+.O    *#$$$$$$$$",
"$$$$$$$$X*    -&$$$$$$$$$$@- :;$$$&-    OX$$$$$$$$",
"$$$$$$$@*O  *O#$$$$$$$$$$@oOO**;$$$#    O*%$$$$$$$",
"$$$$$$$;     -+$$$$$$$$$@o O OO ;+$$-O   *;$$$$$$$",
"$$$$$$$.     ;$$$$$$$$$@-OO OO  X&$$;O    .$$$$$$$",
"$$$$$$$o    *#$$$$$$$$@o  O O O-@$$$#O   *o$$$$$$$",
"$$$$$$+=    *@$$$$$$$@o* OO   -@$$$$&:    =$$$$$$$",
"$$$$$$+:    :+$$$$$$@-      *-@$$$$$$:    :+$$$$$$",
"$$$$$$+:    :+$$$$$@o* O    *-@$$$$$$:    :+$$$$$$",
"$$$$$$$=    :@$$$$@o*OOO      -@$$$$@:    =+$$$$$$",
"$$$$$$$-    O%$$$@o* O O    O O-@$$$#*   OX$$$$$$$",
"$$$$$$$. O *O;$$&o O*O* *O      -@$$;    O.$$$$$$$",
"$$$$$$$;*   Oo+$$;O*O:OO--      Oo@+=    *;$$$$$$$",
"$$$$$$$@*  O O#$$$;*OOOo@@-O     Oo;O*  **@$$$$$$$",
"$$$$$$$$X* OOO-+$$$;O o@$$@-    O O     OX$$$$$$$$",
"$$$$$$$$#*  * O.$$$$;X@$$$$@-O O        O#$$$$$$$$",
"$$$$$$$$+oO O OO.+$$+&$$$$$$@-O         o+$$$$$$$$",
"$$$$$$$$$#*    **.&$$$$$$$$$$@o      OO:#$$$$$$$$$",
"$$$$$$$$$+.   O* O-#+$$$$$$$$+;O    OOO:@$$$$$$$$$",
"$$$$$$$$$$&X  *O    -;#@++@#;=O    O    -@$$$$$$$$",
"$$$$$$$$$$$&X O     O*O::::O      OO    Oo@$$$$$$$",
"$$$$$$$$$$$$@XOO                  OO    O*X+$$$$$$",
"$$$$$$$$$$$$$&.*       **  O      ::    *:#$$$$$$$",
"$$$$$$$$$$$$$$$#o*OO       O    Oo#@-OOO=#$$$$$$$$",
"$$$$$$$$$$$$$$$$+#X:* *     O**X#+$$@-*:#$$$$$$$$$",
"$$$$$$$$$$$$$$$$$$$%;.o=::=o.#@$$$$$$@X#$$$$$$$$$$",
"$$$$$$$$$$$$$$$$$$$$$$$$+++$$$$$$$$$$$+$$$$$$$$$$$",
"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$",
"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$",
"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$",
"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$",
"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$",
"/$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$/",
};

Zoomer::Zoomer(QVFb* target) :
    qvfb(target)
{
    (new QVBoxLayout(this))->setAutoAdd(TRUE);
    QSlider *sl = new QSlider(10,64,1,32,Horizontal,this);
    connect(sl,SIGNAL(valueChanged(int)),this,SLOT(zoom(int)));
    label = new QLabel(this);
}

void Zoomer::zoom(int z)
{
    double d = (double)z/32.0;
    qvfb->setZoom(d);
    label->setText(QString::number(d,'g',2));
}

QVFb::QVFb( int display_id, int w, int h, int d, int r, const QString &skin, QWidget *parent,
	    const char *name, uint flags )
    : QMainWindow( parent, name, flags )
{
    currentSkinIndex = -1;
    findSkins(skin);
    zoomer = 0;
    imagesave = new QFileDialog(".", "*.png", this, 0, TRUE);
    imagesave->setSelection("snapshot.png");
    setIcon(QPixmap(logo));
    rateDlg = 0;
    view = 0;
#if QT_VERSION >= 0x030000
    // When compiling with Qt 3 we need to create the menu first to
    // avoid scroll bars in the main window
    createMenu( menuBar() );
    init( display_id, w, h, d, r, skin );
    enableCursor( TRUE );
#else
    init( display_id, w, h, d, r, skin );
    createMenu( menuBar() );
#endif
}

QVFb::~QVFb()
{
}

void QVFb::popupMenu()
{
    QPopupMenu *pm = new QPopupMenu( this );
    createMenu( pm );
    pm->exec(QCursor::pos());
}

void QVFb::init( int display_id, int pw, int ph, int d, int r, const QString& skin_name )
{
    setCaption( QString("Virtual framebuffer %1x%2 %3bpp Display :%4 Rotate %5")
		    .arg(pw).arg(ph).arg(d).arg(display_id).arg(r) );
    delete view;
    view = 0;
    skin = 0;
    skinscaleH = skinscaleV = 1.0;
    QVFbView::Rotation rot = ((r ==  90) ? QVFbView::Rot90  :
			     ((r == 180) ? QVFbView::Rot180 :
			     ((r == 270) ? QVFbView::Rot270 :
					   QVFbView::Rot0 )));
    if ( !skin_name.isEmpty() ) {
	bool vis = isVisible();
	int sw, sh;
	skin = new Skin( this, skin_name, sw, sh );
	if ( !pw ) pw = sw;
	if ( !ph ) ph = sh;
	if (skin && skin->isValid()){
    	    if ( vis ) hide();
    	    menuBar()->hide();
	    view = new QVFbView( display_id, pw, ph, d, rot, skin );
	    skin->setView( view );
	    view->setMargin( 0 );
	    view->setFrameStyle( QFrame::NoFrame );
	    view->setFixedSize( sw, sh );
	    setCentralWidget( skin );
	    adjustSize();
	    skinscaleH = (double)sw/pw;
	    skinscaleV = (double)sh/ph;
	    if ( skinscaleH != 1.0 || skinscaleH != 1.0 )
		setZoom(skinscaleH);
	    view->show();
	    if ( vis ) show();
	} else {	    
	    delete skin;
	    skin = 0;
	}
    }

    // If we failed to get a skin or we were not supplied
    //	    with one then fallback to a framebuffer without
    //	    a skin
    if (!skin){
	// Default size
	if ( !pw ) pw = 240;
	if ( !ph ) ph = 320;

     	if ( currentSkinIndex!=-1 ) {
	    clearMask();
	    reparent( 0, 0, pos(), TRUE );
	    //unset fixed size:
	    setMinimumSize(0,0);
	    setMaximumSize(QWIDGETSIZE_MAX,QWIDGETSIZE_MAX);
	}
	menuBar()->show();
	view = new QVFbView( display_id, pw, ph, d, rot, this );
	view->setMargin( 0 );
	view->setFrameStyle( QFrame::NoFrame );
	setCentralWidget( view );
#if QT_VERSION >= 0x030000
	ph += 2;					// avoid scrollbar
#endif
	resize(pw,menuBar()->height()+ph);
	view->show();
	// delete defaultbuttons.conf if it was left behind...
	unlink(QFileInfo(QString("/tmp/qtembedded-%1/defaultbuttons.conf").arg(view->displayId())).absFilePath().latin1());
    }
}

void QVFb::enableCursor( bool e )
{
    if ( skin && skin->hasCursor() ) {
	view->viewport()->setCursor( BlankCursor );
    } else {
	view->viewport()->setCursor( e ? ArrowCursor : BlankCursor );
    }
    viewMenu->setItemChecked( cursorId, e );
}

void QVFb::createMenu(QMenuData *menu)
{
    QPopupMenu *file = new QPopupMenu( this );
    file->insertItem( "&Configure...", this, SLOT(configure()), ALT+CTRL+Key_C );
    file->insertSeparator();
    file->insertItem( "&Save image...", this, SLOT(saveImage()), ALT+CTRL+Key_S );
    file->insertItem( "&Animation...", this, SLOT(toggleAnimation()), ALT+CTRL+Key_A );
    file->insertSeparator();
    file->insertItem( "&Quit", qApp, SLOT(quit()) );

    menu->insertItem( "&File", file );

    viewMenu = new QPopupMenu( this );
    viewMenu->setCheckable( true );
    cursorId = viewMenu->insertItem( "Show &Cursor", this, SLOT(toggleCursor()) );
    if ( view )	
	enableCursor(TRUE);
    viewMenu->insertItem( "&Refresh Rate...", this, SLOT(changeRate()) );
    viewMenu->insertSeparator();
    viewMenu->insertItem( "Zoom scale &0.5", this, SLOT(setZoomHalf()) );
    viewMenu->insertItem( "Zoom scale 0.75", this, SLOT(setZoom075()) );
    viewMenu->insertItem( "Zoom scale &1", this, SLOT(setZoom1()) );
    viewMenu->insertItem( "Zoom scale &2", this, SLOT(setZoom2()) );
    viewMenu->insertItem( "Zoom scale &3", this, SLOT(setZoom3()) );
    viewMenu->insertItem( "Zoom scale &4", this, SLOT(setZoom4()) );
    viewMenu->insertSeparator();
    viewMenu->insertItem( "Zoom scale...", this, SLOT(setZoom()) );

    menu->insertItem( "&View", viewMenu );

    QPopupMenu *help = new QPopupMenu( this );
    help->insertItem("About...", this, SLOT(about()));
    menu->insertSeparator();
    menu->insertItem( "&Help", help );
}

void QVFb::setZoom(double z)
{
    view->setZoom(z,z*skinscaleV/skinscaleH);
    if (skin) {
	skin->setZoom(z/skinscaleH);
	view->setFixedSize(
	    int(view->displayWidth()*z),
	    int(view->displayHeight()*z*skinscaleV/skinscaleH));
    }
}

void QVFb::setZoomHalf()
{
    setZoom(0.5);
}

void QVFb::setZoom075()
{
    setZoom(0.75);
}

void QVFb::setZoom1()
{
    setZoom(1);
}

void QVFb::setZoom()
{
    if ( !zoomer )
	zoomer = new Zoomer(this);
    zoomer->show();
}

void QVFb::setZoom2()
{
    setZoom(2);
}

void QVFb::setZoom3()
{
    setZoom(3);
}

void QVFb::setZoom4()
{
    setZoom(4);
}

void QVFb::saveImage()
{
    QImage img = view->image();
    if ( imagesave->exec() ) {
	QString filename = imagesave->selectedFile();
	if ( !!filename )
	    img.save(filename,"PNG");
    }
}

void QVFb::toggleAnimation()
{
    if ( view->animating() ) {
	view->stopAnimation();
    } else {
	QString filename = imagesave->getSaveFileName("animation.mng", "*.mng", this, "", "Save animation...");
	if ( !filename ) {
	    view->stopAnimation();
	} else {
	    view->startAnimation(filename);
	}
    }
}

void QVFb::toggleCursor()
{
    enableCursor( !viewMenu->isItemChecked( cursorId ) );
}

void QVFb::changeRate()
{
    if ( !rateDlg ) {
	rateDlg = new QVFbRateDialog( view->rate(), this );
	connect( rateDlg, SIGNAL(updateRate(int)), view, SLOT(setRate(int)) );
    }

    rateDlg->show();
}

void QVFb::about()
{
    QMessageBox::about(this, "About QVFB",
	"<h2>The Qt/Embedded Virtual X11 Framebuffer</h2>"
	"<p>This application runs under Qt/X11, emulating a simple framebuffer, "
	"which the Qt/Embedded server and clients can attach to just as if "
	"it was a hardware Linux framebuffer. "
	"<p>With the aid of this development tool, you can develop Qt/Embedded "
	"applications under X11 without having to switch to a virtual console. "
	"This means you can comfortably use your other development tools such "
	"as GUI profilers and debuggers."
    );
}

void QVFb::findSkins(const QString &currentSkin)
{
    skinnames.clear();
    skinfiles.clear();
    QDir dir(".","*.skin");
    const QFileInfoList *l = dir.entryInfoList();
    QFileInfo* fi;
    int i = 1; // "None" is already in list at index 0
    for (QFileInfoListIterator it(*l); (fi=*it); ++it) {
	skinnames.append(fi->baseName()); // should perhaps be in file
	skinfiles.append(fi->filePath());
	if ((fi->baseName() + ".skin") == currentSkin)
	    currentSkinIndex = i;
	i++;
    }
}

void QVFb::configure()
{
    config = new Config(this,0,TRUE);

    int w = view->displayWidth();
    int h = view->displayHeight();

    // Need to block signals, because we connect to animateClick(),
    // since QCheckBox doesn't have setChecked(bool) in 2.x.
    chooseSize(QSize(w,h));
    config->skin->insertStringList(skinnames);
    if (currentSkinIndex > 0)
	config->skin->setCurrentItem(currentSkinIndex);
    config->touchScreen->setChecked(view->touchScreenEmulation());
    config->lcdScreen->setChecked(view->lcdScreenEmulation());
    config->depth_1->setChecked(view->displayDepth()==1);
    config->depth_4gray->setChecked(view->displayDepth()==4);
    config->depth_8->setChecked(view->displayDepth()==8);
    config->depth_12->setChecked(view->displayDepth()==12);
    config->depth_16->setChecked(view->displayDepth()==16);
    config->depth_32->setChecked(view->displayDepth()==32);
    connect(config->skin, SIGNAL(activated(int)), this, SLOT(skinConfigChosen(int)));
    if ( view->gammaRed() == view->gammaGreen() && view->gammaGreen() == view->gammaBlue() ) {
	config->gammaslider->setValue(int(view->gammaRed()*400));
	config->rslider->setValue(100);
	config->gslider->setValue(100);
	config->bslider->setValue(100);
    } else {
	config->gammaslider->setValue(100);
	config->rslider->setValue(int(view->gammaRed()*400));
	config->gslider->setValue(int(view->gammaGreen()*400));
	config->bslider->setValue(int(view->gammaBlue()*400));
    }
    connect(config->gammaslider, SIGNAL(valueChanged(int)), this, SLOT(setGamma400(int)));
    connect(config->rslider, SIGNAL(valueChanged(int)), this, SLOT(setR400(int)));
    connect(config->gslider, SIGNAL(valueChanged(int)), this, SLOT(setG400(int)));
    connect(config->bslider, SIGNAL(valueChanged(int)), this, SLOT(setB400(int)));
    updateGammaLabels();

    double ogr=view->gammaRed(), ogg=view->gammaGreen(), ogb=view->gammaBlue();
    hide();
    if ( config->exec() ) {
	int id = view->displayId(); // not settable yet
	if ( config->size_176_220->isChecked() ) {
	    w=176; h=220;
	} else if ( config->size_240_320->isChecked() ) {
	    w=240; h=320;
	} else if ( config->size_320_240->isChecked() ) {
	    w=320; h=240;
	} else if ( config->size_640_480->isChecked() ) {
	    w=640; h=480;
	} else {
	    w=config->size_width->value();
	    h=config->size_height->value();
	}
	int d;
	if ( config->depth_1->isChecked() )
	    d=1;
	else if ( config->depth_4gray->isChecked() )
	    d=4;
	else if ( config->depth_8->isChecked() )
	    d=8;
	else if ( config->depth_12->isChecked() )
	    d=12;
	else if ( config->depth_16->isChecked() )
	    d=16;
	else
	    d=32;
	int skinIndex = config->skin->currentItem();
	if ( w != view->displayWidth() || h != view->displayHeight()
		|| d != view->displayDepth() || skinIndex != currentSkinIndex ) {
	    QVFbView::Rotation rot = view->displayRotation();
	    int r = ((rot == QVFbView::Rot90)  ?  90 :
		    ((rot == QVFbView::Rot180) ? 180 :
		    ((rot == QVFbView::Rot270) ? 270 : 0 )));
	    currentSkinIndex = skinIndex;
	    init( id, w, h, d, r, skinIndex > 0 ? skinfiles[skinIndex-1] : QString::null );
	}
	view->setTouchscreenEmulation( config->touchScreen->isChecked() );
	bool lcdEmulation = config->lcdScreen->isChecked();
	view->setLcdScreenEmulation( lcdEmulation );
	if ( lcdEmulation )
	    setZoom3();
    } else {
	view->setGamma(ogr, ogg, ogb);
    }
    show();
    delete config;
    config=0;
}

void QVFb::chooseSize(const QSize& sz)
{
    config->size_width->blockSignals(TRUE);
    config->size_height->blockSignals(TRUE);
    config->size_width->setValue(sz.width());
    config->size_height->setValue(sz.height());
    config->size_width->blockSignals(FALSE);
    config->size_height->blockSignals(FALSE);
    config->size_custom->setChecked(TRUE); // unless changed by settings below
    config->size_176_220->setChecked(sz == QSize(176,220));
    config->size_240_320->setChecked(sz == QSize(240,320));
    config->size_320_240->setChecked(sz == QSize(320,240));
    config->size_640_480->setChecked(sz == QSize(640,480));
}

void QVFb::skinConfigChosen(int i)
{
    if ( i ) {
	chooseSize(Skin::screenSize(skinfiles[i-1]));
    }
}

void QVFb::setGamma400(int n)
{
    double g = n/100.0;
    view->setGamma(config->rslider->value()/100.0*g,
                   config->gslider->value()/100.0*g,
                   config->bslider->value()/100.0*g);
    updateGammaLabels();
}

void QVFb::setR400(int n)
{
    double g = n/100.0;
    view->setGamma(config->rslider->value()/100.0*g,
                   view->gammaGreen(),
                   view->gammaBlue());
    updateGammaLabels();
}

void QVFb::setG400(int n)
{
    double g = n/100.0;
    view->setGamma(view->gammaRed(),
                   config->gslider->value()/100.0*g,
                   view->gammaBlue());
    updateGammaLabels();
}

void QVFb::setB400(int n)
{
    double g = n/100.0;
    view->setGamma(view->gammaRed(),
                   view->gammaGreen(),
                   config->bslider->value()/100.0*g);
    updateGammaLabels();
}

void QVFb::updateGammaLabels()
{
    config->rlabel->setText(QString::number(view->gammaRed(),'g',2));
    config->glabel->setText(QString::number(view->gammaGreen(),'g',2));
    config->blabel->setText(QString::number(view->gammaBlue(),'g',2));
}

QSize QVFb::sizeHint() const
{
    return QSize(int(view->displayWidth()*view->zoomH()),
	    int(menuBar()->height()+view->displayHeight()*view->zoomV()));
}
