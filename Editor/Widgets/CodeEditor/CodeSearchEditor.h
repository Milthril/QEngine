#ifndef CodeSearchEditor_h__
#define CodeSearchEditor_h__

#include <QWidget>
#include "QCodeEditor.h"

class QLineEdit;
class ImageButton;
class WinCloseButton;

class CodeSearchEditor :public QWidget {
	Q_OBJECT
public:
	CodeSearchEditor();
	using QWidget::QWidget;
	void showSearch(const QString& str = QString());
protected:
	void keyPressEvent(QKeyEvent* e) override;
	QCodeEditor::SearchContext getCurrentContext();
Q_SIGNALS:
	void requestSearch(const QCodeEditor::SearchContext& ctx);
	void requestReplace(const QCodeEditor::SearchContext& ctx, const QString& dst);
private:
	QLineEdit* mSearchEdit;
	ImageButton* btCaseSensitive;
	ImageButton* btWholeMatching;
	ImageButton* btUseRegExp;
	ImageButton* btNext;
	ImageButton* btLast;
	WinCloseButton* btClose;
	QLineEdit* mReplaceEdit;
	ImageButton* btReplaceNext;
};

#endif // CodeSearchEditor_h__
