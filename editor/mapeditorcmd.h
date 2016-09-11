#ifndef MAPEDITORCMD_H
#define MAPEDITORCMD_H

/*class AppendText : public QUndoCommand {
public:
	AppendText(QString *doc, const QString &text)
		: m_document(doc), m_text(text) {
		setText("append text");
	}
	virtual void undo() {
		m_document->chop(m_text.length());
	}
	virtual void redo() {
		m_document->append(m_text);
	}
private:
	QString *m_document;
	QString m_text;
};*/

#endif // MAPEDITORCMD_H