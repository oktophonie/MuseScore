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
#ifndef MU_PROJECT_PROJECTCONFIGURATION_H
#define MU_PROJECT_PROJECTCONFIGURATION_H

#include "../iprojectconfiguration.h"
#include "modularity/ioc.h"
#include "iglobalconfiguration.h"
#include "extensions/iextensionsconfiguration.h"
#include "notation/inotationconfiguration.h"
#include "global/val.h"
#include "system/ifilesystem.h"

namespace mu::project {
class ProjectConfiguration : public IProjectConfiguration
{
    INJECT(project, framework::IGlobalConfiguration, globalConfiguration)
    INJECT(project, extensions::IExtensionsConfiguration, extensionsConfiguration)
    INJECT(project, notation::INotationConfiguration, notationConfiguration)
    INJECT(project, system::IFileSystem, fileSystem)

public:
    static const QString DEFAULT_FILE_SUFFIX;
    static const QString DEFAULT_EXPORT_SUFFIX;

    void init();

    io::paths recentProjectPaths() const override;
    void setRecentProjectPaths(const io::paths& recentScorePaths) override;
    async::Channel<io::paths> recentProjectPathsChanged() const override;

    io::path myFirstProjectPath() const override;

    io::paths availableTemplateDirs() const override;
    io::path templateCategoriesJsonPath(const io::path& templatesDir) const override;

    io::path userTemplatesPath() const override;
    void setUserTemplatesPath(const io::path& path) override;
    async::Channel<io::path> userTemplatesPathChanged() const override;

    io::path userProjectsPath() const override;
    void setUserProjectsPath(const io::path& path) override;
    async::Channel<io::path> userProjectsPathChanged() const override;

    io::path defaultSavingFilePath(const io::path& fileName) const override;

    QColor templatePreviewBackgroundColor() const override;
    async::Notification templatePreviewBackgroundChanged() const override;

    PreferredScoreCreationMode preferredScoreCreationMode() const override;
    void setPreferredScoreCreationMode(PreferredScoreCreationMode mode) override;

    MigrationOptions migrationOptions(MigrationType type) const override;
    void setMigrationOptions(MigrationType type, const MigrationOptions& opt, bool persistent = true) override;

    bool isAutoSaveEnabled() const override;
    void setAutoSaveEnabled(bool enabled) override;
    async::Channel<bool> autoSaveEnabledChanged() const override;

    int autoSaveIntervalMinutes() const override;
    void setAutoSaveInterval(int minutes) override;
    async::Channel<int> autoSaveIntervalChanged() const override;

private:
    io::paths parsePaths(const mu::Val& value) const;

    io::path appTemplatesPath() const;

    async::Channel<io::paths> m_recentProjectPathsChanged;
    async::Channel<io::path> m_userTemplatesPathChanged;
    async::Channel<io::path> m_userScoresPathChanged;

    async::Channel<bool> m_autoSaveEnabledChanged;
    async::Channel<int> m_autoSaveIntervalChanged;

    mutable std::map<MigrationType, MigrationOptions> m_migrationOptions;
};
}

#endif // MU_PROJECT_PROJECTCONFIGURATION_H
