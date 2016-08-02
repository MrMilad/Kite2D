#include "glslhlight.h"

GLSLHLight::GLSLHLight(QTextDocument *parent):
    QSyntaxHighlighter(parent)
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
	keywordPatterns << "\\battribute\\b" << "\\buniform\\b" << "\\bvarying\\b"
		<< "\\blayout\\b" << "\\bcentroid\\b" << "\\bflat\\b"
		<< "\\bsmooth\\b" << "\\bnoperspective\\b" << "\\bpatch\\b"
		<< "\\bsample\\b" << "\\bsubroutine\\b" << "\\bin\\b"
		<< "\\bout\\b" << "\\binout\\b" << "\\binvariant\\b"
		<< "\\bdiscard\\b" << "\\bmat2\\b" << "\\bmat3\\b"
		<< "\\bmat4\\b" << "\\bdmat2\\b" << "\\bdmat3\\b"
		<< "\\bdmat4\\b" << "\\bmat2x2\\b" << "\\bmat2x3\\b"
		<< "\\bmat2x4\\b" << "\\bdmat2x2\\b" << "\\bdmat2x3\\b"
		<< "\\bdmat2x4\\b" << "\\bmat3x2\\b" << "\\bmat3x3\\b"
		<< "\\bmat3x4\\b" << "\\bdmat3x2\\b" << "\\bdmat3x3\\b"
		<< "\\bdmat3x4\\b" << "\\bmat4x2\\b" << "\\bmat4x3\\b"
		<< "\\bmat4x4\\b" << "\\bdmat4x2\\b" << "\\bdmat4x3\\b"
		<< "\\bdmat4x4\\b" << "\\bvec2\\b" << "\\bvec3\\b"
		<< "\\bvec4\\b" << "\\bivec2\\b" << "\\bivec3\\b"
		<< "\\bivec4\\b" << "\\bbvec2\\b" << "\\bbvec3\\b"
		<< "\\bbvec4\\b" << "\\bdvec2\\b" << "\\bdvec3\\b"
		<< "\\bdvec4\\b" << "\\buvec2\\b" << "\\buvec3\\b"
		<< "\\buvec4\\b" << "\\blowp\\b" << "\\bmediump\\b"
		<< "\\bhighp\\b" << "\\bprecision\\b" << "\\bsampler1D\\b"
		<< "\\bsampler2D\\b" << "\\bsampler3D\\b" << "\\bsamplerCube\\b"
		<< "\\bsampler1DShadow\\b" << "\\bsampler2DShadow\\b" << "\\bsamplerCubeShadow\\b"
		<< "\\bsampler1DArray\\b" << "\\bsampler2DArray\\b" << "\\bsampler1DArrayShadow\\b"
		<< "\\bsampler2DArrayShadow\\b" << "\\bisampler1D\\b" << "\\bisampler2D\\b"
		<< "\\bisampler3D\\b" << "\\bisamplerCube\\b" << "\\bisampler1DArray\\b"
		<< "\\bisampler2DArray\\b" << "\\busampler1D\\b" << "\\busampler2D\\b"
		<< "\\busampler3D\\b" << "\\busamplerCube\\b" << "\\busampler1DArray\\b"
		<< "\\busampler2DArray\\b" << "\\bsampler2DRect\\b" << "\\bsampler2DRectShadow\\b"
		<< "\\bisampler2DRect\\b" << "\\busampler2DRect\\b" << "\\bsamplerBuffer\\b"
		<< "\\bisamplerBuffer\\b" << "\\busamplerBuffer\\b" << "\\bsampler2DMS\\b"
		<< "\\bisampler2DMS\\b" << "\\busampler2DMS\\b" << "\\bsampler2DMSArray\\b"
		<< "\\bisampler2DMSArray\\b" << "\\busampler2DMSArray\\b" << "\\bsamplerCubeArray\\b"
		<< "\\bsamplerCubeArrayShadow\\b" << "\\bisamplerCubeArray\\b" << "\\busamplerCubeArray\\b"
		<< "\\bvoid\\b";

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

    // sharp tag
    sharptag.setForeground(Qt::blue);
    rule.pattern = QRegExp("#[^\n]*");
    rule.format = sharptag;
    highlightingRules.append(rule);

    // single line comment
    singleLineComment.setForeground(Qt::green);
    rule.pattern = QRegExp("//[^\n]*");
    rule.format = singleLineComment;
    highlightingRules.append(rule);
}

void GLSLHLight::highlightBlock(const QString &text)
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
