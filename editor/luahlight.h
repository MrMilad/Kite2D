#ifndef LUAHLIGHT_H
#define LUAHLIGHT_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>

QT_BEGIN_NAMESPACE
class QTextDocument;
QT_END_NAMESPACE

class LuaHLight : public QSyntaxHighlighter{
    Q_OBJECT
public:
    LuaHLight(QTextDocument *parent = 0);

protected:
    void highlightBlock(const QString &text) Q_DECL_OVERRIDE;

private:
    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QTextCharFormat keyword;
    QTextCharFormat singleLineComment;
    QTextCharFormat squotation;
	QTextCharFormat dquotation;
    QTextCharFormat function;
	QTextCharFormat number;
	QTextCharFormat ident;
};

#endif // LUAHLIGHT_H
