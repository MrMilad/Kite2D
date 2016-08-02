#ifndef GLSLHLIGHT_H
#define GLSLHLIGHT_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>

QT_BEGIN_NAMESPACE
class QTextDocument;
QT_END_NAMESPACE

class GLSLHLight : public QSyntaxHighlighter {
    Q_OBJECT
public:
    GLSLHLight(QTextDocument *parent = 0);

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
    QTextCharFormat function;
    QTextCharFormat number;
    QTextCharFormat sharptag;
    QTextCharFormat ident;
};

#endif // GLSLHLIGHT_H
