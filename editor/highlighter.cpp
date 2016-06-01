#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

	// Identifiers
	identFormat.setForeground(QColor(0x43C6DB));
	rule.pattern = QRegExp("\\b[a-zA-Z0-9_]+");
	rule.format = identFormat;
	highlightingRules.append(rule);

	// functions
	functionFormat.setForeground(QColor(0x728C00));
	rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
	rule.format = functionFormat;
	highlightingRules.append(rule);

	// keywords
    keywordFormat.setForeground(QColor(0x1E90FF));
    QStringList keywordPatterns;
	keywordPatterns << "\\band\\b" << "\\bbreak\\b" << "\\bdo\\b"
					<< "\\bfalse\\b" << "\\bfor\\b" << "\\bfunction\\b"
					<< "\\blocal\\b" << "\\bnil\\b" << "\\bnot\\b"
					<< "\\bthen\\b" << "\\btrue\\b" << "\\buntil\\b"
					<< "\\belse\\b" << "\\belseif\\b" << "\\bin\\b"
					<< "\\bgoto\\b" << "\\bif\\b" << "\\breturn\\b"
					<< "\\bor\\b" << "\\brepeat\\b" << "\\bwhile\\b"
					<< "\\bend\\b";
    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

	// numbers
	numberFormat.setForeground(QColor(0xC11B17));
	rule.pattern = QRegExp("\\b[-+]?[0-9]*\\.?[0-9]+");
	rule.format = numberFormat;
	highlightingRules.append(rule);
	
	// single line comment
    singleLineCommentFormat.setForeground(Qt::green);
    rule.pattern = QRegExp("--[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);
	
	// double quotation
    dquotationFormat.setForeground(QColor(0xE56717));
    rule.pattern = QRegExp("\".*\"");
    rule.format = dquotationFormat;
    highlightingRules.append(rule);

	// single quotation
	squotationFormat.setForeground(QColor(0xE56717));
	rule.pattern = QRegExp("\'.*\'");
	rule.format = squotationFormat;
	highlightingRules.append(rule);
}

void Highlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
    setCurrentBlockState(0);
}
