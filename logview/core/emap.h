#ifndef EMAP_H
#define EMAP_H

#include <QWidget>
#include <QPaintEvent>
class EMap : public QWidget
{
	Q_OBJECT
public:
	explicit EMap(QWidget *parent = 0);
	void passData(QList<double> x,QList<double> y,QList<double> z);
protected:
	void paintEvent(QPaintEvent *e);
private:
	QList<double> m_x;
	QList<double> m_y;
	QList<double> m_z;
	double m_minX;
	double m_minY;
	double m_minZ;
	double m_maxX;
	double m_maxY;
	double m_maxZ;
signals:
	
public slots:
	
};

#endif // EMAP_H
