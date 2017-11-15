#ifndef UNITWRAPPERMODEL_H
#define UNITWRAPPERMODEL_H

#include <QAbstractListModel>


class UnitWrapperModel: public QAbstractListModel
{
    Q_OBJECT

public:


    UnitWrapperModel(QAbstractListModel* base);

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;


    ~UnitWrapperModel() = default;

    Q_INVOKABLE void next();
    Q_INVOKABLE void prev();

private slots:

    void reset();


private:

    QAbstractListModel* mBase;
    int mCurrIndex;
};

#endif // UNITWRAPPERMODEL_H
