#include "QCodeEditor.h"
#include <QStyleOption>
#include "QApplication"
#include "QStyle"
#include "private\qstylesheetstyle_p.h"
#include "Platform.h"
#include "CodeSearchEditor.h"

QCodeEditor::QCodeEditor(QsciLexer* lexer, QWidget* parent)
	:QsciScintilla(parent)
	, mLexer(lexer)
	, mApis(new QsciAPIs(lexer))
	, mSearchEditor(new CodeSearchEditor)
{
	setMinimumWidth(300);
	mSearchEditor->setParent(this);
	lexer->setAPIs(mApis);
	setFrameStyle(QFrame::NoFrame);
	//设置语法
	setLexer(lexer);//给QsciScintilla设置词法分析器
	//行号提示
	setMargins(1);
	setMarginsFont(QFont("新宋体", 11));
	setMarginType(0, QsciScintilla::NumberMargin);//设置编号为0的页边显示行号。
	markerDefine(QsciScintilla::MarkerSymbol::CircledPlus, 0);
	setMarginWidth(0, 30);//设置页边宽度
	setAutoCompletionSource(QsciScintilla::AcsAll);   //设置源，自动补全所有地方出现的
	setAutoCompletionCaseSensitivity(true);           //设置自动补全大小写敏感
	setAutoCompletionThreshold(1);                    //设置每输入一个字符就会出现自动补全的提示
	setTabWidth(4);
	setIndentationGuides(false);
	setTabIndents(true);
	setAutoIndent(true);
	autoCompleteFromAll();
	setCallTipsStyle(QsciScintilla::CallTipsNoAutoCompletionContext);
	setCallTipsVisible(0);
	setBraceMatching(QsciScintilla::StrictBraceMatch);
	mLexer->setDefaultColor(Qt::red);
	SendScintilla(QsciScintilla::SCI_SETHSCROLLBAR, 1);
	//    connect(this,&QsciScintilla::selectionChanged,this,[this](){
	//        int lineFrom,indexFrom,lineTo,indexTo;
	//        getSelection(&lineFrom,&indexFrom,&lineTo,&indexTo);
	//        int startPos=positionFromLineIndex(lineFrom,indexFrom);
	//        int endPos=positionFromLineIndex(lineTo,indexTo);
	//        this->fillIndicatorRange(lineFrom,indexFrom,lineTo,indexTo,INDIC_BOX);
	//        qDebug()<<this->selectedText()<<text(startPos,endPos);
	//    });
	mSearchEditor->setVisible(false);
	connect(mSearchEditor, &CodeSearchEditor::requestSearch, this, &QCodeEditor::searchCode);
	connect(mSearchEditor, &CodeSearchEditor::requestReplace, this, &QCodeEditor::replaceCode);
}

void QCodeEditor::showEvent(QShowEvent* event)
{
	QsciScintilla::showEvent(event);
	QStyleOption style;
	style.initFrom(this);

	QColor textColor = style.palette.color(QPalette::Text);
	QColor windowColor = style.palette.color(QPalette::Window);
	setMarginsForegroundColor(textColor);
	setMarginsBackgroundColor(style.palette.color(QPalette::Window));
	setMatchedBraceBackgroundColor(windowColor);
	setMatchedBraceForegroundColor(QColor(154, 183, 190));
	setUnmatchedBraceBackgroundColor(windowColor);
	setUnmatchedBraceForegroundColor(Qt::red);
	setCaretForegroundColor(textColor);
	setCallTipsBackgroundColor(windowColor);
	setSelectionBackgroundColor(QColor(154, 183, 190));
	setSelectionForegroundColor(textColor);
	mLexer->setDefaultPaper(windowColor);
	mLexer->setPaper(windowColor);
	mLexer->setColor(textColor);
}

void QCodeEditor::resizeEvent(QResizeEvent* e)
{
	QsciScintilla::resizeEvent(e);
	QRect rect = mSearchEditor->rect();
	rect.moveTopRight(this->rect().topRight());
	mSearchEditor->setGeometry(rect);
}

void QCodeEditor::keyPressEvent(QKeyEvent* e)
{
	QsciScintilla::keyPressEvent(e);
	if (e->modifiers().testFlag(Qt::KeyboardModifier::ControlModifier) && (e->key() == Qt::Key_F || e->key() == Qt::Key_H)) {
		mSearchEditor->showSearch(selectedText());
	}
}

void QCodeEditor::searchCode(const SearchContext& sctx)
{
	this->findFirst(sctx.text, sctx.useRegularExpression, sctx.isCaseSensitive, sctx.isWholeMatching, true);
}

void QCodeEditor::replaceCode(const SearchContext& sctx, const QString& dst)
{
	if (this->findFirst(sctx.text, sctx.useRegularExpression, sctx.isCaseSensitive, sctx.isWholeMatching, true)) {
		this->replace(dst);
	}
}