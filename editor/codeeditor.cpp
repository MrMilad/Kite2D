#include <QtWidgets>
#include "codeeditor.h"

Completer *CodeEditor::completer = new Completer();
CodeEditor::CodeEditor(QWidget *parent) :
	QPlainTextEdit(parent), currScript(nullptr)
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

	QObject::connect(completer, SIGNAL(activated(QString)),
					 this, SLOT(insertCompletion(QString)));

	hlight = new Highlighter(document());
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

void CodeEditor::setCompleterModel(QStandardItemModel *Model) {
	bool inite = false;
	completer->setModel(Model);
	
	if (!inite) {
		completer->setSeparator(QLatin1String("."));
		completer->popup()->setStyleSheet("QToolTip { border: 1px solid #2c2c2c; background-color: #242424; color: white;}");
		completer->setMaxVisibleItems(9);
		completer->setCaseSensitivity(Qt::CaseInsensitive);
		completer->setWrapAround(false);

		// add lua keywords into the model
		QStringList luaKeys;
		luaKeys << "and" << "break" << "do"
			<< "false" << "for" << "function"
			<< "local" << "nil" << "not"
			<< "then" << "true" << "until"
			<< "else" << "elseif" << "in"
			<< "goto" << "if" << "return"
			<< "or" << "repeat" << "while"
			<< "end";

		for (auto it = luaKeys.begin(); it != luaKeys.end(); ++it) {
			Model->appendRow(new QStandardItem(QIcon(":/icons/key16"), (*it)));
		}

		inite = true;
	}
}

void CodeEditor::scriptEdit(Kite::KResource *Res) {
	if (Res->getResourceType() != "KScript") {
		return;
	}

	currScript = (Kite::KScript *)Res;
	clear();
	appendPlainText(currScript->getCode().c_str());

	show();
}

void CodeEditor::scriptDelete(Kite::KResource *Res) {
	if (Res == currScript) {
		clear();
		currScript = nullptr;
	}
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
	if (completer->widget() != this)
		return;
	QTextCursor tc = textCursor();
	tc.select(QTextCursor::WordUnderCursor);
	if (tc.selectedText() == ".") {
		tc.clearSelection();
	}
	tc.removeSelectedText();
	tc.insertText(completion.split(".").back());
	setTextCursor(tc);
}

QString CodeEditor::textUnderCursor() const {
	QTextCursor tc = textCursor();

	auto text = document()->toPlainText();
	bool needDot = true;
	unsigned int pos = 0;
	auto it = text.begin() + tc.position() - 1;

	if ((*it) == '.') {
		needDot = false;
		++pos;
		--it;
	}

	for (it; it != text.begin() - 1; --it) {
		if ((*it) == '.' && needDot) {
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
	completer->setWidget(this);
	QPlainTextEdit::focusInEvent(e);
}

void CodeEditor::keyPressEvent(QKeyEvent *e) {
	if (completer->popup()->isVisible()) {
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
		completer->popup()->hide();
		return;
	}

	if (completionPrefix != completer->completionPrefix()) {
		completer->setCompletionPrefix(completionPrefix);
		completer->popup()->setCurrentIndex(completer->completionModel()->index(0, 0));
	}
	QRect cr = cursorRect();
	cr.setWidth(completer->popup()->sizeHintForColumn(0)
				+ completer->popup()->verticalScrollBar()->sizeHint().width());
	completer->complete(cr); // popup it up!
}

void CodeEditor::highlightCurrentLine(){
    QList<QTextEdit::ExtraSelection> extraSelections;

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
    int blockNumber = block.blockNumber() + 6;
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
