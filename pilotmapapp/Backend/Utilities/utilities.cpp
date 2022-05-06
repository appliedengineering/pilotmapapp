#include "utilities.h"

MainWindow* utilities::mainWindow = nullptr;

MainWindow* utilities::findMainWindow() {
	
	if (utilities::mainWindow)
		return utilities::mainWindow;

	for (QWidget* w : QApplication::topLevelWidgets()) {
		MainWindow* wnd = qobject_cast<MainWindow*>(w);
		if (wnd) {
			utilities::mainWindow = wnd;
			return wnd;
		}
	}

	return nullptr;
}

/*void utilities::changeFontSize(QLabel* l, int s) {
	QFont f = l->font();
	f.setPointSize(s);
	setFontAntialias(&f);
	l->setFont(f);
}
void utilities::changeFontSize(QPushButton* b, int s) {
	QFont f = b->font();
	f.setPointSize(s);
	setFontAntialias(&f);
	b->setFont(f);
}
void utilities::setFontAntialias(QFont* f) {
	f->setStyleStrategy(QFont::PreferAntialias);
}
void utilities::setFontLetterSpacing(QFont* f) {
	f->setLetterSpacing(Qt::absolute);
}*/

void utilities::setPaletteColor(QWidget* w, QPalette::ColorRole r, Qt::GlobalColor c, bool shouldNotAutoFillBackground) {
	QPalette pal = w->palette();

	pal.setColor(r, c);
	if (!shouldNotAutoFillBackground)
		w->setAutoFillBackground(true);
	
	w->setPalette(pal);
}

// https://stackoverflow.com/a/15289912/
void utilities::setWidgetRoundedCorner(QWidget* w, int r, std::vector<corners> noRoundCorners){
	QBitmap bmp(w->size());
    bmp.clear();
    QPainter painter(&bmp);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QColor(Qt::black));
    painter.setBrush(QColor(Qt::black));
    painter.drawRoundedRect(w->rect(), r, r, Qt::AbsoluteSize);
	
	for (int i = 0; i < noRoundCorners.size(); i++){
		switch (noRoundCorners[i])
		{	
		case topLeft:
			painter.drawRect(0, 0, r, r);
			break;
		
		case topRight:
			painter.drawRect(w->width() - r, 0, r, r);
			break;

		case bottomLeft:
			painter.drawRect(0, w->height() - r, r, r);
			break;

		case bottomRight:
			painter.drawRect(w->width() - r, w->height() - r, r, r);
			break;

		default:
			break;
		}
	}
	/*painter.drawRect(0, w->height() - r, r, r);
	painter.drawRect(w->width() - r, w->height() - r, r, r);*/
    
	w->setMask(bmp);
}

std::string utilities::createPaddingString(int n) {
	return std::string(" ", n);
}

double utilities::roundDouble(double a, int places){
	int p = pow(10, places);
	return ((double)(round(a * p)) / p);
}