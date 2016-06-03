#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

	// Identifiers
	ident.setForeground(QColor(0x43C6DB));
	rule.pattern = QRegExp("\\b[a-zA-Z0-9_]+");
	rule.format = ident;
	highlightingRules.append(rule);

	// functions
	function.setForeground(QColor(0x728C00));
	rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
	rule.format = function;
	highlightingRules.append(rule);

	// keywords
    keyword.setForeground(QColor(0x1E90FF));
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
        rule.format = keyword;
        highlightingRules.append(rule);
    }

	// numbers
	number.setForeground(QColor(0xC11B17));
	rule.pattern = QRegExp("\\b[-+]?[0-9]*\\.?[0-9]+");
	rule.format = number;
	highlightingRules.append(rule);
	
	// double quotation
    dquotation.setForeground(QColor(0xE56717));
    rule.pattern = QRegExp("\"(\\.|[^\"])*\"");
    rule.format = dquotation;
    highlightingRules.append(rule);

	// single quotation
	squotation.setForeground(QColor(0xE56717));
	rule.pattern = QRegExp("\'(\\.|[^\'])*\'");
	rule.format = squotation;
	highlightingRules.append(rule);

	// single line comment
	singleLineComment.setForeground(Qt::green);
	rule.pattern = QRegExp("--[^\n]*");
	rule.format = singleLineComment;
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
