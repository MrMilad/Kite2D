#include <QtWidgets>
#include "codeeditor.h"

CodeEditor::CodeEditor(Completer *Comp, QWidget *parent) :
	QPlainTextEdit(parent),
	snumber(0),
	comp(Comp)
{
	setLineWrapMode(QPlainTextEdit::LineWrapMode::NoWrap);
    lineNumberArea = new LineNumberArea(this);
	QFont font;
	font.setFamily("Consolas");
	font.setStyleHint(QFont::Monospace);
	font.setFixedPitch(true);
	font.setPointSize(10);
	setFont(font);

	QFontMetrics metrics(font);
	setTabStopWidth(4 * metrics.width(' '));

	auto p = palette();
	p.setColor(QPalette::Active, QPalette::Base, QColor(0x111111));
	p.setColor(QPalette::Inactive, QPalette::Base, QColor(0x111111));
	setPalette(p);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
}

int CodeEditor::lineNumberAreaWidth()
{
    int digits = 7;
    /*int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }*/

    int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;

    return space;
}

void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */){
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void CodeEditor::insertCompletion(const QString &completion) {
	if (comp->widget() != this)
		return;
	QTextCursor tc = textCursor();
	tc.movePosition(QTextCursor::MoveOperation::PreviousCharacter, QTextCursor::MoveMode::KeepAnchor);
	if (tc.selectedText() != "." && tc.selectedText() != ":") {
		tc.select(QTextCursor::SelectionType::WordUnderCursor);
		tc.removeSelectedText();
	} else {
		tc.movePosition(QTextCursor::MoveOperation::NextCharacter);
	}
	tc.insertText(completion.split(".").back());
	setTextCursor(tc);
}

QString CodeEditor::textUnderCursor() const {
	QTextCursor tc = textCursor();

	auto text = document()->toPlainText();
	bool needDot = true;
	unsigned int pos = 0;
	auto it = text.begin() + tc.position() - 1;

	if ((*it) == '.' || (*it) == ':') {
		needDot = false;
		++pos;
		--it;
	}

	for (it; it != text.begin() - 1; --it) {
		if (((*it) == '.' || (*it) == ':') && needDot) {
			++pos;
			needDot = false;
			continue;
		} else if (((*it) >= 65 && (*it) <= 90) ||		// A-Z
				   ((*it) >= 97 && (*it) <= 122) ||		// a-z
				   ((*it) >= 48 && (*it) <= 57)) {		// 0-9
			++pos;
			needDot = true;
			continue;
		}
		break;
	}
	
	tc.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, pos);
	return tc.selectedText();
}

void CodeEditor::resizeEvent(QResizeEvent *e){
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeEditor::focusInEvent(QFocusEvent *e) {
	comp->setWidget(this);
	QPlainTextEdit::focusInEvent(e);
}

void CodeEditor::keyPressEvent(QKeyEvent *e) {
	if (comp->popup()->isVisible()) {
		// The following keys are forwarded by the completer to the widget
		switch (e->key()) {
		case Qt::Key_Enter:
		case Qt::Key_Return:
		case Qt::Key_Escape:
		case Qt::Key_Tab:
		case Qt::Key_Backtab:
			e->ignore();
			return; // let the completer do default behavior
		default:
			break;
		}
	}

	bool isShortcut = ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_E); // CTRL+E
	if (!isShortcut) // do not process the shortcut when we have a completer
		QPlainTextEdit::keyPressEvent(e);

	const bool ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
	if ((ctrlOrShift && e->text().isEmpty()))
		return;

	bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;
	QString completionPrefix = textUnderCursor();

	if (!isShortcut && (hasModifier || e->text().isEmpty() || completionPrefix.isEmpty())){
		comp->popup()->hide();
		return;
	}

	if (completionPrefix != comp->completionPrefix()) {
		comp->setCompletionPrefix(completionPrefix);
		comp->popup()->setCurrentIndex(comp->completionModel()->index(0, 0));
	}
	QRect cr = cursorRect();
	cr.setWidth(comp->popup()->sizeHintForColumn(0)
				+ comp->popup()->verticalScrollBar()->sizeHint().width());
	comp->complete(cr); // popup it up!
}

void CodeEditor::highlightCurrentLine(){
    QList<QTextEdit::ExtraSelection> extraSelections;
	// line highlighter
    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

		QColor lineColor = QColor(Qt::black);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
		selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event){
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::black);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber() + snumber;
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(QColor("DodgerBlue"));
			painter.setFont(font());
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignCenter, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}
