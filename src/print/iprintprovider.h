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
#ifndef MU_PRINT_IPRINTPROVIDER_H
#define MU_PRINT_IPRINTPROVIDER_H

#include <functional>
#include <QSizeF>

#include "modularity/imoduleexport.h"
#include "ret.h"
#include "notation/inotation.h"

namespace mu::print {
class IPrintProvider : MODULE_EXPORT_INTERFACE
{
    INTERFACE_ID(IPrintProvider)

public:
    virtual ~IPrintProvider() = default;

    virtual Ret printNotation(notation::INotationPtr notation) = 0;
};
}

#endif // MU_PRINT_IPRINTPROVIDER_H
