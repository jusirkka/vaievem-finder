#include "unitwrappermodel.h"


UnitWrapperModel::UnitWrapperModel(QAbstractListModel* base)
    : mBase(base)
    , mCurrIndex(0)
{
    connect(mBase, SIGNAL(modelReset()), this, SLOT(reset()));
}

int UnitWrapperModel::rowCount(const QModelIndex&) const {
    if (mBase->rowCount() > 0) return 1;
    return 0;
}

QVariant UnitWrapperModel::data(const QModelIndex& index, int role) const {

    if (!index.isValid()) {
        return QVariant();
    }

    if (mCurrIndex < mBase->rowCount()) {
        return mBase->data(mBase->index(mCurrIndex), role);
    }

    return QVariant();
}

QHash<int, QByteArray> UnitWrapperModel::roleNames() const {
    return mBase->roleNames();
}


void UnitWrapperModel::next() {
    if (mCurrIndex + 1 < mBase->rowCount()) {
        beginResetModel();
        mCurrIndex += 1;
        endResetModel();
    }
}

void UnitWrapperModel::prev() {
    if (mCurrIndex - 1 >= 0) {
        beginResetModel();
        mCurrIndex -= 1;
        endResetModel();
    }
}


void UnitWrapperModel::reset() {
    beginResetModel();
    mCurrIndex = 0;
    endResetModel();
}
