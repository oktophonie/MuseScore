/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-CLA-applies
 *
 * MuseScore
 * Music Composition & Notation
 *
 * Copyright (C) 2021 MuseScore BVBA and others
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#ifndef MU_INSPECTOR_INSPECTORLISTMODEL_H
#define MU_INSPECTOR_INSPECTORLISTMODEL_H

#include <QAbstractListModel>

#include "libmscore/engravingitem.h"

#include "modularity/ioc.h"
#include "async/asyncable.h"
#include "context/iglobalcontext.h"
#include "models/abstractinspectormodel.h"

namespace mu::inspector {
class IElementRepositoryService;
class InspectorListModel : public QAbstractListModel, public mu::async::Asyncable
{
    Q_OBJECT

    INJECT(inspector, context::IGlobalContext, context)

public:
    explicit InspectorListModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

signals:
    void elementsModified();

private:
    enum RoleNames {
        InspectorSectionModelRole = Qt::UserRole + 1
    };

    void onNotationChanged();

    void setElementList(const QList<Ms::EngravingItem*>& selectedElementList, bool isRangeSelection = false);

    void buildModelsForEmptySelection();
    void buildModelsForSelectedElements(const ElementKeySet& selectedElementKeySet, bool isRangeSelection);

    void createModelsBySectionType(const QList<InspectorSectionType>& sectionTypeList, const ElementKeySet& selectedElementKeySet = {});
    void removeUnusedModels(const ElementKeySet& newElementKeySet,
                            const QList<InspectorSectionType>& exclusions = QList<InspectorSectionType>());

    bool isModelAllowed(const AbstractInspectorModel* model, const InspectorModelTypeSet& allowedModelTypes,
                        const InspectorSectionTypeSet& allowedSectionTypes) const;

    void sortModels();

    AbstractInspectorModel* modelBySectionType(InspectorSectionType sectionType) const;

    QList<AbstractInspectorModel*> m_modelList;

    IElementRepositoryService* m_repository = nullptr;
};
}

#endif // MU_INSPECTOR_INSPECTORLISTMODEL_H
