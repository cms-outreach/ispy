/****************************************************************************
** Form implementation generated from reading ui file 'c:\Graphics\Projects\IGUANA\Ig_Modules\IgVis\src\IgQtSlicer.ui'
**
** Created: Tue Feb 18 14:15:47 2003
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "Ig_Modules/IgVis/interface/IgQtSlicer.h"

#include <qvariant.h>
#include <Ig_Extensions/IgQt/interface/IgSoQtThumbWheel.h>
#include <qbuttongroup.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

static const char* const image0_data[] = { 
"129 21 105 2",
".# c #000000",
".Y c #2d2525",
".a c #302727",
"#J c #392e2e",
".b c #443737",
".Z c #453838",
".0 c #473a3a",
"#I c #4f4141",
".c c #534444",
"#M c #584848",
".3 c #5d4c4c",
".d c #5f4e4e",
".1 c #614f4f",
".2 c #685555",
".e c #6a5757",
"#H c #6f5a5a",
".6 c #725d5d",
".f c #745f5f",
"#G c #755f5f",
".4 c #7b6464",
".g c #7d6666",
"#L c #7e6767",
".5 c #806969",
".h c #856d6d",
"## c #876e6e",
"#F c #8a7171",
".i c #8c7373",
"#E c #8e7474",
".j c #937878",
".7 c #977b7b",
"#f c #977c7c",
".k c #9a7e7e",
".8 c #9b7e7e",
".9 c #9e8181",
".l c #a08383",
"#. c #a28484",
"#n c #a48686",
".m c #a58787",
"#K c #a68888",
"#D c #a88a8a",
".n c #ab8c8c",
"#t c #ad8d8d",
"#y c #ae8e8e",
"#a c #ae8f8f",
"#x c #af8f8f",
".o c #b09090",
"#b c #b19191",
"#c c #b49393",
".p c #b59494",
"#d c #b69595",
"#e c #b99797",
".q c #ba9898",
"#C c #bb9999",
"#B c #bd9b9b",
".r c #be9b9b",
"#g c #c09d9d",
"#h c #c29e9e",
".s c #c29f9f",
"#i c #c4a0a0",
"#j c #c5a1a1",
".t c #c6a2a2",
"#k c #c7a3a3",
"#l c #c9a4a4",
".u c #caa5a5",
"#m c #cba6a6",
"#A c #cca7a7",
".v c #cea8a8",
"#o c #cfa9a9",
"#p c #d0aaaa",
".w c #d1abab",
"#q c #d3acac",
"#r c #d4adad",
"Qt c #d4d0c8",
".x c #d5aeae",
"#s c #d6afaf",
"#z c #d7b0b0",
".y c #d8b1b1",
"#u c #d9b1b1",
"#v c #d9b2b2",
"#w c #dab2b2",
".z c #dbb3b3",
".A c #deb6b6",
".B c #e1b8b8",
".C c #e3baba",
".D c #e6bcbc",
".E c #e9bebe",
".F c #ebc0c0",
".G c #edc2c2",
".H c #efc4c4",
".I c #f1c5c5",
".J c #f3c7c7",
".K c #f5c8c8",
".L c #f7caca",
".M c #f8cbcb",
".N c #facccc",
".O c #fbcece",
".P c #fdcfcf",
".Q c #fed0d0",
".R c #ffd1d1",
".S c #ffd2d2",
".T c #ffd3d3",
".U c #ffd4d4",
".V c #ffd5d5",
".W c #ffd6d6",
".X c #ffd7d7",
"QtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQt",
"QtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQt",
"QtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQt",
"QtQtQtQtQt.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#QtQtQtQtQtQt",
"QtQtQtQtQt.#.#.a.b.c.d.e.f.g.h.i.j.k.l.m.n.o.p.q.r.s.t.u.v.w.x.y.z.A.B.C.D.E.F.G.H.I.J.K.L.M.N.O.P.Q.R.S.T.T.U.V.V.W.W.X.X.X.X.X.X.X.X.X.X.X.X.W.W.V.V.U.T.T.S.R.Q.P.O.N.M.L.K.J.I.H.G.F.E.D.C.B.A.z.y.x.w.v.u.t.s.r.q.p.o.n.m.l.k.j.i.h.g.f.e.d.c.b.#QtQtQtQtQtQt",
"QtQtQtQtQt.#.#.a.b.c.d.e.f.g.h.i.j.k.l.m.n.o.p.q.r.s.t.u.v.w.x.y.z.A.B.C.D.E.F.G.H.I.J.K.L.M.N.O.P.Q.R.S.T.T.U.V.V.W.W.X.X.X.X.X.X.X.X.X.X.X.X.W.W.V.V.U.T.T.S.R.Q.P.O.N.M.L.K.J.I.H.G.F.E.D.C.B.A.z.y.x.w.v.u.t.s.r.q.p.o.n.m.l.k.j.i.h.g.f.e.d.c.b.#QtQtQtQtQtQt",
"QtQtQtQtQt.#.#.a.Y.Z.d.0.1.2.h.3.4.5.h.m.6.j.7.8.9#..t##.n#a#b#c#d#e.B#f#g#h#i#j#k#l#m.K#n#o#p.w#q#r.x#s#s.T#t#u#v#w#w.z.z.z.z.X#x.z.z.z.z.z.z#w.W#y#u.y#z#s#s.x#r#q.O#p#o.v#A#m#l#k#j.F#h#g#B#C#e#d#c#b.w.n#D.m#..9.8.p.j#E#F.h.5.j#G#H.2.1.e#I.Z#J.#QtQtQtQtQtQt",
"QtQtQtQtQt.#.#.a.Y.Z.d.0.1.2.h.3.4.5.h.m.6.j.7.8.9#..t##.n#a#b#c#d#e.B#f#g#h#i#j#k#l#m.K#n#o#p.w#q#r.x#s.T.T#t#u#v#w#w.z.z.z.X.X#x.z.z.z.z.z.z.W.W#y#u.y#z#s#s.x#r.P.O#p#o.v#A#m#l#k.S.F#h#g#B#C#e#d#c.D.w.n#D.m#..9#l.p.j#E#F.h#K.j#G#H.2#L.e#I.Z#J.#QtQtQtQtQtQt",
"QtQtQtQtQt.#.#.a.Y.Z.d.0.1.2.h.3.4.5.h.m.6.j.7.8.9#..t##.n#a#b#c#d#e.B#f#g#h#i#j#k#l#m.K#n#o#p.w#q#r.x#s.T.T#t#u#v#w#w.z.z.z.X.X#x.z.z.z.z.z.z.W.W#y#u.y#z#s#s.x#r.P.O#p#o.v#A#m#l#k.S.F#h#g#B#C#e#d#c.D.w.n#D.m#..9#l.p.j#E#F.h#K.j#G#H.2#L.e#I.Z#J.#QtQtQtQtQtQt",
"QtQtQtQtQt.#.#.a.Y.Z.d.0.1.2.h.3.4.5.h.m.6.j.7.8.9#..t##.n#a#b#c#d#e.B#f#g#h#i#j#k#l#m.K#n#o#p.w#q#r.x#s.T.T#t#u#v#w#w.z.z.z.X.X#x.z.z.z.z.z.z.W.W#y#u.y#z#s#s.x#r.P.O#p#o.v#A#m#l#k.S.F#h#g#B#C#e#d#c.D.w.n#D.m#..9#l.p.j#E#F.h#K.j#G#H.2#L.e#I.Z#J.#QtQtQtQtQtQt",
"QtQtQtQtQt.#.#.a.Y.Z.d.0.1.2.h.3.4.5.h.m.6.j.7.8.9#..t##.n#a#b#c#d#e.B#f#g#h#i#j#k#l#m.K#n#o#p.w#q#r.x#s.T.T#t#u#v#w#w.z.z.z.X.X#x.z.z.z.z.z.z.W.W#y#u.y#z#s#s.x#r.P.O#p#o.v#A#m#l#k.S.F#h#g#B#C#e#d#c.D.w.n#D.m#..9#l.p.j#E#F.h#K.j#G#H.2#L.e#I.Z#J.#QtQtQtQtQtQt",
"QtQtQtQtQt.#.#.a.Y.Z.d.0.1.2.h.3.4.5.h.m.6.j.7.8.9#..t##.n#a#b#c#d#e.B#f#g#h#i#j#k#l#m.K#n#o#p.w#q#r.x#s.T.T#t#u#v#w#w.z.z.z.X.X#x.z.z.z.z.z.z.W.W#y#u.y#z#s#s.x#r.P.O#p#o.v#A#m#l#k.S.F#h#g#B#C#e#d#c.D.w.n#D.m#..9#l.p.j#E#F.h#K.j#G#H.2#L.e#I.Z#J.#QtQtQtQtQtQt",
"QtQtQtQtQt.#.#.a.Y.Z.d.0.1.2.h.3.4.5.h.m.6.j.7.8.9#..t##.n#a#b#c#d#e.B#f#g#h#i#j#k#l#m.K#n#o#p.w#q#r.x#s.T.T#t#u#v#w#w.z.z.z.X.X#x.z.z.z.z.z.z.W.W#y#u.y#z#s#s.x#r.P.O#p#o.v#A#m#l#k.S.F#h#g#B#C#e#d#c.D.w.n#D.m#..9#l.p.j#E#F.h#K.j#G#H.2#L.e#I.Z#J.#QtQtQtQtQtQt",
"QtQtQtQtQt.#.#.a#J.Z.d#M.1.2.h#G.4.5.h.m#E.j.7.8.9#..t#D.n#a#b#c#d#e.B#B#g#h#i#j#k#l#m.K.v#o#p.w#q#r.x#s.T.T.y#u#v#w#w.z.z.z.X.X.z.z.z.z.z.z.z.W.W#v#u.y#z#s#s.x#r.P.O#p#o.v#A#m#l#k.S.F#h#g#B#C#e#d#c.D.w.n#D.m#..9#l.p.j#E#F.h#K.j#G#H.2#L.e#I.Z#J.#QtQtQtQtQtQt",
"QtQtQtQtQt.#.#.a.b.c.d.e.f.g.h.i.j.k.l.m.n.o.p.q.r.s.t.u.v.w.x.y.z.A.B.C.D.E.F.G.H.I.J.K.L.M.N.O.P.Q.R.S.T.T.U.V.V.W.W.X.X.X.X.X.X.X.X.X.X.X.X.W.W.V.V.U.T.T.S.R.Q.P.O.N.M.L.K.J.I.H.G.F.E.D.C.B.A.z.y.x.w.v.u.t.s.r.q.p.o.n.m.l.k.j.i.h.g.f.e.d.c.b.#QtQtQtQtQtQt",
"QtQtQtQtQt.#.#.a.b.c.d.e.f.g.h.i.j.k.l.m.n.o.p.q.r.s.t.u.v.w.x.y.z.A.B.C.D.E.F.G.H.I.J.K.L.M.N.O.P.Q.R.S.T.T.U.V.V.W.W.X.X.X.X.X.X.X.X.X.X.X.X.W.W.V.V.U.T.T.S.R.Q.P.O.N.M.L.K.J.I.H.G.F.E.D.C.B.A.z.y.x.w.v.u.t.s.r.q.p.o.n.m.l.k.j.i.h.g.f.e.d.c.b.#QtQtQtQtQtQt",
"QtQtQtQtQt.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#QtQtQtQtQtQt",
"QtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQt",
"QtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQt",
"QtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQt",
"QtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQt"};


/* 
 *  Constructs a IgQtSlicer which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
IgQtSlicer::IgQtSlicer( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    QPixmap image0( ( const char** ) image0_data );
    if ( !name )
	setName( "IgQtSlicer" );
    resize( 378, 149 ); 
    setCaption( trUtf8( "Slicer" ) );
    QWhatsThis::add( this, trUtf8( "Controls selective slicing along an axis" ) );
    IgQtSlicerLayout = new QVBoxLayout( this, 11, 6, "IgQtSlicerLayout"); 

    Layout23 = new QHBoxLayout( 0, 0, 6, "Layout23"); 

    SliceCenterThumbwheel = new IgSoQtThumbWheel( this, "SliceCenterThumbwheel" );
    Layout23->addWidget( SliceCenterThumbwheel );
    QSpacerItem* spacer = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout23->addItem( spacer );

    SliceCenterLabel = new QLabel( this, "SliceCenterLabel" );
    SliceCenterLabel->setText( trUtf8( "Slice Center" ) );
    Layout23->addWidget( SliceCenterLabel );
    QSpacerItem* spacer_2 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout23->addItem( spacer_2 );

    SliceCenterText = new QLineEdit( this, "SliceCenterText" );
    SliceCenterText->setFrameShape( QLineEdit::LineEditPanel );
    SliceCenterText->setFrameShadow( QLineEdit::Sunken );
    Layout23->addWidget( SliceCenterText );
    IgQtSlicerLayout->addLayout( Layout23 );

    Layout24 = new QHBoxLayout( 0, 0, 6, "Layout24"); 

    SliceThicknessThumbwheel = new IgSoQtThumbWheel( this, "SliceThicknessThumbwheel" );
    Layout24->addWidget( SliceThicknessThumbwheel );
    QSpacerItem* spacer_3 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout24->addItem( spacer_3 );

    SliceThicknessLabel = new QLabel( this, "SliceThicknessLabel" );
    SliceThicknessLabel->setText( trUtf8( "Slice Thickness" ) );
    Layout24->addWidget( SliceThicknessLabel );
    QSpacerItem* spacer_4 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout24->addItem( spacer_4 );

    SliceThicknessText = new QLineEdit( this, "SliceThicknessText" );
    Layout24->addWidget( SliceThicknessText );
    IgQtSlicerLayout->addLayout( Layout24 );
    QSpacerItem* spacer_5 = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
    IgQtSlicerLayout->addItem( spacer_5 );

    Layout40 = new QHBoxLayout( 0, 0, 6, "Layout40"); 

    SliceOnOff = new QCheckBox( this, "SliceOnOff" );
    SliceOnOff->setText( trUtf8( "Slice On" ) );
    Layout40->addWidget( SliceOnOff );

    SliceAxisGroup = new QButtonGroup( this, "SliceAxisGroup" );
    SliceAxisGroup->setFrameShape( QButtonGroup::Box );
    SliceAxisGroup->setTitle( trUtf8( "" ) );
    SliceAxisGroup->setExclusive( TRUE );
    SliceAxisGroup->setColumnLayout(0, Qt::Vertical );
    SliceAxisGroup->layout()->setSpacing( 6 );
    SliceAxisGroup->layout()->setMargin( 11 );
    SliceAxisGroupLayout = new QHBoxLayout( SliceAxisGroup->layout() );
    SliceAxisGroupLayout->setAlignment( Qt::AlignTop );

    ClipAxisTitle = new QLabel( SliceAxisGroup, "ClipAxisTitle" );
    ClipAxisTitle->setText( trUtf8( "Clip Axis" ) );
    SliceAxisGroupLayout->addWidget( ClipAxisTitle );

    xAxisButton = new QRadioButton( SliceAxisGroup, "xAxisButton" );
    xAxisButton->setText( trUtf8( "x" ) );
    SliceAxisGroupLayout->addWidget( xAxisButton );

    yAxisButton = new QRadioButton( SliceAxisGroup, "yAxisButton" );
    yAxisButton->setText( trUtf8( "y" ) );
    SliceAxisGroupLayout->addWidget( yAxisButton );

    zAxisButton = new QRadioButton( SliceAxisGroup, "zAxisButton" );
    zAxisButton->setText( trUtf8( "z" ) );
    zAxisButton->setChecked( TRUE );
    SliceAxisGroupLayout->addWidget( zAxisButton );
    Layout40->addWidget( SliceAxisGroup );

    CloseButton = new QPushButton( this, "CloseButton" );
    CloseButton->setText( trUtf8( "OK" ) );
    Layout40->addWidget( CloseButton );
    IgQtSlicerLayout->addLayout( Layout40 );

    // signals and slots connections
    connect( CloseButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( SliceCenterThumbwheel, SIGNAL( wheelMoved(float) ), this, SLOT( centerPositionSettingSlot(float) ) );
    connect( SliceThicknessThumbwheel, SIGNAL( wheelMoved(float) ), this, SLOT( thicknessSettingSlot(float) ) );
    connect( SliceOnOff, SIGNAL( toggled(bool) ), this, SLOT( sliceOnSlot(bool) ) );
    connect( SliceThicknessText, SIGNAL( returnPressed() ), this, SLOT( thicknessTextChangedSlot() ) );
    connect( SliceCenterText, SIGNAL( returnPressed() ), this, SLOT( centerPositionTextChangedSlot() ) );
    connect( SliceAxisGroup, SIGNAL( clicked(int) ), this, SLOT( axisChanged(int) ) );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
IgQtSlicer::~IgQtSlicer()
{
    // no need to delete child widgets, Qt does it all for us
}

void IgQtSlicer::centerPositionSettingSlot(float)
{
    qWarning( "IgQtSlicer::centerPositionSettingSlot(float): Not implemented yet!" );
}

void IgQtSlicer::centerPositionTextChangedSlot()
{
    qWarning( "IgQtSlicer::centerPositionTextChangedSlot(): Not implemented yet!" );
}

void IgQtSlicer::axisChanged(int)
{
    qWarning( "IgQtSlicer::axisChanged(int): Not implemented yet!" );
}

void IgQtSlicer::sliceOnSlot(bool)
{
    qWarning( "IgQtSlicer::sliceOnSlot(bool): Not implemented yet!" );
}

void IgQtSlicer::thicknessSettingSlot(float)
{
    qWarning( "IgQtSlicer::thicknessSettingSlot(float): Not implemented yet!" );
}

void IgQtSlicer::thicknessTextChangedSlot()
{
    qWarning( "IgQtSlicer::thicknessTextChangedSlot(): Not implemented yet!" );
}

