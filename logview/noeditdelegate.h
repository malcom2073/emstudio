#ifndef NOEDITDELEGATE_H
#define NOEDITDELEGATE_H

#include <QStyledItemDelegate>

class NoEditDelegate: public QStyledItemDelegate {
    public:
      NoEditDelegate(QObject* parent=0): QStyledItemDelegate(parent) {}
      virtual QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
	return 0;
      }
    };
#endif // NOEDITDELEGATE_H
