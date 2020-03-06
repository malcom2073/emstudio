#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QWidget>
#include <QVariantMap>
namespace Ui {
class MapView;
}

class MapView : public QWidget
{
	Q_OBJECT
	
public:
	explicit MapView(QWidget *parent = 0);
	~MapView();
	void addItem(QString item);
	void setData(QList<QVariantMap> *data);
private:
	Ui::MapView *ui;
	QList<QVariantMap> *m_data;
private slots:
	void goButtonClicked();
};

#endif // MAPVIEW_H
