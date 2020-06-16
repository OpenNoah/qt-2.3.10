/*
$Id: qt/examples/xml/tagreader/structureparser.h   2.3.10   edited 2005-01-24 $
*/  

#include <qxml.h>

class QString;

class StructureParser : public QXmlDefaultHandler
{
public:
    bool startDocument();
    bool startElement( const QString&, const QString&, const QString& , 
                       const QXmlAttributes& );
    bool endElement( const QString&, const QString&, const QString& );

private:
    QString indent;
};                   
