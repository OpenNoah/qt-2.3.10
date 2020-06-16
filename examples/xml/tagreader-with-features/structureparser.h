/*
$Id: qt/examples/xml/tagreader-with-features/structureparser.h   2.3.10   edited 2005-01-24 $
*/  

#include <qxml.h>
#include <qstack.h>

class QListView;
class QListViewItem;
class QString;

class StructureParser: public QXmlDefaultHandler
{
public:
    StructureParser( QListView * );
    bool startDocument();
    bool startElement( const QString&, const QString&, const QString& , 
                       const QXmlAttributes& );
    bool endElement( const QString&, const QString&, const QString& );

private:
    QStack<QListViewItem> stack;
    QListView * table;
};                   
